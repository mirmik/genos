#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/axisctr.h>

namespace py = pybind11;
using namespace heimer;

void heimer_submodule(py::module &m)
{
    // auto signal_head_cls = py::class_<signal_head>(m, "signal_head")
    //                       .def("info", &signal_head::info);

    // auto axis_state_cls = py::class_<axis_state>(m, "axis_state",
    // signal_head_cls)
    //                      .def(py::init<>())
    //                      	.def(py::init<const char *>());

    // auto signal_processor_cls = py::class_<signal_processor>(m,
    // "signal_processor");

    // auto axis_controller_cls = py::class_<axis_controller>(m,
    // "axis_controller", signal_processor_cls) 	.def(py::init<>());
}
