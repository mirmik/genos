#include <buffered_channels.h>

#include <chrono>
#include <semaphore.h>
#include <string>
#include <thread>
#include <vector>

#include <main.h>
#include <nos/print.h>

using namespace std::chrono_literals;

static std::vector<std::vector<std::string>> buffered_channels_measure_names;
static std::thread informer_listener_thread;
// static std::thread data_requestor_thread;
// static std::vector<std::vector<std::string>>::iterator bufchannel_it;
static sem_t sem;
static std::mutex mtx;
static std::queue<
    std::tuple<std::chrono::time_point<std::chrono::high_resolution_clock>,
               size_t,
               size_t,
               std::vector<std::string>>>
    timequeue;
static std::chrono::milliseconds timelapse = 100ms;
static std::mutex can_clean_mtx;
static int buffer_channels_offset = 0;
size_t bnumber = 0;

std::vector<std::vector<std::string>>::iterator bufchannel_it;

struct data_record
{
    std::shared_ptr<std::string> adata;
    std::shared_ptr<std::string> bdata;
    size_t number;
};

std::queue<data_record> predatq;

void set_buffered_channels(const nos::trent &tr)
{
    nos::println("set_buffered_channels:", tr);

    mtx.lock();
    buffered_channels_measure_names.clear();
    for (auto &channel_tr : tr["measures"].as_list())
    {
        std::vector<std::string> measures;
        for (auto &measure_tr : channel_tr.as_list())
        {
            measures.push_back(measure_tr.as_string());
        }
        buffered_channels_measure_names.push_back(std::move(measures));
    }

    bnumber = 0;
    buffer_channels_offset = tr["offset"].as_numer();
    bufchannel_it = buffered_channels_measure_names.begin();
    mtx.unlock();
}

void set_buffered_channels_timelapse(double lapse)
{
    timelapse = std::chrono::milliseconds((int)lapse);
}

void informer_listener_routine()
{
    nos::println("BufferedChannels: informer_listener_routine -> enter");

    while (true)
    {
        mtx.lock();
        auto check = buffered_channels_measure_names.size() == 0;
        mtx.unlock();
        if (!check)
            break;
        std::this_thread::sleep_for(10ms);
    }

    char buf[128];

    while (1)
    {
        auto exret = nos::read_until_from(informer, {buf, 128}, "\n");

        if (exret.is_error())
        {
            nos::println("BufferedChannels: informer_listener_routine -> exit");
            return;
        }

        int ret = *exret;

        if (discStart)
            return;

        if (ret <= 0)
            return;

        {
            mtx.lock();
            size_t distance = std::distance(
                buffered_channels_measure_names.begin(), bufchannel_it);
            std::vector<std::string> traces = *bufchannel_it;

            auto request_time =
                std::chrono::high_resolution_clock::now() + timelapse;
            timequeue.push({request_time, bnumber++, distance, traces});

            bufchannel_it++;
            if (bufchannel_it == buffered_channels_measure_names.end())
                bufchannel_it = buffered_channels_measure_names.begin();
            mtx.unlock();
        }

        // nos::println("send sem", "timelapse:", timelapse.count());
        sem_post(&sem);
    }

    nos::println("BufferedChannels: informer_listener_routine -> leave");
}

void data_requestor_routine(size_t threadno)
{
    size_t bufdist;
    std::vector<std::string> traces;
    NetworkAnalyzer local_netan;
    local_netan.connect(netan.host_ip(), netan.host_port());

    size_t number = 0;
    while (true)
    {
        if (discStart)
            return;
        std::this_thread::sleep_for(1ms);

        // Получаем от контроллера уведомление о новой точке.
        // can_clean_mtx.unlock();
        sem_wait(&sem);

        if (discStart)
            return;
        // can_clean_mtx.lock();

        {
            mtx.lock();
            std::chrono::time_point<std::chrono::high_resolution_clock> tp;
            std::tie(tp, number, bufdist, traces) = timequeue.front();
            timequeue.pop();
            mtx.unlock();

            // nos::fprintln(
            //   "[DEBUG] worker:{} Get token with number {}", threadno,
            //   number);

            std::this_thread::sleep_until(tp);
        }

        std::shared_ptr<std::string> ampdata;
        if (netantype == "pna")
        {
            std::vector<rawtype> storage;
            // auto &traces = *bufit;

            for (int trace_index = 0; trace_index < traces.size();
                 ++trace_index)
            {
                // nos::println(
                //   "read trace:", traces[trace_index], "packno:", packno);
                // netanlock.lock();
                int chnum = bufdist + 1 + buffer_channels_offset;
                if (local_netan.send(nos::format("CALC{}:PAR:SEL \'{}\'",
                                                 chnum,
                                                 traces[trace_index])
                                         .c_str()) < 0)
                    goto __error__;
                if (local_netan.send(
                        nos::format("CALC{}:DATA? SDATA", chnum).c_str()) < 0)
                    goto __error__;

                std::this_thread::sleep_for(1ms);
                ampdata = local_netan.read_block();
                // netanlock.unlock();

                if (!ampdata)
                {
                    goto __error__;
                }

                storage.push_back(ampdata);
                std::this_thread::sleep_for(1ms);
            }

            datmutx.lock();
            for (int i = 0; i < storage.size(); ++i)
            {
                datq.push(std::make_pair(storage[i], storage[i]));

                // TODO: Добавить очередь для гарантированного порядка.
                // predatq.push({storage[i], storage[i], number});
            }
            datmutx.unlock();

            std::this_thread::sleep_for(1ms);
        }
        else if (netantype == "ena")
        {
            dprln("ena point");
        }
        else
        {
            dprln("wrong netan type");
        }
    }

__error__:
    local_netan.disconnect();
    // can_clean_mtx.unlock();
    client.write("err", 3);
    dprln("listenerThreadFunc::dataerror");
    disconnect();
    dprln("listenerThreadFunc::after_disc");
    return;
}

std::vector<std::thread> workers;
void start_buffered_channels_automate()
{
    size_t count_of_workers = 32;

    sem_init(&sem, 0, 0);
    informer_listener_thread = std::thread(informer_listener_routine);
    for (int i = 0; i < count_of_workers; ++i)
        workers.emplace_back(data_requestor_routine, i);
}

void stop_buffered_channels_automate()
{
    discStart = true;

    nos::println("informer join");
    informer_listener_thread.join();
    nos::println("requestor join");
    for (auto &thr : workers)
        sem_post(&sem);

    for (auto &thr : workers)
        thr.join();

    workers.clear();
    buffered_channels_measure_names.clear();

    nos::println("join...ok");
    sem_destroy(&sem);

    buffered_channels_clean();
}

void buffered_channels_clean()
{
    can_clean_mtx.lock();
    decltype(timequeue) empty;
    std::swap(timequeue, empty);
    can_clean_mtx.unlock();
}
