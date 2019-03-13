/* =================================================================================================

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/GooseFEM

================================================================================================= */

#ifndef GOOSEFEM_VECTORPARTITIONED_HPP
#define GOOSEFEM_VECTORPARTITIONED_HPP

// -------------------------------------------------------------------------------------------------

#include "VectorPartitioned.h"
#include "Mesh.h"

// =================================================================================================

namespace GooseFEM {

// -------------------------------------------------------------------------------------------------

inline VectorPartitioned::VectorPartitioned(
  const xt::xtensor<size_t,2>& conn,
  const xt::xtensor<size_t,2>& dofs,
  const xt::xtensor<size_t,1>& iip) :
  m_conn(conn),
  m_dofs(dofs),
  m_iip(iip)
{
  m_nelem = m_conn.shape()[0];
  m_nne   = m_conn.shape()[1];
  m_nnode = m_dofs.shape()[0];
  m_ndim  = m_dofs.shape()[1];

  m_iiu   = xt::setdiff1d(dofs, iip);

  m_ndof  = xt::amax(m_dofs)[0] + 1;
  m_nnp   = m_iip.size();
  m_nnu   = m_iiu.size();

  m_part  = Mesh::Reorder({m_iiu, m_iip}).get(m_dofs);

  assert(xt::amax(m_conn)[0] + 1 == m_nnode);
  assert(xt::amax(m_iip)[0] <= xt::amax(m_dofs)[0]);
  assert(m_ndof <= m_nnode * m_ndim);
}

// -------------------------------------------------------------------------------------------------

inline size_t VectorPartitioned::nelem() const
{ return m_nelem; }

inline size_t VectorPartitioned::nne() const
{ return m_nne; }

inline size_t VectorPartitioned::nnode() const
{ return m_nnode; }

inline size_t VectorPartitioned::ndim() const
{ return m_ndim; }

inline size_t VectorPartitioned::ndof() const
{ return m_ndof; }

inline size_t VectorPartitioned::nnu() const
{ return m_nnu; }

inline size_t VectorPartitioned::nnp() const
{ return m_nnp; }

inline xt::xtensor<size_t,2> VectorPartitioned::dofs() const
{ return m_dofs; }

inline xt::xtensor<size_t,1> VectorPartitioned::iiu() const
{ return m_iiu; }

inline xt::xtensor<size_t,1> VectorPartitioned::iip() const
{ return m_iip; }

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::copy(
  const xt::xtensor<double,2>& nodevec_src,
        xt::xtensor<double,2>& nodevec_dest) const
{
  assert(nodevec_src .shape()[0] == m_nnode);
  assert(nodevec_src .shape()[1] == m_ndim );
  assert(nodevec_dest.shape()[0] == m_nnode);
  assert(nodevec_dest.shape()[1] == m_ndim );

  xt::noalias(nodevec_dest) = nodevec_src;
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::copy_u(
  const xt::xtensor<double,2>& nodevec_src,
        xt::xtensor<double,2>& nodevec_dest) const
{
  assert(nodevec_src .shape()[0] == m_nnode);
  assert(nodevec_src .shape()[1] == m_ndim );
  assert(nodevec_dest.shape()[0] == m_nnode);
  assert(nodevec_dest.shape()[1] == m_ndim );

  #pragma omp parallel for
  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      if (m_part(m,i) < m_nnu)
        nodevec_dest(m,i) = nodevec_src(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::copy_p(
  const xt::xtensor<double,2>& nodevec_src,
        xt::xtensor<double,2>& nodevec_dest) const
{
  assert(nodevec_src .shape()[0] == m_nnode);
  assert(nodevec_src .shape()[1] == m_ndim );
  assert(nodevec_dest.shape()[0] == m_nnode);
  assert(nodevec_dest.shape()[1] == m_ndim );

  #pragma omp parallel for
  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      if (m_part(m,i) >= m_nnu)
        nodevec_dest(m,i) = nodevec_src(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs(
  const xt::xtensor<double,1>& dofval_u,
  const xt::xtensor<double,1>& dofval_p,
        xt::xtensor<double,1>& dofval) const
{
  assert(dofval_u.size() == m_nnu );
  assert(dofval_p.size() == m_nnp );
  assert(dofval.size()   == m_ndof);

  #pragma omp parallel for
  for (size_t d = 0 ; d < m_nnu ; ++d)
    dofval(m_iiu(d)) = dofval_u(d);

  #pragma omp parallel for
  for (size_t d = 0 ; d < m_nnp ; ++d)
    dofval(m_iip(d)) = dofval_p(d);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs(
  const xt::xtensor<double,2>& nodevec,
        xt::xtensor<double,1>& dofval) const
{
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );
  assert(dofval.size()      == m_ndof );

  #pragma omp parallel for
  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      dofval(m_dofs(m,i)) = nodevec(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs_u(
  const xt::xtensor<double,1>& dofval,
        xt::xtensor<double,1>& dofval_u) const
{
  assert(dofval.size()   == m_ndof);
  assert(dofval_u.size() == m_nnu );

  #pragma omp parallel for
  for (size_t d = 0 ; d < m_nnu ; ++d)
    dofval_u(d) = dofval(m_iiu(d));
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs_u(
  const xt::xtensor<double,2>& nodevec,
        xt::xtensor<double,1>& dofval_u) const
{
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );
  assert(dofval_u.size()    == m_nnu  );

  #pragma omp parallel for
  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      if (m_part(m,i) < m_nnu)
        dofval_u(m_part(m,i)) = nodevec(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs_p(
  const xt::xtensor<double,1>& dofval,
        xt::xtensor<double,1>& dofval_p) const
{
  assert(dofval.size()   == m_ndof);
  assert(dofval_p.size() == m_nnp );

  #pragma omp parallel for
  for (size_t d = 0 ; d < m_nnp ; ++d)
    dofval_p(d) = dofval(m_iip(d));
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs_p(
  const xt::xtensor<double,2>& nodevec,
        xt::xtensor<double,1>& dofval_p) const
{
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );
  assert(dofval_p.size()    == m_nnp  );

  #pragma omp parallel for
  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      if (m_part(m,i) >= m_nnu)
        dofval_p(m_part(m,i)-m_nnu) = nodevec(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,1>& dofval) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(dofval.size()      == m_ndof );

  #pragma omp parallel for
  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        dofval(m_dofs(m_conn(e,m),i)) = elemvec(e,m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs_u(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,1>& dofval_u) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(dofval_u.size()    == m_nnu  );

  #pragma omp parallel for
  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        if (m_part(m_conn(e,m),i) < m_nnu)
          dofval_u(m_part(m_conn(e,m),i)) = elemvec(e,m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asDofs_p(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,1>& dofval_p) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(dofval_p.size()    == m_nnp  );

  #pragma omp parallel for
  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        if (m_part(m_conn(e,m),i) >= m_nnu)
          dofval_p(m_part(m_conn(e,m),i)-m_nnu) = elemvec(e,m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asNode(
  const xt::xtensor<double,1>& dofval,
        xt::xtensor<double,2>& nodevec) const
{
  assert(dofval.size()      == m_ndof );
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );

  #pragma omp parallel for
  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      nodevec(m,i) = dofval(m_dofs(m,i));
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asNode(
  const xt::xtensor<double,1>& dofval_u,
  const xt::xtensor<double,1>& dofval_p,
        xt::xtensor<double,2>& nodevec) const
{
  assert(dofval_u.size()    == m_nnu  );
  assert(dofval_p.size()    == m_nnp  );
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );

  #pragma omp parallel for
  for (size_t m = 0 ; m < m_nnode ; ++m) {
    for (size_t i = 0 ; i < m_ndim ; ++i) {
      if (m_part(m,i) < m_nnu)
        nodevec(m,i) = dofval_u(m_part(m,i));
      else
        nodevec(m,i) = dofval_p(m_part(m,i)-m_nnu);
    }
  }
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asNode(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,2>& nodevec) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );

  #pragma omp parallel for
  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        nodevec(m_conn(e,m),i) = elemvec(e,m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asElement(
  const xt::xtensor<double,1>& dofval,
        xt::xtensor<double,3>& elemvec) const
{
  assert(dofval.size()      == m_ndof );
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );

  #pragma omp parallel for
  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        elemvec(e,m,i) = dofval(m_dofs(m_conn(e,m),i));
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asElement(
  const xt::xtensor<double,1>& dofval_u,
  const xt::xtensor<double,1>& dofval_p,
        xt::xtensor<double,3>& elemvec) const
{
  assert(dofval_u.size()    == m_nnu  );
  assert(dofval_p.size()    == m_nnp  );
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );

  #pragma omp parallel for
  for (size_t e = 0 ; e < m_nelem ; ++e) {
    for (size_t m = 0 ; m < m_nne ; ++m) {
      for (size_t i = 0 ; i < m_ndim ; ++i) {
        if (m_part(m_conn(e,m),i)<m_nnu)
          elemvec(e,m,i) = dofval_u(m_part(m_conn(e,m),i));
        else
          elemvec(e,m,i) = dofval_p(m_part(m_conn(e,m),i)-m_nnu);
      }
    }
  }
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::asElement(
  const xt::xtensor<double,2>& nodevec,
        xt::xtensor<double,3>& elemvec) const
{
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );

  #pragma omp parallel for
  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        elemvec(e,m,i) = nodevec(m_conn(e,m),i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::assembleDofs(
  const xt::xtensor<double,2>& nodevec,
        xt::xtensor<double,1>& dofval) const
{
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );
  assert(dofval.size()      == m_ndof );

  dofval.fill(0.0);

  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      dofval(m_dofs(m,i)) += nodevec(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::assembleDofs_u(
  const xt::xtensor<double,2>& nodevec,
        xt::xtensor<double,1>& dofval_u) const
{
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );
  assert(dofval_u.size()    == m_nnu  );

  dofval_u.fill(0.0);

  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      if (m_part(m,i) < m_nnu)
        dofval_u(m_part(m,i)) += nodevec(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::assembleDofs_p(
  const xt::xtensor<double,2>& nodevec,
        xt::xtensor<double,1>& dofval_p) const
{
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );
  assert(dofval_p.size()    == m_nnp  );

  dofval_p.fill(0.0);

  for (size_t m = 0 ; m < m_nnode ; ++m)
    for (size_t i = 0 ; i < m_ndim ; ++i)
      if (m_part(m,i) >= m_nnu)
        dofval_p(m_part(m,i)-m_nnu) += nodevec(m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::assembleDofs(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,1>& dofval) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(dofval.size()      == m_ndof );

  dofval.fill(0.0);

  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
          dofval(m_dofs(m_conn(e,m),i)) += elemvec(e,m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::assembleDofs_u(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,1>& dofval_u) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(dofval_u.size()    == m_nnu  );

  dofval_u.fill(0.0);

  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        if (m_part(m_conn(e,m),i) < m_nnu)
          dofval_u(m_part(m_conn(e,m),i)) += elemvec(e,m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::assembleDofs_p(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,1>& dofval_p) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(dofval_p.size()    == m_nnp  );

  dofval_p.fill(0.0);

  for (size_t e = 0 ; e < m_nelem ; ++e)
    for (size_t m = 0 ; m < m_nne ; ++m)
      for (size_t i = 0 ; i < m_ndim ; ++i)
        if (m_part(m_conn(e,m),i) >= m_nnu)
          dofval_p(m_part(m_conn(e,m),i)-m_nnu) += elemvec(e,m,i);
}

// -------------------------------------------------------------------------------------------------

inline void VectorPartitioned::assembleNode(
  const xt::xtensor<double,3>& elemvec,
        xt::xtensor<double,2>& nodevec) const
{
  assert(elemvec.shape()[0] == m_nelem);
  assert(elemvec.shape()[1] == m_nne  );
  assert(elemvec.shape()[2] == m_ndim );
  assert(nodevec.shape()[0] == m_nnode);
  assert(nodevec.shape()[1] == m_ndim );

  xt::xtensor<double,1> dofval = this->AssembleDofs(elemvec);

  this->asNode(dofval, nodevec);
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs(
  const xt::xtensor<double,1>& dofval_u,
  const xt::xtensor<double,1>& dofval_p) const
{
  xt::xtensor<double,1> dofval = xt::empty<double>({m_ndof});

  this->asDofs(dofval_u, dofval_p, dofval);

  return dofval;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs(
  const xt::xtensor<double,2>& nodevec) const
{
  xt::xtensor<double,1> dofval = xt::empty<double>({m_ndof});

  this->asDofs(nodevec, dofval);

  return dofval;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs_u(
  const xt::xtensor<double,1>& dofval) const
{
  xt::xtensor<double,1> dofval_u = xt::empty<double>({m_nnu});

  this->asDofs_u(dofval, dofval_u);

  return dofval_u;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs_u(
  const xt::xtensor<double,2>& nodevec) const
{
  xt::xtensor<double,1> dofval_u = xt::empty<double>({m_nnu});

  this->asDofs_u(nodevec, dofval_u);

  return dofval_u;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs_p(
  const xt::xtensor<double,1>& dofval) const
{
  xt::xtensor<double,1> dofval_p = xt::empty<double>({m_nnp});

  this->asDofs_p(dofval, dofval_p);

  return dofval_p;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs_p(
  const xt::xtensor<double,2>& nodevec) const
{
  xt::xtensor<double,1> dofval_p = xt::empty<double>({m_nnp});

  this->asDofs_p(nodevec, dofval_p);

  return dofval_p;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,1> dofval = xt::empty<double>({m_ndof});

  this->asDofs(elemvec, dofval);

  return dofval;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs_u(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,1> dofval_u = xt::empty<double>({m_nnu});

  this->asDofs_u(elemvec, dofval_u);

  return dofval_u;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AsDofs_p(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,1> dofval_p = xt::empty<double>({m_nnp});

  this->asDofs_p(elemvec, dofval_p);

  return dofval_p;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,2> VectorPartitioned::AsNode(
  const xt::xtensor<double,1>& dofval) const
{
  xt::xtensor<double,2> nodevec = xt::empty<double>({m_nnode, m_ndim});

  this->asNode(dofval, nodevec);

  return nodevec;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,2> VectorPartitioned::AsNode(
  const xt::xtensor<double,1>& dofval_u,
  const xt::xtensor<double,1>& dofval_p) const
{
  xt::xtensor<double,2> nodevec = xt::empty<double>({m_nnode, m_ndim});

  this->asNode(dofval_u, dofval_p, nodevec);

  return nodevec;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,2> VectorPartitioned::AsNode(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,2> nodevec = xt::empty<double>({m_nnode, m_ndim});

  this->asNode(elemvec, nodevec);

  return nodevec;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,3> VectorPartitioned::AsElement(
  const xt::xtensor<double,1>& dofval) const
{
  xt::xtensor<double,3> elemvec = xt::empty<double>({m_nelem, m_nne, m_ndim});

  this->asElement(dofval, elemvec);

  return elemvec;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,3> VectorPartitioned::AsElement(
  const xt::xtensor<double,1>& dofval_u,
  const xt::xtensor<double,1>& dofval_p) const
{
  xt::xtensor<double,3> elemvec = xt::empty<double>({m_nelem, m_nne, m_ndim});

  this->asElement(dofval_u, dofval_p, elemvec);

  return elemvec;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,3> VectorPartitioned::AsElement(
  const xt::xtensor<double,2>& nodevec) const
{
  xt::xtensor<double,3> elemvec = xt::empty<double>({m_nelem, m_nne, m_ndim});

  this->asElement(nodevec, elemvec);

  return elemvec;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AssembleDofs(
  const xt::xtensor<double,2>& nodevec) const
{
  xt::xtensor<double,1> dofval = xt::empty<double>({m_ndof});

  this->assembleDofs(nodevec, dofval);

  return dofval;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AssembleDofs_u(
  const xt::xtensor<double,2>& nodevec) const
{
  xt::xtensor<double,1> dofval_u = xt::empty<double>({m_nnu});

  this->assembleDofs_u(nodevec, dofval_u);

  return dofval_u;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AssembleDofs_p(
  const xt::xtensor<double,2>& nodevec) const
{
  xt::xtensor<double,1> dofval_p = xt::empty<double>({m_nnp});

  this->assembleDofs_p(nodevec, dofval_p);

  return dofval_p;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AssembleDofs(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,1> dofval = xt::empty<double>({m_ndof});

  this->assembleDofs(elemvec, dofval);

  return dofval;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AssembleDofs_u(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,1> dofval_u = xt::empty<double>({m_nnu});

  this->assembleDofs_u(elemvec, dofval_u);

  return dofval_u;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,1> VectorPartitioned::AssembleDofs_p(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,1> dofval_p = xt::empty<double>({m_nnp});

  this->assembleDofs_p(elemvec, dofval_p);

  return dofval_p;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<double,2> VectorPartitioned::AssembleNode(
  const xt::xtensor<double,3>& elemvec) const
{
  xt::xtensor<double,2> nodevec = xt::empty<double>({m_nnode, m_ndim});

  this->assembleNode(elemvec, nodevec);

  return nodevec;
}

// -------------------------------------------------------------------------------------------------

} // namespace ...

// =================================================================================================

#endif
