#ifndef NOS_LOG_LOGMSG_H
#define NOS_LOG_LOGMSG_H

#include <ctime>
#include <cstdio>
#include <string>

#include <nos/log/level.h>

namespace nos
{
	namespace log
	{
		class tlogger;

		class logmsg
		{
		public:
			nos::log::tlogger * logger=nullptr; 
			level lvl={};
			std::string text={};
			std::tm timestamp={};

		public:
			logmsg(
				nos::log::tlogger * logger,
				nos::log::level lvl, 
				std::string && str, 
				std::tm & timestamp
			) :
				logger(logger),
				lvl(lvl),
				text(std::move(str)),
				timestamp(timestamp)
			{}

			logmsg(const logmsg&) = default;
			logmsg& operator= (const logmsg&) = default;
		};
	}
}

#endif
