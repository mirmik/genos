#include "NetworkAnalyzer.h"
#include <mutex>
#include <queue>
#include <string>
#include <vector>

extern std::vector<std::pair<std::string, std::string>> traces;
extern NetworkAnalyzer netan;

extern bool discStart;

extern std::mutex netanlock;
extern std::string netantype;
extern int packno;

extern nos::inet::tcp_client informer;
extern nos::inet::tcp_client client;

extern std::queue<
    std::pair<std::shared_ptr<std::string>, std::shared_ptr<std::string>>>
    datq;
extern std::mutex datmutx;

using qtype = std::queue<
    std::pair<std::shared_ptr<std::string>, std::shared_ptr<std::string>>>;
using ptype =
    std::pair<std::shared_ptr<std::string>, std::shared_ptr<std::string>>;
using rawtype = std::shared_ptr<std::string>;

void listenerThread_SignalAnalyzerBasic_Func(void *);
void listenerThread_SignalAnalyzerMarker_Func(void *);

int disconnect();
