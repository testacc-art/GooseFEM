/* =================================================================================================

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/GooseFEM

================================================================================================= */

#include <Eigen/Eigen>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

#include <pyxtensor/pyxtensor.hpp>

#include "../include/GooseFEM/GooseFEM.h"

// =================================================================================================

namespace py = pybind11;

// =================================================================================================

void init_VectorPartitioned(py::module &m)
{

py::class_<GooseFEM::VectorPartitioned>(m, "VectorPartitioned")

  .def(py::init<const xt::xtensor<size_t,2> &, const xt::xtensor<size_t,2> &, const xt::xtensor<size_t,1> &>(), "Switch between dofval/nodevec/elemvec", py::arg("conn"), py::arg("dofs"), py::arg("iip"))

  .def("nelem", &GooseFEM::VectorPartitioned::nelem, "Return number of element")
  .def("nne"  , &GooseFEM::VectorPartitioned::nne  , "Return number of nodes per element")
  .def("nnode", &GooseFEM::VectorPartitioned::nnode, "Return number of nodes")
  .def("ndim" , &GooseFEM::VectorPartitioned::ndim , "Return number of dimensions")
  .def("ndof" , &GooseFEM::VectorPartitioned::ndof , "Return number of degrees-of-freedom")
  .def("nnu"  , &GooseFEM::VectorPartitioned::nnu  , "Return number of unknown degrees-of-freedom")
  .def("nnp"  , &GooseFEM::VectorPartitioned::nnp  , "Return number of prescribed degrees-of-freedom")

  .def("dofs" , &GooseFEM::VectorPartitioned::dofs , "Return degrees-of-freedom")
  .def("iiu"  , &GooseFEM::VectorPartitioned::iiu  , "Return unknown degrees-of-freedom")
  .def("iip"  , &GooseFEM::VectorPartitioned::iip  , "Return prescribed degrees-of-freedom")

  .def("AsDofs", py::overload_cast<const xt::xtensor<double,1>&,const xt::xtensor<double,1>&>(&GooseFEM::VectorPartitioned::AsDofs, py::const_), "Set 'dofval" , py::arg("dofval_u"), py::arg("dofval_p"))
  .def("AsDofs", py::overload_cast<const xt::xtensor<double,2>&                             >(&GooseFEM::VectorPartitioned::AsDofs, py::const_), "Set 'dofval" , py::arg("nodevec"))
  .def("AsDofs", py::overload_cast<const xt::xtensor<double,3>&                             >(&GooseFEM::VectorPartitioned::AsDofs, py::const_), "Set 'dofval" , py::arg("elemvec"))

  .def("AsDofs_u", py::overload_cast<const xt::xtensor<double,2>&>(&GooseFEM::VectorPartitioned::AsDofs_u , py::const_), "Set 'dofval" , py::arg("nodevec"))
  .def("AsDofs_u", py::overload_cast<const xt::xtensor<double,3>&>(&GooseFEM::VectorPartitioned::AsDofs_u , py::const_), "Set 'dofval" , py::arg("elemvec"))
  .def("AsDofs_p", py::overload_cast<const xt::xtensor<double,2>&>(&GooseFEM::VectorPartitioned::AsDofs_p , py::const_), "Set 'dofval" , py::arg("nodevec"))
  .def("AsDofs_p", py::overload_cast<const xt::xtensor<double,3>&>(&GooseFEM::VectorPartitioned::AsDofs_p , py::const_), "Set 'dofval" , py::arg("elemvec"))

  .def("AsNode", py::overload_cast<const xt::xtensor<double,1>&,const xt::xtensor<double,1>&>(&GooseFEM::VectorPartitioned::AsNode, py::const_), "Set 'nodevec", py::arg("dofval_u"), py::arg("dofval_p"))
  .def("AsNode", py::overload_cast<const xt::xtensor<double,1>&                             >(&GooseFEM::VectorPartitioned::AsNode, py::const_), "Set 'nodevec", py::arg("dofval"))
  .def("AsNode", py::overload_cast<const xt::xtensor<double,3>&                             >(&GooseFEM::VectorPartitioned::AsNode, py::const_), "Set 'nodevec", py::arg("elemvec"))

  .def("AsElement", py::overload_cast<const xt::xtensor<double,1>&,const xt::xtensor<double,1>&>(&GooseFEM::VectorPartitioned::AsElement, py::const_), "Set 'elemvec", py::arg("dofval_u"), py::arg("dofval_p"))
  .def("AsElement", py::overload_cast<const xt::xtensor<double,1>&                             >(&GooseFEM::VectorPartitioned::AsElement, py::const_), "Set 'elemvec", py::arg("dofval"))
  .def("AsElement", py::overload_cast<const xt::xtensor<double,2>&                             >(&GooseFEM::VectorPartitioned::AsElement, py::const_), "Set 'elemvec", py::arg("nodevec"))

  .def("AssembleDofs"  , py::overload_cast<const xt::xtensor<double,2>&>(&GooseFEM::VectorPartitioned::AssembleDofs  , py::const_), "Assemble 'dofval'" , py::arg("nodevec"))
  .def("AssembleDofs"  , py::overload_cast<const xt::xtensor<double,3>&>(&GooseFEM::VectorPartitioned::AssembleDofs  , py::const_), "Assemble 'dofval'" , py::arg("elemvec"))
  .def("AssembleDofs_u", py::overload_cast<const xt::xtensor<double,2>&>(&GooseFEM::VectorPartitioned::AssembleDofs_u, py::const_), "Assemble 'dofval'" , py::arg("nodevec"))
  .def("AssembleDofs_u", py::overload_cast<const xt::xtensor<double,3>&>(&GooseFEM::VectorPartitioned::AssembleDofs_u, py::const_), "Assemble 'dofval'" , py::arg("elemvec"))
  .def("AssembleDofs_p", py::overload_cast<const xt::xtensor<double,2>&>(&GooseFEM::VectorPartitioned::AssembleDofs_p, py::const_), "Assemble 'dofval'" , py::arg("nodevec"))
  .def("AssembleDofs_p", py::overload_cast<const xt::xtensor<double,3>&>(&GooseFEM::VectorPartitioned::AssembleDofs_p, py::const_), "Assemble 'dofval'" , py::arg("elemvec"))

  .def("AssembleNode"  , py::overload_cast<const xt::xtensor<double,3>&>(&GooseFEM::VectorPartitioned::AssembleNode  , py::const_), "Assemble 'nodevec'", py::arg("elemvec"))

  .def("__repr__", [](const GooseFEM::VectorPartitioned &){ return "<GooseFEM.VectorPartitioned>"; });

}

// =================================================================================================

