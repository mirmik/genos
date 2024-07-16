#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include <nos/fprint.h>
#include <nos/print.h>

#include <igris/math/convertors.h>
#include <ralgo/imu/madgwick.h>
#include <ralgo/linalg/linalg.h>

#include <ralgo/space/pose2.h>
#include <ralgo/space/pose3.h>
#include <ralgo/space/screw.h>

using namespace ralgo;
using namespace linalg;
namespace py = pybind11;

#define DOUBLE4 double, double, double, double
#define DOUBLE3 double, double, double
#define DOUBLE2 double, double

#define FLOAT4 float, float, float, float
#define FLOAT3 float, float, float
#define FLOAT2 float, float

void heimer_submodule(py::module &m);

PYBIND11_MODULE(libralgo, m)
{
    auto m2 = m.def_submodule("heimer", "Heimer");
    heimer_submodule(m2);

    // LINALG
    py::class_<linalg::vec<double, 2>>(m, "vec2", py::buffer_protocol())
        .def(py::init<DOUBLE2>())
        .def(py::init<const vec<double, 2> &>())
        .def("__str__",
             [](const linalg::vec<double, 2> &self) {
                 return nos::format("{}", self);
             })
        .def("__repr__",
             [](const linalg::vec<double, 2> &self) {
                 return nos::format("{}", self);
             })
        .def(py::init([](const py::buffer &b) {
            double x, y;
            auto info = b.request();
            if (info.format == py::format_descriptor<double>::format())
            {
                x = reinterpret_cast<double *>(b.request().ptr)[0];
                y = reinterpret_cast<double *>(b.request().ptr)[1];
            }
            else if (info.format == std::string("b") ||
                     info.format == std::string("h") ||
                     info.format == std::string("i") ||
                     info.format == std::string("l") ||
                     info.format == std::string("q"))
            {
                if (info.itemsize == 1)
                {
                    x = reinterpret_cast<int8_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int8_t *>(b.request().ptr)[1];
                }
                else if (info.itemsize == 2)
                {
                    x = reinterpret_cast<int16_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int16_t *>(b.request().ptr)[1];
                }
                else if (info.itemsize == 4)
                {
                    x = reinterpret_cast<int32_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int32_t *>(b.request().ptr)[1];
                }
                else if (info.itemsize == 8)
                {
                    x = reinterpret_cast<int64_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int64_t *>(b.request().ptr)[1];
                }
            }
            else
                throw std::runtime_error(
                    "Incompatible format: expected a double array!");
            return linalg::vec<double, 2>(x, y);
        }))
        .def("__getitem__",
             [](linalg::vec<double, 2> &v, int i) { return v[i]; })
        .def(py::init([](const py::tuple &b) {
            nos::println("HEERE tuple");
            return linalg::vec<double, 2>(b[0].cast<double>(),
                                          b[1].cast<double>());
        }))
        .def(py::init([](const py::list &b) {
            nos::println("HEERE list");
            return linalg::vec<double, 2>(b[0].cast<double>(),
                                          b[1].cast<double>());
        }))
        .def_buffer([](linalg::vec<double, 2> &v) -> py::buffer_info {
            return py::buffer_info(
                &v.x,           /* Pointer to buffer */
                sizeof(double), /* Size of one scalar */
                py::format_descriptor<double>::format(), /* Python struct-style
                                                            format descriptor */
                1,               /* Number of dimensions */
                {2},             /* Buffer dimensions */
                {sizeof(double)} /* Strides (in bytes) for each index */
            );
        });

    py::class_<linalg::vec<double, 3>>(m, "vec3", py::buffer_protocol())
        .def(py::init<DOUBLE3>())
        .def(py::init<const vec<double, 3> &>())
        .def("__str__",
             [](const linalg::vec<double, 3> &self) {
                 return nos::format("{}", self);
             })
        .def("__repr__",
             [](const linalg::vec<double, 3> &self) {
                 return nos::format("{}", self);
             })
        .def(py::init([](const py::buffer &b) {
            double x, y, z;
            auto info = b.request();
            if (info.format == py::format_descriptor<double>::format())
            {
                x = reinterpret_cast<double *>(b.request().ptr)[0];
                y = reinterpret_cast<double *>(b.request().ptr)[1];
                z = reinterpret_cast<double *>(b.request().ptr)[2];
            }
            else if (info.format == std::string("b") ||
                     info.format == std::string("h") ||
                     info.format == std::string("i") ||
                     info.format == std::string("l") ||
                     info.format == std::string("q"))
            {
                if (info.itemsize == 1)
                {
                    x = reinterpret_cast<int8_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int8_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int8_t *>(b.request().ptr)[2];
                }
                else if (info.itemsize == 2)
                {
                    x = reinterpret_cast<int16_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int16_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int16_t *>(b.request().ptr)[2];
                }
                else if (info.itemsize == 4)
                {
                    x = reinterpret_cast<int32_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int32_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int32_t *>(b.request().ptr)[2];
                }
                else if (info.itemsize == 8)
                {
                    x = reinterpret_cast<int64_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int64_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int64_t *>(b.request().ptr)[2];
                }
            }
            else
                throw std::runtime_error(
                    "Incompatible format: expected a double array!");
            return linalg::vec<double, 3>(x, y, z);
        }))
        //.def("__getitem__", linalg::vec<double,3>::operator[])
        .def(py::init([](py::tuple b) {
            return linalg::vec<double, 3>(
                b[0].cast<double>(), b[1].cast<double>(), b[2].cast<double>());
        }))
        .def(py::init([](py::list b) {
            return linalg::vec<double, 3>(
                b[0].cast<double>(), b[1].cast<double>(), b[2].cast<double>());
        }))
        .def_buffer([](linalg::vec<double, 3> &v) -> py::buffer_info {
            return py::buffer_info(
                &v.x,           /* Pointer to buffer */
                sizeof(double), /* Size of one scalar */
                py::format_descriptor<double>::format(), /* Python struct-style
                                                            format descriptor */
                1,               /* Number of dimensions */
                {3},             /* Buffer dimensions */
                {sizeof(double)} /* Strides (in bytes) for each index */
            );
        });

    py::class_<linalg::vec<double, 4>>(m, "vec4", py::buffer_protocol())
        .def(py::init<DOUBLE4>())
        .def(py::init<const vec<double, 4> &>())
        .def("__str__",
             [](const linalg::vec<double, 4> &self) {
                 return nos::format("{}", self);
             })
        .def("__repr__",
             [](const linalg::vec<double, 4> &self) {
                 return nos::format("{}", self);
             })
        .def(py::init([](const py::buffer &b) {
            double x, y, z, w;
            auto info = b.request();
            else if (info.format == std::string("b") ||
                     info.format == std::string("h") ||
                     info.format == std::string("i") ||
                     info.format == std::string("l") ||
                     info.format == std::string("q"))
            {
                if (info.itemsize == 1)
                {
                    x = reinterpret_cast<int8_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int8_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int8_t *>(b.request().ptr)[2];
                    w = reinterpret_cast<int8_t *>(b.request().ptr)[3];
                }
                else if (info.itemsize == 2)
                {
                    x = reinterpret_cast<int16_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int16_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int16_t *>(b.request().ptr)[2];
                    w = reinterpret_cast<int16_t *>(b.request().ptr)[3];
                }
                else if (info.itemsize == 4)
                {
                    x = reinterpret_cast<int32_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int32_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int32_t *>(b.request().ptr)[2];
                    w = reinterpret_cast<int32_t *>(b.request().ptr)[3];
                }
                else if (info.itemsize == 8)
                {
                    x = reinterpret_cast<int64_t *>(b.request().ptr)[0];
                    y = reinterpret_cast<int64_t *>(b.request().ptr)[1];
                    z = reinterpret_cast<int64_t *>(b.request().ptr)[2];
                    w = reinterpret_cast<int64_t *>(b.request().ptr)[3];
                }
            }
            else throw std::runtime_error(
                "Incompatible format: expected a double array!");
            return linalg::vec<double, 4>(x, y, z, w);
        }))
        //.def("__getitem__", linalg::vec<double,4>::operator[])
        .def(py::init([](py::tuple b) {
            return linalg::vec<double, 4>(b[0].cast<double>(),
                                          b[1].cast<double>(),
                                          b[2].cast<double>(),
                                          b[3].cast<double>());
        }))
        .def(py::init([](py::list b) {
            return linalg::vec<double, 4>(b[0].cast<double>(),
                                          b[1].cast<double>(),
                                          b[2].cast<double>(),
                                          b[3].cast<double>());
        }))
        .def_buffer([](linalg::vec<double, 4> &v) -> py::buffer_info {
            return py::buffer_info(
                &v.x,           /* Pointer to buffer */
                sizeof(double), /* Size of one scalar */
                py::format_descriptor<double>::format(), /* Python struct-style
                                                            format descriptor */
                1,               /* Number of dimensions */
                {4},             /* Buffer dimensions */
                {sizeof(double)} /* Strides (in bytes) for each index */
            );
        });

    py::implicitly_convertible<py::buffer, linalg::vec<double, 2>>();
    py::implicitly_convertible<py::buffer, linalg::vec<double, 3>>();
    py::implicitly_convertible<py::buffer, linalg::vec<double, 4>>();
    py::implicitly_convertible<py::tuple, linalg::vec<double, 2>>();
    py::implicitly_convertible<py::tuple, linalg::vec<double, 3>>();
    py::implicitly_convertible<py::tuple, linalg::vec<double, 4>>();
    py::implicitly_convertible<py::list, linalg::vec<double, 2>>();
    py::implicitly_convertible<py::list, linalg::vec<double, 3>>();
    py::implicitly_convertible<py::list, linalg::vec<double, 4>>();

    py::class_<linalg::vec<float, 3>>(m, "vec3f")
        .def(py::init<FLOAT3>())
        .def(py::init<const vec<float, 3> &>())
        .def("__str__", [](const linalg::vec<float, 3> &self) {
            return nos::format("{}", self);
        });

    py::class_<linalg::vec<float, 4>>(m, "vec4f")
        .def(py::init<FLOAT4>())
        .def(py::init<const vec<float, 4> &>())
        .def_readwrite("x", &linalg::vec<float, 4>::x)
        .def_readwrite("y", &linalg::vec<float, 4>::y)
        .def_readwrite("z", &linalg::vec<float, 4>::z)
        .def_readwrite("w", &linalg::vec<float, 4>::w)
        .def("__str__", [](const linalg::vec<float, 4> &self) {
            return nos::format("{}", self);
        });

    py::class_<ralgo::madgwick>(m, "madgwick")
        .def(py::init<>())
        .def("update",
             (void (madgwick::*)(float,
                                 float,
                                 float,
                                 float,
                                 float,
                                 float,
                                 float,
                                 float,
                                 float)) &
                 madgwick::update)
        .def("update",
             (void (madgwick::*)(float, float, float, float, float, float)) &
                 madgwick::update)
        .def("update",
             (void (madgwick::*)(float, float, float)) & madgwick::update)
        .def("quat", (linalg::vec<float, 4>(madgwick::*)()) & madgwick::quat)
        .def("reset", (void (madgwick::*)()) & madgwick::reset)
        //.def("setKoeff", &madgwick::setKoeff)
        .def("getPitchRad", &madgwick::getPitchRad)
        .def("getRollRad", &madgwick::getRollRad)
        .def("getYawRad", &madgwick::getYawRad)
        .def("getPitchDeg", &madgwick::getPitchDeg)
        .def("getRollDeg", &madgwick::getRollDeg)
        .def("getYawDeg", &madgwick::getYawDeg)
        .def("magnetic_reference_x", &madgwick::magnetic_reference_x)
        .def("magnetic_reference_y", &madgwick::magnetic_reference_y);

    py::class_<ralgo::pose2<double>>(m, "pose2")
        .def(py::init<>())
        .def(py::init<double, linalg::vec<double, 2>>())
        .def("inverse", &pose2<double>::inverse)
        .def("transform_vector", &pose2<double>::transform_vector)
        .def("transform_point", &pose2<double>::transform_point)
        .def("inverse_transform_vector",
             &pose2<double>::inverse_transform_vector)
        .def("inverse_transform_point", &pose2<double>::inverse_transform_point)
        .def("__str__",
             [](const pose2<double> &self) { return nos::format("{}", self); })
        .def("__repr__",
             [](const pose2<double> &self) { return nos::format("{}", self); })
        .def_readwrite("ang", &pose2<double>::ang)
        .def_readwrite("lin", &pose2<double>::lin);

    py::class_<ralgo::pose3<double>>(m, "pose3")
        .def(py::init<>())
        .def(py::init<linalg::vec<double, 4>, linalg::vec<double, 3>>())
        .def("inverse", &pose3<double>::inverse)
        .def("transform_vector", &pose3<double>::transform_vector)
        .def("transform_point", &pose3<double>::transform_point)
        .def("inverse_transform_vector",
             &pose3<double>::inverse_transform_vector)
        .def("inverse_transform_point", &pose3<double>::inverse_transform_point)
        .def("__mul__", &pose3<double>::operator*)
        .def("__str__",
             [](const pose3<double> &self) { return nos::format("{}", self); })
        .def("__repr__",
             [](const pose3<double> &self) { return nos::format("{}", self); })
        .def_readwrite("ang", &pose3<double>::ang)
        .def_readwrite("lin", &pose3<double>::lin);
}
