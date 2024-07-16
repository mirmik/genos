/**
    @file
*/

#ifndef __SCPI_DEF_H_
#define __SCPI_DEF_H_

#include "scpi/scpi.h"
#include <sys/cdefs.h>

#define SCPI_INPUT_BUFFER_LENGTH 512
#define SCPI_ERROR_QUEUE_SIZE 34

extern const scpi_command_t scpi_commands[];
extern const scpi_command_t ncpi_commands[];
extern scpi_interface_t scpi_interface;
extern scpi_interface_t ncpi_interface;
extern char scpi_input_buffer[];
extern scpi_error_t scpi_error_queue_data[];
extern scpi_t scpi_context;

#include <utilxx/protected_set.h>
extern protected_set<scpi_t *> scpi_context_set;

void start_scpi_server(int port);
void stop_scpi_server();
void scpi_server_join();

__BEGIN_DECLS

size_t SCPI_Write(scpi_t *context, const char *data, size_t len);
int SCPI_Error(scpi_t *context, int_fast16_t err);
scpi_result_t
SCPI_Control(scpi_t *context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val);
scpi_result_t SCPI_Reset(scpi_t *context);
scpi_result_t SCPI_Flush(scpi_t *context);

void SCPI_threadFunc(int);

__END_DECLS

#endif /* __SCPI_DEF_H_ */
