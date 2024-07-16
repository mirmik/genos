#ifndef IGRIS_OSUTIL_GETCWD_H
#define IGRIS_OSUTIL_GETCWD_H

#include <igris/string.h>
#include <string>
#include <sys/stat.h> // mkdir
#include <unistd.h>

#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>

namespace igris
{
    namespace osutil
    {
        static inline bool isexist(const std::string &path)
        {
            return access(path.c_str(), F_OK) != -1;
        }

        static inline bool isdir(const std::string &path)
        {
            struct stat statbuf;
            if (stat(path.c_str(), &statbuf) != 0)
                return 0;
            return S_ISDIR(statbuf.st_mode);
        }

        static inline void mkdir(const std::string &path, int access)
        {
            if (::mkdir(path.c_str(), access) == -1)
            {
                perror("mkdir");
                exit(EXIT_FAILURE);
            }
        }

        static std::vector<std::string> listdir(const std::string &path)
        {
            std::vector<std::string> ret;
            struct dirent *entry;

            DIR *dir = opendir(path.c_str());
            if (dir == NULL)
            {
                return {};
            }

            while ((entry = readdir(dir)) != NULL)
            {
                std::string f = entry->d_name;
                if (f != ".." && f != ".")
                    ret.push_back(f);
            }

            closedir(dir);

            return ret;
        }
    }
}

#endif
