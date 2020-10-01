/*

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/GooseFEM

*/

#ifndef GOOSEFEM_MATRIX_H
#define GOOSEFEM_MATRIX_H

#include "config.h"

#include <Eigen/Eigen>
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>

namespace GooseFEM {

template <class Solver = Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>>>
class Matrix {
public:
    // Constructors
    Matrix() = default;
    Matrix(const xt::xtensor<size_t, 2>& conn, const xt::xtensor<size_t, 2>& dofs);

    // Dimensions
    size_t nelem() const; // number of elements
    size_t nne() const;   // number of nodes per element
    size_t nnode() const; // number of nodes
    size_t ndim() const;  // number of dimensions
    size_t ndof() const;  // number of DOFs

    // DOF lists
    xt::xtensor<size_t, 2> dofs() const; // DOFs

    // Assemble from matrices stored per element [nelem, nne*ndim, nne*ndim]
    void assemble(const xt::xtensor<double, 3>& elemmat);

    // Dot-product:
    // b_i = A_ij * x_j
    void dot(const xt::xtensor<double, 2>& x, xt::xtensor<double, 2>& b) const;
    void dot(const xt::xtensor<double, 1>& x, xt::xtensor<double, 1>& b) const;

    // Solve
    // x_u = A_uu \ ( b_u - A_up * x_p )
    void solve(const xt::xtensor<double, 2>& b, xt::xtensor<double, 2>& x);
    void solve(const xt::xtensor<double, 1>& b, xt::xtensor<double, 1>& x);

    // Auto-allocation of the functions above
    xt::xtensor<double, 2> Dot(const xt::xtensor<double, 2>& x) const;
    xt::xtensor<double, 1> Dot(const xt::xtensor<double, 1>& x) const;
    xt::xtensor<double, 2> Solve(const xt::xtensor<double, 2>& b);
    xt::xtensor<double, 1> Solve(const xt::xtensor<double, 1>& b);

private:
    // The matrix
    Eigen::SparseMatrix<double> m_A;

    // Matrix entries
    std::vector<Eigen::Triplet<double>> m_T;

    // Solver (re-used to solve different RHS)
    Solver m_solver;

    // Signal changes to data compare to the last inverse
    bool m_factor = false;

    // Bookkeeping
    xt::xtensor<size_t, 2> m_conn; // connectivity [nelem, nne]
    xt::xtensor<size_t, 2> m_dofs; // DOF-numbers per node [nnode, ndim]

    // Dimensions
    size_t m_nelem; // number of elements
    size_t m_nne;   // number of nodes per element
    size_t m_nnode; // number of nodes
    size_t m_ndim;  // number of dimensions
    size_t m_ndof;  // number of DOFs

    // Compute inverse (automatically evaluated by "solve")
    void factorize();

    // Convert arrays (Eigen version of Vector, which contains public functions)
    Eigen::VectorXd asDofs(const xt::xtensor<double, 2>& nodevec) const;

    void asNode(const Eigen::VectorXd& dofval, xt::xtensor<double, 2>& nodevec) const;
};

} // namespace GooseFEM

#include "Matrix.hpp"

#endif
