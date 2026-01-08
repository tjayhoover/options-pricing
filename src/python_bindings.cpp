#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "variable.h"
#include "operators.h"
#include "tape.h"

namespace py = pybind11;

PYBIND11_MODULE(options_pricing_py, m) {
    m.doc() = "Simple reverse-mode AD core bindings";

    // Variable
    py::class_<Variable>(m, "Variable")
        .def(py::init([](py::object val){
            if (val.is_none()) return Variable(std::optional<double>());
            return Variable(val.cast<double>());
        }), py::arg("value") = py::none())
        .def("value", &Variable::value)
        .def("adjoint", &Variable::adjoint)
        .def("set_adjoint", &Variable::setAdjoint)
        .def("set_value", &Variable::setValue)
        .def("__add__", [](const Variable& a, const Variable& b) { return a + b; })
        .def("__sub__", [](const Variable& a, const Variable& b) { return a - b; })
        .def("__mul__", [](const Variable& a, const Variable& b) { return a * b; })
        .def("__truediv__", [](const Variable& a, const Variable& b) { return a / b; })
        ;

    // Expose common math operators
    m.def("sin", &sin, "sine of a Variable");
    m.def("cos", &cos, "cosine of a Variable");
    m.def("exp", &exp, "exponential of a Variable");

    // Tape utilities
    m.def("tape_execute_backpass", []() {
        Tape::get().execute_backpass();
    }, "Execute backward pass on global tape");

    m.def("tape_nodes_count", []() {
        return static_cast<size_t>(Tape::get().nodes.size());
    }, "Return number of nodes recorded on the tape");

    m.def("tape_reset", []() {
        Tape::get().reset();
    }, "Reset the tape by clearing all recorded nodes");
}
