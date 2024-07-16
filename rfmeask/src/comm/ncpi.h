/**
    @file
    Система уведомлений, работающая поверх протокола SCPI.
*/

#ifndef NCPI_H
#define NCPI_H

#include <comm/Notify.h>
#include <map>
#include <scpi/error.h>
#include <scpi/types.h>
#include <sys/cdefs.h>
#include <vector>

class NotifyClient;
class NotifyTheme;
class NotifyBroker;

extern NotifyBrocker brocker;

void start_ncpi_server(int port);
void stop_ncpi_server();
void ncpi_server_join();

__BEGIN_DECLS

/*Сервер NCPI*/
void NCPI_notifyThreadFunc(int port);

/*Обработчики внешних запросов*/
scpi_result_t NCPI_setNotification(scpi_t *context);
scpi_result_t NCPI_setNotification_without(scpi_t *context);
scpi_result_t NCPI_setSmoothNotification(scpi_t *context);
scpi_result_t NCPI_notificationQ(scpi_t *context);
scpi_result_t NCPI_unsubscribe(scpi_t *context);

/*private*/
size_t NCPI_Write(scpi_t *context, const char *data, size_t len);
scpi_result_t NCPI_Flush(scpi_t *context);
int NCPI_Error(scpi_t *context, int_fast16_t err);
scpi_result_t
NCPI_Control(scpi_t *context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val);
scpi_result_t NCPI_Reset(scpi_t *context);

__END_DECLS

#endif