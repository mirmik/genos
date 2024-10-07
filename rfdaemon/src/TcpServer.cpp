#include "TcpServer.h"
#include "arpa/inet.h"
#include "crc32_ccitt.h"
#include <cstring>
#include <modes.h>
#include <signal.h>
#include <unistd.h>

TcpServer::TcpServer(uint16_t port, size_t bufferSize)
{
    bufferLength = bufferSize;
    rxQueue.reserve(bufferSize * 2);
    txQueue.reserve(bufferSize * 2);

    // Prevent crash due to broken socket pipe
    signal(SIGPIPE, SIG_IGN);
    usedPort = port;
    setupConnection();
}

TcpServer::~TcpServer()
{
    terminateRxThread = true;
    terminateTxThread = true;

    connection.close();
    socket.close();

    while (terminateRxThread || terminateTxThread)
        ;
}

int TcpServer::receiveThread()
{
    for (;;)
    {
        if (!connectionAccepted)
        {
            usleep(10000);
            if (!connectionCreated)
                setupConnection();
            connection = socket.accept();
            if (!connection.good())
            {
                perror("Socket accept error.\n");
                socket.close();
                break;
            }

            connectionAccepted = true;
        }
        else
        {
            ssize_t result = 0;
            do
            {
                char *buffer = (char *)rxBufferPtr + rxBufferHeaderCollectCnt;
                auto eresult = connection.recv(buffer, bufferLength, 0);
                if (eresult.is_ok())
                    result = *eresult;
                else
                    result = -1;

                if (result > 0)
                {
                    uint8_t *dataStartPtr = rxBufferPtr;

                    if (!rxQueueActive)
                    {
                        if (rxBufferHeaderCollectCnt < sizeof(PacketHeader))
                            rxBufferHeaderCollectCnt += result;

                        if (rxBufferHeaderCollectCnt >= sizeof(PacketHeader))
                        {
                            result = rxBufferHeaderCollectCnt;
                            rxBufferHeaderCollectCnt =
                                0; // Clear header fragment size counter

                            // Received data contains header at start
                            PacketHeader *h = (PacketHeader *)&rxBufferPtr;
                            if (h->preamble == HeaderPreamble)
                            {
                                rxQueueActive = true;
                                rxQueue.clear();
                                currentHeader = *h;
                                dataStartPtr =
                                    rxBufferPtr + sizeof(PacketHeader);
                                result -= sizeof(PacketHeader);
                                if ((size_t)result >= h->size)
                                    rxQueue.insert(rxQueue.end(), dataStartPtr,
                                                   dataStartPtr + result);
                            }
                        }
                    }

                    if (rxQueueActive && (result > 0))
                    {
                        // Received data is not header, it is body part
                        if (rxQueue.size() < currentHeader.size)
                            rxQueue.insert(rxQueue.end(), dataStartPtr,
                                           dataStartPtr + result);
                        else
                        {
                            rxQueueActive = false;
                            if (crc32_ccitt(rxQueue.data(), currentHeader.size,
                                            0) == currentHeader.crc32)
                            {
                                while (txQueueActive)
                                    ;
                                mQueue.lock();
                                txQueue = parseReceivedData(rxQueue);
                                if (txQueue.empty())
                                    lastQueryResult = QueryResult::AppRejected;
                                else
                                    lastQueryResult = QueryResult::AllOk;
                                mQueue.unlock();
                            }
                            else
                                lastQueryResult = QueryResult::CRCError;
                            rxQueue.clear();
                        }
                    }
                }
                if (terminateRxThread)
                    break;
            } while (result > 0);
            rxQueueActive = false;
            connectionAccepted = false;
            if (result != 0)
                printf("Socket receive error %d, restart connection.\n",
                       (int)result);
            else
                printf("Client disconnected.\n");
            mConn.lock();
            connection.close();
            mConn.unlock();
            socket.close();
            connectionCreated = false;
        }
        if (terminateRxThread)
            break;
    }
    terminateRxThread = false;
    return 0;
}

int TcpServer::sendThread()
{
    for (;;)
    {
        if (connectionAccepted)
        {
            // Connection established
            if (!txQueue.empty())
            {
                // Transmission queue not null
                int headerOffset = 0;
                ssize_t result = 0;
                mQueue.lock();

                // Header and first data part not sent yet
                if (!txQueueActive)
                {
                    txQueueActive = true;
                    PacketHeader *h = (PacketHeader *)txBufferPtr;
                    h->preamble = HeaderPreamble;
                    h->size = (uint32_t)txQueue.size();
                    h->crc32 = crc32_ccitt(txQueue.data(), h->size, 0);
                    txQueuePos = 0;
                    headerOffset = sizeof(PacketHeader);
                }

                // Header and first part has been sent, transmit next parts
                if (txQueueActive)
                {
                    if (txQueuePos < txQueue.size())
                    {
                        // Multipacket transmission not finished, copy next data
                        // part to buffer
                        size_t packetSize =
                            std::min(txQueue.size() - txQueuePos + headerOffset,
                                     bufferLength);
                        memcpy(txBufferPtr + headerOffset,
                               txQueue.data() + txQueuePos,
                               packetSize - headerOffset);

                        // Send next data part
                        mConn.lock();

                        if (VERBOSE)
                        {
                            nos::println("Sending packet: ");
                            nos::print_dump(txBufferPtr, packetSize);
                        }

                        auto eresult =
                            connection.write(txBufferPtr, packetSize);
                        if (eresult.is_ok())
                            result = *eresult;
                        else
                            result = -1;

                        mConn.unlock();
                        if (result > 0)
                            txQueuePos += result - headerOffset;
                        else if (result < 0)
                        {
                            // In case of error at header transfer, reset header
                            // include flag
                            if (headerOffset)
                                txQueueActive = false;
                            printf("Server packet header send error.\n");
                        }
                    }
                    else
                    {
                        // Multipacket transmission finished, clear buffer
                        txQueue.clear();
                        txQueuePos = 0;
                        txQueueActive = false;
                    }
                }
                mQueue.unlock();
                if (result < 0)
                    printf("Socket send error.\n");
            }
            else
                usleep(1000);
        }
        else
            usleep(10000);
        if (terminateTxThread)
            break;
    }
    terminateTxThread = false;
    return 0;
}

size_t TcpServer::getRxQueueSize() const
{
    return rxQueue.size();
}

size_t TcpServer::getTxQueueSize() const
{
    return txQueue.size();
}

size_t TcpServer::getBufferSize() const
{
    return bufferLength;
}

bool TcpServer::clientConnected()
{
    return connectionAccepted;
}

void TcpServer::setupConnection()
{

    socket.init();
    socket.reusing(true);

    if (socket.bind("0.0.0.0", usedPort) == -1)
    {
        perror("Socket bind error.\n");
        socket.close();
        exit(EXIT_FAILURE);
    }
    socket.reusing(true);

    if (socket.listen(1) == -1)
    {
        perror("Socket listening error.\n");
        socket.close();
        exit(EXIT_FAILURE);
    }
    socket.reusing(true);
    connectionCreated = true;
}
