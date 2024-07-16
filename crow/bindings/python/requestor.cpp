#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

#include <crow/hostaddr.h>
#include <crow/nodes/pubsub_defs.h>
#include <crow/nodes/requestor_node.h>
#include <pybind11/embed.h>

#include <functional>
#include <string>

namespace py = pybind11;

#pragma GCC visibility push(hidden)
class pybind_requestor : public crow::requestor_node
{
    std::string theme;
    std::string reply_theme;
    crow::hostaddr addr;

public:
    pybind_requestor(crow::hostaddr addr,
                     std::string theme,
                     std::string reply_theme)
        : crow::requestor_node()
    {
        this->addr = addr;
        this->theme = theme;
        this->reply_theme = reply_theme;
        set_theme(this->theme);
        set_reply_theme(this->reply_theme);
        set_address(this->addr);
    }

    void async_request(const py::bytes &data)
    {
        std::string info = data;
        requestor_node::async_request(nos::buffer{info.data(), info.size()});
    }

    auto request(const py::bytes &data)
    {
        std::string info = data;
        return requestor_node::request(nos::buffer{info.data(), info.size()});
    }
};
#pragma GCC visibility pop

void register_requestor_class(py::module &m)
{
    py::class_<pybind_requestor>(m, "requestor")
        .def(py::init<crow::hostaddr, std::string, std::string>(),
             py::arg("addr"), py::arg("theme"), py::arg("reply_theme"))
        .def("async_request", &pybind_requestor::async_request, py::arg("data"))
        .def("request", &pybind_requestor::request, py::arg("data"));
}
