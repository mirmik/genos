#ifndef __SCPI_ERROR_H
#define __SCPI_ERROR_H

#include <igris/result.h>
//#include <igris/rpc/status.h>

/*class rpc_error : public igris::result_type::exception {
public:
    enum errtype_e {
        RPC_ARGUMENT_VALUE_ERROR,
        RPC_ARGUMENT_NUMBER_ERROR,
        RPC_FUNCTION_NOT_SUPPORTED_ERROR,
        RPC_FUNCTION_NOT_RELEASED_ERROR,
        RPC_INTERNAL_ERROR,
    };

    errtype_e errtype;

    rpc_error(errtype_e type) : errtype(type) {}

    const char* what() {
        switch (errtype) {
            case RPC_ARGUMENT_VALUE_ERROR: return "RPC_ARGUMENT_VALUE_ERROR";
            case RPC_ARGUMENT_NUMBER_ERROR: return "RPC_ARGUMENT_NUMBER_ERROR";
            case RPC_FUNCTION_NOT_SUPPORTED_ERROR: return
"RPC_FUNCTION_NOT_SUPPORTED_ERROR"; case RPC_FUNCTION_NOT_RELEASED_ERROR: return
"RPC_FUNCTION_NOT_RELEASED_ERROR"; case RPC_INTERNAL_ERROR: return
"RPC_INTERNAL_ERROR";
        }
    }
};*/

#endif // SCPI_ERROR_H
