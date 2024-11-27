#ifndef COMMUNICATION_PLANNER_H
#define COMMUNICATION_PLANNER_H

#include <devices/Device.h>
#include <thread>
#include <vector>
#include <functional>
#include <semaphore>

class CommunicationTask
{
    public:
        virtual void execute() = 0;
};

// class DispathcerCommunicationTask : public CommunicationTask
// {
//     std::function<void()> function;
//     std::mutex mutex;

//     public:
//         void execute();
//         void wait();
// };

class DeviceCommunicationTask : public CommunicationTask
{
    Device& device;

    public:
        DeviceCommunicationTask(Device& device);
        void execute();
};

class CommunicationPlanner 
{
public:
    static CommunicationPlanner* instance;

private:
    std::vector<std::shared_ptr<CommunicationTask>> tasks_list;
    std::vector<std::shared_ptr<CommunicationTask>>::iterator it;
    std::thread updater_thread;

    //volatile bool prevent_updater_thread = false;

    //std::binary_semaphore syncronization_phase_semaphore;
    std::mutex communication_phase_semaphore;

    public:
        CommunicationPlanner();

        void pause_updater_thread();
        void resume_updater_thread(); 

        void add_task(std::shared_ptr<CommunicationTask> device);
        void remove_task(std::shared_ptr<CommunicationTask> device);

        void start_updater_thread();
        void updater_function();
        void updater_function_iteration_body();

        void acquire();
        void release();

        std::shared_ptr<DeviceCommunicationTask> find_task_of_device(Device& device);
};


#endif // COMMUNICATION_PLANNER_H