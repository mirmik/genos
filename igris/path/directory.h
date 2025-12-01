#ifndef GXX_PATH_DIRECTORY_H
#define GXX_PATH_DIRECTORY_H

#include <gxx/path/file.h>
#include <gxx/path/path.h>

namespace gxx
{
    namespace path
    {
        class directory
        {

        public:
            directory(gxx::path pth);

            bool exist();

            bool make();
            bool make_recurse();
        }
    }
}

#endif
