/*

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/GooseFEM

*/

#ifndef GOOSEFEM_ELEMENTHEX8_HPP
#define GOOSEFEM_ELEMENTHEX8_HPP

#include "ElementHex8.h"

namespace GooseFEM {
namespace Element {
namespace Hex8 {

template <class T>
inline double inv(const T& A, T& Ainv)
{
    double det =
        (A(0, 0) * A(1, 1) * A(2, 2) + A(0, 1) * A(1, 2) * A(2, 0) + A(0, 2) * A(1, 0) * A(2, 1)) -
        (A(0, 2) * A(1, 1) * A(2, 0) + A(0, 1) * A(1, 0) * A(2, 2) + A(0, 0) * A(1, 2) * A(2, 1));

    Ainv(0, 0) = (A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1)) / det;
    Ainv(0, 1) = (A(0, 2) * A(2, 1) - A(0, 1) * A(2, 2)) / det;
    Ainv(0, 2) = (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)) / det;

    Ainv(1, 0) = (A(1, 2) * A(2, 0) - A(1, 0) * A(2, 2)) / det;
    Ainv(1, 1) = (A(0, 0) * A(2, 2) - A(0, 2) * A(2, 0)) / det;
    Ainv(1, 2) = (A(0, 2) * A(1, 0) - A(0, 0) * A(1, 2)) / det;

    Ainv(2, 0) = (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0)) / det;
    Ainv(2, 1) = (A(0, 1) * A(2, 0) - A(0, 0) * A(2, 1)) / det;
    Ainv(2, 2) = (A(0, 0) * A(1, 1) - A(0, 1) * A(1, 0)) / det;

    return det;
}

namespace Gauss {

inline size_t nip()
{
    return 8;
}

inline xt::xtensor<double, 2> xi()
{
    size_t nip = 8;
    size_t ndim = 3;

    xt::xtensor<double, 2> xi = xt::empty<double>({nip, ndim});

    xi(0, 0) = -1.0 / std::sqrt(3.0);
    xi(0, 1) = -1.0 / std::sqrt(3.0);
    xi(0, 2) = -1.0 / std::sqrt(3.0);

    xi(1, 0) = +1.0 / std::sqrt(3.0);
    xi(1, 1) = -1.0 / std::sqrt(3.0);
    xi(1, 2) = -1.0 / std::sqrt(3.0);

    xi(2, 0) = +1.0 / std::sqrt(3.0);
    xi(2, 1) = +1.0 / std::sqrt(3.0);
    xi(2, 2) = -1.0 / std::sqrt(3.0);

    xi(3, 0) = -1.0 / std::sqrt(3.0);
    xi(3, 1) = +1.0 / std::sqrt(3.0);
    xi(3, 2) = -1.0 / std::sqrt(3.0);

    xi(4, 0) = -1.0 / std::sqrt(3.0);
    xi(4, 1) = -1.0 / std::sqrt(3.0);
    xi(4, 2) = +1.0 / std::sqrt(3.0);

    xi(5, 0) = +1.0 / std::sqrt(3.0);
    xi(5, 1) = -1.0 / std::sqrt(3.0);
    xi(5, 2) = +1.0 / std::sqrt(3.0);

    xi(6, 0) = +1.0 / std::sqrt(3.0);
    xi(6, 1) = +1.0 / std::sqrt(3.0);
    xi(6, 2) = +1.0 / std::sqrt(3.0);

    xi(7, 0) = -1.0 / std::sqrt(3.0);
    xi(7, 1) = +1.0 / std::sqrt(3.0);
    xi(7, 2) = +1.0 / std::sqrt(3.0);

    return xi;
}

inline xt::xtensor<double, 1> w()
{
    size_t nip = 8;

    xt::xtensor<double, 1> w = xt::empty<double>({nip});

    w(0) = 1.0;
    w(1) = 1.0;
    w(2) = 1.0;
    w(3) = 1.0;
    w(4) = 1.0;
    w(5) = 1.0;
    w(6) = 1.0;
    w(7) = 1.0;

    return w;
}

} // namespace Gauss

