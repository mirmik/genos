#ifndef CROW_SERVICE_H
#define CROW_SERVICE_H

#include <crow/addons/logger.h>

void init_crow_part();
void send_final_status_handle();
void send_start_status_handle();

extern crow::publish_logger helix_logger;

#endif