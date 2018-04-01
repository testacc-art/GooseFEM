/* =================================================================================================

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/GooseFEM

================================================================================================= */

#ifndef GOOSEFEM_ELEMENT_H
#define GOOSEFEM_ELEMENT_H

// -------------------------------------------------------------------------------------------------

#include "GooseFEM.h"

// ======================================= GooseFEM::Element =======================================

namespace GooseFEM {
namespace Element {

// -------------------------------------------------------------------------------------------------

// convert nodal vector [nnode, ndim] to corresponding element vector [nelem, nne, ndim]
ArrD asElementVector(const MatS &conn, const MatD &nodevec);

// assemble element vector [nelem, nne, ndim] to corresponding nodal vector [nnode, ndim]
MatD assembleElementVector(const MatS &conn, const ArrD &elemvec);

// -------------------------------------------------------------------------------------------------

}} // namespace ...

// =================================================================================================

#endif