namespace Nodal {

inline size_t nip()
{
    return 8;
}

inline xt::xtensor<double, 2> xi()
{
    size_t nip = 8;
    size_t ndim = 3;

    xt::xtensor<double, 2> xi = xt::empty<double>({nip, ndim});

    xi(0, 0) = -1.0;
    xi(0, 1) = -1.0;
    xi(0, 2) = -1.0;

    xi(1, 0) = +1.0;
    xi(1, 1) = -1.0;
    xi(1, 2) = -1.0;

    xi(2, 0) = +1.0;
    xi(2, 1) = +1.0;
    xi(2, 2) = -1.0;

    xi(3, 0) = -1.0;
    xi(3, 1) = +1.0;
    xi(3, 2) = -1.0;

    xi(4, 0) = -1.0;
    xi(4, 1) = -1.0;
    xi(4, 2) = +1.0;

    xi(5, 0) = +1.0;
    xi(5, 1) = -1.0;
    xi(5, 2) = +1.0;

    xi(6, 0) = +1.0;
    xi(6, 1) = +1.0;
    xi(6, 2) = +1.0;

    xi(7, 0) = -1.0;
    xi(7, 1) = +1.0;
    xi(7, 2) = +1.0;

    return xi;
}

inline xt::xtensor<double, 1> w()
{
    size_t nip = 8;

    xt::xtensor<double, 1> w = xt::empty<double>({nip});

    w(0) = 1.0;
    w(1) = 1.0;
    w(2) = 1.0;
    w(3) = 1.0;
    w(4) = 1.0;
    w(5) = 1.0;
    w(6) = 1.0;
    w(7) = 1.0;

    return w;
}

} // namespace Nodal

inline Quadrature::Quadrature(const xt::xtensor<double, 3>& x)
    : Quadrature(x, Gauss::xi(), Gauss::w())
{
}

