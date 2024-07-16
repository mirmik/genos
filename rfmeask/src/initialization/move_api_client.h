#ifndef INITIALIZATION_MOVE_APICLIENT_H
#define INITIALIZATION_MOVE_APICLIENT_H

#include <moveapi/MoveApiClient.h>
#include <nos/trent/trent.h>

MoveApiClient *create_move_api_client_from_dict(const nos::trent &dict,
                                                const nos::trent &axdict,
                                                const std::string &name);

#endif