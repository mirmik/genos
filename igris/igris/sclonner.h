#ifndef IGRIS_CLONNER_H
#define IGRIS_CLONNER_H

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <pty.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <igris/datastruct/argvc.h>
#include <igris/event/delegate.h>
#include <igris/util/numconvert.h>
#include <memory>
#include <pty.h>
#include <set>
#include <unistd.h>
#include <vector>

#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/wait.h>
#endif

namespace igris
{
    class subprocess
    {
    public:
        int _pid = 0;
        int _master_fd = -1;  // PTY master fd (для чтения и записи)

        igris::delegate<void> on_close = {};

    public:
        subprocess() = default;
        subprocess(const char *data)
        {
            exec(data);
        }
        
        ~subprocess()
        {
            close();
        }

        void sigchld()
        {
            on_close.invoke();
        }

        void set_pid(int pid)
        {
            this->_pid = pid;
        }

        int pid() const
        {
            return _pid;
        }

        void invalidate()
        {
            close();
            _pid = 0;
            _master_fd = -1;
        }

        void terminate()
        {
            if (_pid > 0)
                ::kill(_pid, SIGTERM);
        }

        void kill()
        {
            if (_pid > 0)
                ::kill(_pid, SIGKILL);
        }

        int wait()
        {
            if (_pid <= 0)
                return -1;
            int status = 0;
            waitpid(_pid, &status, 0);
            _pid = 0;
            return WEXITSTATUS(status);
        }

        int input_fd()
        {
            return _master_fd;
        }

        int output_fd()
        {
            return _master_fd;
        }

        void close()
        {
            if (_master_fd >= 0)
            {
                ::close(_master_fd);
                _master_fd = -1;
            }
        }

        
        void exec(const std::string &name,
                  const std::vector<std::string> &args,
                  const std::vector<std::string> &env)
        {
            std::vector<char *> cargs;
            std::vector<char *> cenv;

            for (auto &arg : args)
                cargs.push_back(const_cast<char *>(arg.c_str()));
            cargs.push_back(nullptr);

            for (auto &e : env)
                cenv.push_back(const_cast<char *>(e.c_str()));
            cenv.push_back(nullptr);

            exec(name, cargs, cenv);
        }

        void exec(const std::string &name,
                  const std::vector<char *> &args,
                  const std::vector<char *> &env)
        {
            // Закрываем старый fd если был
            close();
            
            int master_fd;
            int pid = forkpty(&master_fd, nullptr, nullptr, nullptr);
            
            if (pid < 0)
            {
                perror("forkpty");
                return;
            }
            
            if (pid == 0)
            {
                // Child process
                // Используем execvp/execvpe для поиска в PATH
                if (env.empty() || env[0] == nullptr)
                {
                    execvp(name.c_str(), args.data());
                }
                else
                {
                    // execvpe - расширение GNU для execve с поиском в PATH
                    execvpe(name.c_str(), args.data(), env.data());
                }
                
                // Если exec* не удался
                perror("exec");
                _exit(127);
            }

            // Parent process
            _pid = pid;
            _master_fd = master_fd;
        }

        void exec(const char *ccmd)
        {
            // Закрываем старый fd если был  
            close();
            
            int pipes_host_in_child_out[2];
            int pipes_host_out_child_in[2];
            
            if (pipe(pipes_host_in_child_out) < 0)
            {
                perror("pipe");
                return;
            }
            if (pipe(pipes_host_out_child_in) < 0)
            {
                perror("pipe");
                ::close(pipes_host_in_child_out[0]);
                ::close(pipes_host_in_child_out[1]);
                return;
            }
            
            int pid = fork();
            if (pid < 0)
            {
                perror("fork");
                ::close(pipes_host_in_child_out[0]);
                ::close(pipes_host_in_child_out[1]);
                ::close(pipes_host_out_child_in[0]);
                ::close(pipes_host_out_child_in[1]);
                return;
            }
            
            if (pid == 0)
            {
                // Child process
                ::close(pipes_host_in_child_out[0]);
                ::close(pipes_host_out_child_in[1]);
                
                dup2(pipes_host_in_child_out[1], STDOUT_FILENO);
                dup2(pipes_host_in_child_out[1], STDERR_FILENO);
                dup2(pipes_host_out_child_in[0], STDIN_FILENO);
                
                ::close(pipes_host_in_child_out[1]);
                ::close(pipes_host_out_child_in[0]);

                char *cmd = strdup(ccmd);
                char *argv[10];
                int argc = argvc_internal_split(cmd, argv, 10);
                argv[argc] = nullptr;

                execv(argv[0], argv);
                perror("execv");
                free(cmd);
                _exit(127);
            }

            // Parent process
            ::close(pipes_host_out_child_in[0]);
            ::close(pipes_host_in_child_out[1]);

            _pid = pid;
            // Для простого pipe используем только read pipe как master_fd
            // (write pipe закрываем, т.к. обычно не нужен)
            _master_fd = pipes_host_in_child_out[0];
            ::close(pipes_host_out_child_in[1]);
        }
    };

    class sclonner
    {
        static std::set<std::shared_ptr<subprocess>> _childs;

    public:
        static std::shared_ptr<subprocess> start_subprocess(const char *ccmd)
        {
            auto child = std::make_shared<igris::subprocess>(ccmd);
            _childs.emplace(child);
            return child;
        }

        static void terminate_childs()
        {
            for (auto child : _childs)
                child->terminate();

            _childs.clear();
        }

        static void sigchild_trap(int)
        {
            pid_t pid;
            int status;
            while ((pid = waitpid(-1, &status, WNOHANG)) != -1)
            {
                auto it =
                    std::find_if(_childs.begin(),
                                 _childs.end(),
                                 [pid](auto &a) { return a->pid() == pid; });

                if (it != _childs.end())
                    (*it)->sigchld();
            }
        }

        static void install_sigchild_trap()
        {
            signal(SIGCHLD, sigchild_trap);
        }

        static const std::set<std::shared_ptr<subprocess>> &childs()
        {
            return _childs;
        }

        ~sclonner()
        {
            terminate_childs();
        }
    };
}

#endif
