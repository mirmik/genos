#pragma once

#include <mutex>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>
#include <stdint.h>
#include <vector>

typedef struct
{
    uint32_t preamble;
    uint32_t crc32;
    uint32_t size;
} PacketHeader;

enum class QueryResult
{
    AllOk = 0,
    CRCError,
    AppRejected
};

class TcpServer
{
public:
    TcpServer(uint16_t port, size_t bufferSize = 65535);
    ~TcpServer();
    int receiveThread();
    int sendThread();
    size_t getRxQueueSize() const;
    size_t getTxQueueSize() const;
    size_t getBufferSize() const;
    bool clientConnected();
    virtual std::vector<uint8_t>
    parseReceivedData(const std::vector<uint8_t> &data) = 0;

private:
    void setupConnection();
    static constexpr uint32_t HeaderPreamble = 0x69EA23BE;
    bool connectionCreated = false;
    bool connectionAccepted = false;
    size_t bufferLength = 0;
    size_t txQueuePos = 0;
    // Byte counter for receiving packet header in multistep mode
    // when recv() returns less bytes than size of header
    size_t rxBufferHeaderCollectCnt = 0;
    uint8_t rxBufferPtr[65535 + sizeof(PacketHeader)],
        txBufferPtr[65535 + sizeof(PacketHeader)];
    bool rxQueueActive = false;
    bool txQueueActive = false;
    std::vector<uint8_t> rxQueue, txQueue;
    PacketHeader currentHeader;
    QueryResult lastQueryResult = QueryResult::AllOk;
    uint16_t usedPort = 0;
    nos::inet::tcp_server socket;
    nos::inet::tcp_client connection;

    bool terminateRxThread = false;
    bool terminateTxThread = false;
    std::mutex mQueue;
    std::mutex mConn;
};
