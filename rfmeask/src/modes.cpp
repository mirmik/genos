/**
    @file
*/

bool NOFAULT = true;
bool PARSE_ONLY = false;
// int HELIX_QOS = 0;
bool CROW_DIAG = false;
// bool HELIX_UPDATE = true;

bool DISABLE_REMOTER = false;

// int system_ctrmode = -1;
bool FORCE_DEBUG = false;
bool FORCE_UPDATE_SIMULATOR_SCRIPTS = false;
//bool WITH_REMOTER = false;

bool with_remoter()
{
    return !DISABLE_REMOTER;
}