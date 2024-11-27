
// #include <devices/Device.h>
// #include <CommunicationPlanner.h>

// CommunicationPlanner* CommunicationPlanner::instance;

// DeviceCommunicationTask::DeviceCommunicationTask(Device& device)
//     : device(device) {}
 
// void DeviceCommunicationTask::execute() 
// {
//     device.updaterThreadFunc_iteration();
// }

// CommunicationPlanner::CommunicationPlanner() 
// {
//     CommunicationPlanner::instance = this;
// }
 
// void CommunicationPlanner::add_task(std::shared_ptr<CommunicationTask> task) 
// {
//     tasks_list.push_back(task);
// }

// void CommunicationPlanner::remove_task(std::shared_ptr<CommunicationTask> task) 
// {
//     tasks_list.erase(std::remove(tasks_list.begin(), tasks_list.end(), task), tasks_list.end());
// }

// void CommunicationPlanner::updater_function_iteration_body() 
// {
//     try {
//         (*it++)->execute();
//     } catch (const std::exception& e) 
//     {
//         nos::println("Exception in updater thread: ", e.what());
//     }
//     if (it == tasks_list.end()) 
//         it = tasks_list.begin();
// }

// void CommunicationPlanner::updater_function() 
// {
//     while (true) 
//     {
//         acquire();
//         updater_function_iteration_body();
//         release();
//     }
// }

// void CommunicationPlanner::acquire() 
// {
//     //syncronization_phase_semaphore.acquire();
//     //communication_phase_semaphore.acquire();
//     communication_phase_semaphore.lock();
//     //syncronization_phase_semaphore.release();
// }

// void CommunicationPlanner::release() 
// {
//     //communication_phase_semaphore.release();
//     communication_phase_semaphore.unlock();
// }

// void CommunicationPlanner::pause_updater_thread() 
// {
//     acquire();
// }

// void CommunicationPlanner::resume_updater_thread()
// {
//     release();
// } 

// void CommunicationPlanner::start_updater_thread() 
// {
//     if (tasks_list.empty()) 
//         return;

//     it = tasks_list.begin();
//     updater_thread = std::thread(&CommunicationPlanner::updater_function, this);    
// }