inline Quadrature::Quadrature(
    const xt::xtensor<double, 3>& x,
    const xt::xtensor<double, 2>& xi,
    const xt::xtensor<double, 1>& w)
    : m_x(x), m_w(w), m_xi(xi)
{
    GOOSEFEM_ASSERT(m_x.shape(1) == m_nne);
    GOOSEFEM_ASSERT(m_x.shape(2) == m_ndim);

    m_nelem = m_x.shape(0);
    m_nip = m_w.size();

    GOOSEFEM_ASSERT(m_xi.shape(0) == m_nip);
    GOOSEFEM_ASSERT(m_xi.shape(1) == m_ndim);
    GOOSEFEM_ASSERT(m_w.size() == m_nip);

    m_N = xt::empty<double>({m_nip, m_nne});
    m_dNxi = xt::empty<double>({m_nip, m_nne, m_ndim});
    m_dNx = xt::empty<double>({m_nelem, m_nip, m_nne, m_ndim});
    m_vol = xt::empty<double>({m_nelem, m_nip});

    // shape functions
    for (size_t q = 0; q < m_nip; ++q) {
        m_N(q, 0) = 0.125 * (1.0 - m_xi(q, 0)) * (1.0 - m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_N(q, 1) = 0.125 * (1.0 + m_xi(q, 0)) * (1.0 - m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_N(q, 2) = 0.125 * (1.0 + m_xi(q, 0)) * (1.0 + m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_N(q, 3) = 0.125 * (1.0 - m_xi(q, 0)) * (1.0 + m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_N(q, 4) = 0.125 * (1.0 - m_xi(q, 0)) * (1.0 - m_xi(q, 1)) * (1.0 + m_xi(q, 2));
        m_N(q, 5) = 0.125 * (1.0 + m_xi(q, 0)) * (1.0 - m_xi(q, 1)) * (1.0 + m_xi(q, 2));
        m_N(q, 6) = 0.125 * (1.0 + m_xi(q, 0)) * (1.0 + m_xi(q, 1)) * (1.0 + m_xi(q, 2));
        m_N(q, 7) = 0.125 * (1.0 - m_xi(q, 0)) * (1.0 + m_xi(q, 1)) * (1.0 + m_xi(q, 2));
    }

    // shape function gradients in local coordinates
    for (size_t q = 0; q < m_nip; ++q) {
        // - dN / dxi_0
        m_dNxi(q, 0, 0) = -0.125 * (1.0 - m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 1, 0) = +0.125 * (1.0 - m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 2, 0) = +0.125 * (1.0 + m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 3, 0) = -0.125 * (1.0 + m_xi(q, 1)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 4, 0) = -0.125 * (1.0 - m_xi(q, 1)) * (1.0 + m_xi(q, 2));
        m_dNxi(q, 5, 0) = +0.125 * (1.0 - m_xi(q, 1)) * (1.0 + m_xi(q, 2));
        m_dNxi(q, 6, 0) = +0.125 * (1.0 + m_xi(q, 1)) * (1.0 + m_xi(q, 2));
        m_dNxi(q, 7, 0) = -0.125 * (1.0 + m_xi(q, 1)) * (1.0 + m_xi(q, 2));
        // - dN / dxi_1
        m_dNxi(q, 0, 1) = -0.125 * (1.0 - m_xi(q, 0)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 1, 1) = -0.125 * (1.0 + m_xi(q, 0)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 2, 1) = +0.125 * (1.0 + m_xi(q, 0)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 3, 1) = +0.125 * (1.0 - m_xi(q, 0)) * (1.0 - m_xi(q, 2));
        m_dNxi(q, 4, 1) = -0.125 * (1.0 - m_xi(q, 0)) * (1.0 + m_xi(q, 2));
        m_dNxi(q, 5, 1) = -0.125 * (1.0 + m_xi(q, 0)) * (1.0 + m_xi(q, 2));
        m_dNxi(q, 6, 1) = +0.125 * (1.0 + m_xi(q, 0)) * (1.0 + m_xi(q, 2));
        m_dNxi(q, 7, 1) = +0.125 * (1.0 - m_xi(q, 0)) * (1.0 + m_xi(q, 2));
        // - dN / dxi_2
        m_dNxi(q, 0, 2) = -0.125 * (1.0 - m_xi(q, 0)) * (1.0 - m_xi(q, 1));
        m_dNxi(q, 1, 2) = -0.125 * (1.0 + m_xi(q, 0)) * (1.0 - m_xi(q, 1));
        m_dNxi(q, 2, 2) = -0.125 * (1.0 + m_xi(q, 0)) * (1.0 + m_xi(q, 1));
        m_dNxi(q, 3, 2) = -0.125 * (1.0 - m_xi(q, 0)) * (1.0 + m_xi(q, 1));
        m_dNxi(q, 4, 2) = +0.125 * (1.0 - m_xi(q, 0)) * (1.0 - m_xi(q, 1));
        m_dNxi(q, 5, 2) = +0.125 * (1.0 + m_xi(q, 0)) * (1.0 - m_xi(q, 1));
        m_dNxi(q, 6, 2) = +0.125 * (1.0 + m_xi(q, 0)) * (1.0 + m_xi(q, 1));
        m_dNxi(q, 7, 2) = +0.125 * (1.0 - m_xi(q, 0)) * (1.0 + m_xi(q, 1));
    }

    // compute the shape function gradients, based on "x"
    compute_dN();
}

inline size_t Quadrature::nelem() const
{
    return m_nelem;
}

inline size_t Quadrature::nne() const
{
    return m_nne;
}

inline size_t Quadrature::ndim() const
{
    return m_ndim;
}

inline size_t Quadrature::nip() const
{
    return m_nip;
}

inline xt::xtensor<double, 4> Quadrature::GradN() const
{
    return m_dNx;
}

template <size_t rank>
inline void
Quadrature::asTensor(const xt::xtensor<double, 2>& arg, xt::xtensor<double, 2 + rank>& ret) const
{
    GOOSEFEM_ASSERT(xt::has_shape(arg, {m_nelem, m_nne}));
    GooseFEM::asTensor<2, rank>(arg, ret);
}

inline xt::xtensor<double, 2> Quadrature::dV() const
{
    return m_vol;
}

inline void Quadrature::update_x(const xt::xtensor<double, 3>& x)
{
    GOOSEFEM_ASSERT(x.shape() == m_x.shape());
    xt::noalias(m_x) = x;
    compute_dN();
}

inline void Quadrature::compute_dN()
{
    #pragma omp parallel
    {
        xt::xtensor<double, 2> J = xt::empty<double>({3, 3});
        xt::xtensor<double, 2> Jinv = xt::empty<double>({3, 3});

        #pragma omp for
        for (size_t e = 0; e < m_nelem; ++e) {

            auto x = xt::adapt(&m_x(e, 0, 0), xt::xshape<m_nne, m_ndim>());

            for (size_t q = 0; q < m_nip; ++q) {

                auto dNxi = xt::adapt(&m_dNxi(q, 0, 0), xt::xshape<m_nne, m_ndim>());
                auto dNx = xt::adapt(&m_dNx(e, q, 0, 0), xt::xshape<m_nne, m_ndim>());

                J.fill(0.0);

                for (size_t m = 0; m < m_nne; ++m) {
                    for (size_t i = 0; i < m_ndim; ++i) {
                        for (size_t j = 0; j < m_ndim; ++j) {
                            J(i, j) += dNxi(m, i) * x(m, j);
                        }
                    }
                }

                double Jdet = inv(J, Jinv);

                // dNx(m,i) += Jinv(i,j) * dNxi(m,j);
                for (size_t m = 0; m < m_nne; ++m) {
                    dNx(m, 0) =
                        Jinv(0, 0) * dNxi(m, 0) + Jinv(0, 1) * dNxi(m, 1) + Jinv(0, 2) * dNxi(m, 2);
                    dNx(m, 1) =
                        Jinv(1, 0) * dNxi(m, 0) + Jinv(1, 1) * dNxi(m, 1) + Jinv(1, 2) * dNxi(m, 2);
                    dNx(m, 2) =
                        Jinv(2, 0) * dNxi(m, 0) + Jinv(2, 1) * dNxi(m, 1) + Jinv(2, 2) * dNxi(m, 2);
                }

                m_vol(e, q) = m_w(q) * Jdet;
            }
        }
    }
}

inline void Quadrature::gradN_vector(
    const xt::xtensor<double, 3>& elemvec, xt::xtensor<double, 4>& qtensor) const
{
    GOOSEFEM_ASSERT(xt::has_shape(elemvec, {m_nelem, m_nne, m_ndim}));
    GOOSEFEM_ASSERT(xt::has_shape(qtensor, {m_nelem, m_nip, m_ndim, m_ndim}));

    qtensor.fill(0.0);

    #pragma omp parallel for
    for (size_t e = 0; e < m_nelem; ++e) {

        auto u = xt::adapt(&elemvec(e, 0, 0), xt::xshape<m_nne, m_ndim>());

        for (size_t q = 0; q < m_nip; ++q) {

            auto dNx = xt::adapt(&m_dNx(e, q, 0, 0), xt::xshape<m_nne, m_ndim>());
            auto gradu = xt::adapt(&qtensor(e, q, 0, 0), xt::xshape<m_ndim, m_ndim>());

            for (size_t m = 0; m < m_nne; ++m) {
                for (size_t i = 0; i < m_ndim; ++i) {
                    for (size_t j = 0; j < m_ndim; ++j) {
                        gradu(i, j) += dNx(m, i) * u(m, j);
                    }
                }
            }
        }
    }
}

inline void Quadrature::gradN_vector_T(
    const xt::xtensor<double, 3>& elemvec, xt::xtensor<double, 4>& qtensor) const
{
    GOOSEFEM_ASSERT(xt::has_shape(elemvec, {m_nelem, m_nne, m_ndim}));
    GOOSEFEM_ASSERT(xt::has_shape(qtensor, {m_nelem, m_nip, m_ndim, m_ndim}));

    qtensor.fill(0.0);

    #pragma omp parallel for
    for (size_t e = 0; e < m_nelem; ++e) {

        auto u = xt::adapt(&elemvec(e, 0, 0), xt::xshape<m_nne, m_ndim>());

        for (size_t q = 0; q < m_nip; ++q) {

            auto dNx = xt::adapt(&m_dNx(e, q, 0, 0), xt::xshape<m_nne, m_ndim>());
            auto gradu = xt::adapt(&qtensor(e, q, 0, 0), xt::xshape<m_ndim, m_ndim>());

            for (size_t m = 0; m < m_nne; ++m) {
                for (size_t i = 0; i < m_ndim; ++i) {
                    for (size_t j = 0; j < m_ndim; ++j) {
                        gradu(j, i) += dNx(m, i) * u(m, j);
                    }
                }
            }
        }
    }
}

inline void Quadrature::symGradN_vector(
    const xt::xtensor<double, 3>& elemvec, xt::xtensor<double, 4>& qtensor) const
{
    GOOSEFEM_ASSERT(xt::has_shape(elemvec, {m_nelem, m_nne, m_ndim}));
    GOOSEFEM_ASSERT(xt::has_shape(qtensor, {m_nelem, m_nip, m_ndim, m_ndim}));

    qtensor.fill(0.0);

    #pragma omp parallel for
    for (size_t e = 0; e < m_nelem; ++e) {

        auto u = xt::adapt(&elemvec(e, 0, 0), xt::xshape<m_nne, m_ndim>());

        for (size_t q = 0; q < m_nip; ++q) {

            auto dNx = xt::adapt(&m_dNx(e, q, 0, 0), xt::xshape<m_nne, m_ndim>());
            auto eps = xt::adapt(&qtensor(e, q, 0, 0), xt::xshape<m_ndim, m_ndim>());

            for (size_t m = 0; m < m_nne; ++m) {
                for (size_t i = 0; i < m_ndim; ++i) {
                    for (size_t j = 0; j < m_ndim; ++j) {
                        eps(i, j) += 0.5 * dNx(m, i) * u(m, j);
                        eps(j, i) += 0.5 * dNx(m, i) * u(m, j);
                    }
                }
            }
        }
    }
}

inline void Quadrature::int_N_scalar_NT_dV(
    const xt::xtensor<double, 2>& qscalar, xt::xtensor<double, 3>& elemmat) const
{
    GOOSEFEM_ASSERT(xt::has_shape(qscalar, {m_nelem, m_nip}));
    GOOSEFEM_ASSERT(xt::has_shape(elemmat, {m_nelem, m_nne * m_ndim, m_nne * m_ndim}));

    elemmat.fill(0.0);

    #pragma omp parallel for
    for (size_t e = 0; e < m_nelem; ++e) {

        auto M = xt::adapt(&elemmat(e, 0, 0), xt::xshape<m_nne * m_ndim, m_nne * m_ndim>());

        for (size_t q = 0; q < m_nip; ++q) {

            auto N = xt::adapt(&m_N(q, 0), xt::xshape<m_nne>());
            auto& vol = m_vol(e, q);
            auto& rho = qscalar(e, q);

            // M(m * ndim + i, n * ndim + i) += N(m) * scalar * N(n) * dV
            for (size_t m = 0; m < m_nne; ++m) {
                for (size_t n = 0; n < m_nne; ++n) {
                    M(m * m_ndim + 0, n * m_ndim + 0) += N(m) * rho * N(n) * vol;
                    M(m * m_ndim + 1, n * m_ndim + 1) += N(m) * rho * N(n) * vol;
                    M(m * m_ndim + 2, n * m_ndim + 2) += N(m) * rho * N(n) * vol;
                }
            }
        }
    }
}

inline void Quadrature::int_gradN_dot_tensor2_dV(
    const xt::xtensor<double, 4>& qtensor, xt::xtensor<double, 3>& elemvec) const
{
    GOOSEFEM_ASSERT(xt::has_shape(qtensor, {m_nelem, m_nip, m_ndim, m_ndim}));
    GOOSEFEM_ASSERT(xt::has_shape(elemvec, {m_nelem, m_nne, m_ndim}));

    elemvec.fill(0.0);

    #pragma omp parallel for
    for (size_t e = 0; e < m_nelem; ++e) {

        auto f = xt::adapt(&elemvec(e, 0, 0), xt::xshape<m_nne, m_ndim>());

        for (size_t q = 0; q < m_nip; ++q) {

            auto dNx = xt::adapt(&m_dNx(e, q, 0, 0), xt::xshape<m_nne, m_ndim>());
            auto sig = xt::adapt(&qtensor(e, q, 0, 0), xt::xshape<m_ndim, m_ndim>());
            auto& vol = m_vol(e, q);

            for (size_t m = 0; m < m_nne; ++m) {
                f(m, 0) +=
                    (dNx(m, 0) * sig(0, 0) + dNx(m, 1) * sig(1, 0) + dNx(m, 2) * sig(2, 0)) * vol;
                f(m, 1) +=
                    (dNx(m, 0) * sig(0, 1) + dNx(m, 1) * sig(1, 1) + dNx(m, 2) * sig(2, 1)) * vol;
                f(m, 2) +=
                    (dNx(m, 0) * sig(0, 2) + dNx(m, 1) * sig(1, 2) + dNx(m, 2) * sig(2, 2)) * vol;
            }
        }
    }
}

inline void Quadrature::int_gradN_dot_tensor4_dot_gradNT_dV(
    const xt::xtensor<double, 6>& qtensor, xt::xtensor<double, 3>& elemmat) const
{
    GOOSEFEM_ASSERT(xt::has_shape(qtensor, {m_nelem, m_nip, m_ndim, m_ndim, m_ndim, m_ndim}));
    GOOSEFEM_ASSERT(xt::has_shape(elemmat, {m_nelem, m_nne * m_ndim, m_nne * m_ndim}));

    elemmat.fill(0.0);

    #pragma omp parallel for
    for (size_t e = 0; e < m_nelem; ++e) {

        auto K = xt::adapt(&elemmat(e, 0, 0), xt::xshape<m_nne * m_ndim, m_nne * m_ndim>());

        for (size_t q = 0; q < m_nip; ++q) {

            auto dNx = xt::adapt(&m_dNx(e, q, 0, 0), xt::xshape<m_nne, m_ndim>());
            auto C = xt::adapt(&qtensor(e, q, 0, 0, 0, 0), xt::xshape<m_ndim, m_ndim, m_ndim, m_ndim>());
            auto& vol = m_vol(e, q);

            for (size_t m = 0; m < m_nne; ++m) {
                for (size_t n = 0; n < m_nne; ++n) {
                    for (size_t i = 0; i < m_ndim; ++i) {
                        for (size_t j = 0; j < m_ndim; ++j) {
                            for (size_t k = 0; k < m_ndim; ++k) {
                                for (size_t l = 0; l < m_ndim; ++l) {
                                    K(m * m_ndim + j, n * m_ndim + k) +=
                                        dNx(m, i) * C(i, j, k, l) * dNx(n, l) * vol;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

template <size_t rank>
inline xt::xtensor<double, 2 + rank>
Quadrature::AsTensor(const xt::xtensor<double, 2>& qscalar) const
{
    return GooseFEM::AsTensor<2, rank>(qscalar, m_ndim);
}

inline xt::xarray<double>
Quadrature::AsTensor(size_t rank, const xt::xtensor<double, 2>& qscalar) const
{
    return GooseFEM::AsTensor(rank, qscalar, m_ndim);
}

inline xt::xtensor<double, 4> Quadrature::GradN_vector(const xt::xtensor<double, 3>& elemvec) const
{
    xt::xtensor<double, 4> qtensor = xt::empty<double>({m_nelem, m_nip, m_ndim, m_ndim});
    this->gradN_vector(elemvec, qtensor);
    return qtensor;
}

inline xt::xtensor<double, 4>
Quadrature::GradN_vector_T(const xt::xtensor<double, 3>& elemvec) const
{
    xt::xtensor<double, 4> qtensor = xt::empty<double>({m_nelem, m_nip, m_ndim, m_ndim});
    this->gradN_vector_T(elemvec, qtensor);
    return qtensor;
}

inline xt::xtensor<double, 4>
Quadrature::SymGradN_vector(const xt::xtensor<double, 3>& elemvec) const
{
    xt::xtensor<double, 4> qtensor = xt::empty<double>({m_nelem, m_nip, m_ndim, m_ndim});
    this->symGradN_vector(elemvec, qtensor);
    return qtensor;
}

inline xt::xtensor<double, 3>
Quadrature::Int_N_scalar_NT_dV(const xt::xtensor<double, 2>& qscalar) const
{
    xt::xtensor<double, 3> elemmat = xt::empty<double>({m_nelem, m_nne * m_ndim, m_nne * m_ndim});
    this->int_N_scalar_NT_dV(qscalar, elemmat);
    return elemmat;
}

inline xt::xtensor<double, 3>
Quadrature::Int_gradN_dot_tensor2_dV(const xt::xtensor<double, 4>& qtensor) const
{
    xt::xtensor<double, 3> elemvec = xt::empty<double>({m_nelem, m_nne, m_ndim});
    this->int_gradN_dot_tensor2_dV(qtensor, elemvec);
    return elemvec;
}

inline xt::xtensor<double, 3>
Quadrature::Int_gradN_dot_tensor4_dot_gradNT_dV(const xt::xtensor<double, 6>& qtensor) const
{
    xt::xtensor<double, 3> elemmat = xt::empty<double>({m_nelem, m_ndim * m_nne, m_ndim * m_nne});
    this->int_gradN_dot_tensor4_dot_gradNT_dV(qtensor, elemmat);
    return elemmat;
}

template <size_t rank>
inline xt::xtensor<double, rank + 2> Quadrature::AllocateQtensor() const
{
    std::array<size_t, rank + 2> shape;
    shape[0] = m_nelem;
    shape[1] = m_nip;
    size_t n = m_ndim;
    std::fill(shape.begin() + 2, shape.end(), n);
    xt::xtensor<double, rank + 2> ret = xt::empty<double>(shape);
    return ret;
}

template <size_t rank>
inline xt::xtensor<double, rank + 2> Quadrature::AllocateQtensor(double val) const
{
    xt::xtensor<double, rank + 2> ret = this->AllocateQtensor<rank>();
    ret.fill(val);
    return ret;
}

inline xt::xarray<double> Quadrature::AllocateQtensor(size_t rank) const
{
    std::vector<size_t> shape(rank + 2);
    shape[0] = m_nelem;
    shape[1] = m_nip;
    size_t n = m_ndim;
    std::fill(shape.begin() + 2, shape.end(), n);
    xt::xarray<double> ret = xt::empty<double>(shape);
    return ret;
}

inline xt::xarray<double> Quadrature::AllocateQtensor(size_t rank, double val) const
{
    xt::xarray<double> ret = this->AllocateQtensor(rank);
    ret.fill(val);
    return ret;
}

inline xt::xtensor<double, 2> Quadrature::AllocateQscalar() const
{
    return this->AllocateQtensor<0>();
}

inline xt::xtensor<double, 2> Quadrature::AllocateQscalar(double val) const
{
    return this->AllocateQtensor<0>(val);
}

} // namespace Hex8
} // namespace Element
} // namespace GooseFEM

#endif
