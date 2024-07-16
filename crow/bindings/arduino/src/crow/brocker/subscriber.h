/** @file */

#ifndef CROWKER_SUBSCRIBER_H
#define CROWKER_SUBSCRIBER_H

#include "options.h"
#include "theme.h"

#include <unordered_map>

namespace crowker_implementation
{
    struct themenote
    {
        options *opts = nullptr;
        ~themenote() { delete opts; }
    };

    class subscriber
    {
      public:
        std::unordered_map<theme *, themenote> thms;

      public:
        ~subscriber();
        virtual void publish(const std::string &theme, const std::string &data,
                             options *opts) = 0;
    };
} // namespace crowker_implementation

#endif