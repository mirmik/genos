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
//#include <sys/wait.h>
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
        int _ipipe = 0;
        int _opipe = 0;

        igris::delegate<void> on_close = {};

    public:
        subprocess() = default;
        subprocess(const char *data)
        {
            exec(data);
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
            if (_pid != 0)
            {
                terminate();
            }

            _pid = _ipipe = _opipe = 0;
        }

        void terminate()
        {
            ::kill(_pid, SIGTERM);
        }

        void kill()
        {
            ::kill(_pid, SIGKILL);
        }

        void wait()
        {
            int status;
            waitpid(_pid, &status, WCONTINUED);
        }

        void set_pipe_fds(int ipipe, int opipe)
        {
            this->_ipipe = ipipe;
            this->_opipe = opipe;
        }

        int input_fd()
        {
            return _opipe;
        }

        int output_fd()
        {
            return _ipipe;
        }

        // void enable_terminal_policy()
        // {
        //     ioctl(STDOUT_FILENO, TIOC, )
        // }

        void close()
        {
            ::close(_ipipe);
            ::close(_opipe);
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

            for (auto &env : env)
                cenv.push_back(const_cast<char *>(env.c_str()));
            cenv.push_back(nullptr);

            exec(name, cargs, cenv);
        }

        void exec(const std::string &name,
                  const std::vector<char *> &args,
                  const std::vector<char *> &env)
        {
            int sts;
            // int pipes_host_in_child_out[2];
            // int pipes_host_out_child_in[2];
            // pipe2(pipes_host_in_child_out, O_NONBLOCK | O_DIRECT);
            // pipe2(pipes_host_out_child_in, O_NONBLOCK | O_DIRECT);
            int fd;
            int pid = forkpty(&fd, 0, 0, 0);
            if (pid == 0)
            {
                // close(pipes_host_in_child_out[0]);
                // close(pipes_host_out_child_in[1]);
                // lose(STDOUT_FILENO);

                // sts = dup2(pipes_host_in_child_out[1], STDOUT_FILENO);
                char *cmd = strdup(name.c_str());
                // char *argv[10];
                //  int argc = argvc_internal_split(cmd, argv, 10);
                //  argv[argc] = 0;

                struct termios settings;
                tcgetattr(STDOUT_FILENO, &settings);
                settings.c_oflag &= ~OPOST;
                settings.c_oflag &= ~ONLCR;
                tcgetattr(STDOUT_FILENO, &settings);

                sts = execve(cmd, args.data(), env.data());
                if (sts == -1)
                {
                    perror("occasion");
                }

                exit(0);
                // unreached
            }

            // close(pipes_host_out_child_in[0]);
            // close(pipes_host_in_child_out[1]);

            set_pid(pid);
            set_pipe_fds(fd, fd);
        }

        void exec(const char *ccmd)
        {
            int sts;
            int pipes_host_in_child_out[2];
            int pipes_host_out_child_in[2];
            pipe(pipes_host_in_child_out);
            pipe(pipes_host_out_child_in);
            int pid = fork();
            if (pid == 0)
            {
                ::close(pipes_host_in_child_out[0]);
                ::close(pipes_host_out_child_in[1]);
                ::close(STDOUT_FILENO);

                sts = dup2(pipes_host_in_child_out[1], STDOUT_FILENO);
                char *cmd = strdup(ccmd);
                char *argv[10];
                int argc = argvc_internal_split(cmd, argv, 10);
                argv[argc] = 0;

                sts = ::execve(argv[0], argv, NULL);
                if (sts == -1)
                {
                    perror("occasion");
                }

                exit(0);
                // unreached
            }

            ::close(pipes_host_out_child_in[0]);
            ::close(pipes_host_in_child_out[1]);

            set_pid(pid);
            set_pipe_fds(pipes_host_in_child_out[0],
                         pipes_host_out_child_in[1]);
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
