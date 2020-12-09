
#include <catch2/catch.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xmath.hpp>
#include <GooseFEM/GooseFEM.h>

#define ISCLOSE(a,b) REQUIRE_THAT((a), Catch::WithinAbs((b), 1.e-12));

TEST_CASE("GooseFEM::Mesh", "Mesh.h")
{
    SECTION("edgesize")
    {
        GooseFEM::Mesh::Quad4::Regular mesh(2, 2, 10.0);
        auto s = GooseFEM::Mesh::edgesize(mesh.coor(), mesh.conn());
        auto t = GooseFEM::Mesh::edgesize(mesh.coor(), mesh.conn(), mesh.getElementType());
        REQUIRE(xt::allclose(s, 10.0));
        REQUIRE(xt::allclose(t, 10.0));
    }

    SECTION("coordination")
    {
        GooseFEM::Mesh::Quad4::Regular mesh(3, 3);
        auto N = GooseFEM::Mesh::coordination(mesh.conn());
        xt::xtensor<size_t, 1> ret = {1, 2, 2, 1, 2, 4, 4, 2, 2, 4, 4, 2, 1, 2, 2, 1};
        REQUIRE(xt::all(xt::equal(N, ret)));
    }

    SECTION("elem2node")
    {
        GooseFEM::Mesh::Quad4::Regular mesh(3, 3);
        auto tonode = GooseFEM::Mesh::elem2node(mesh.conn());
        REQUIRE(tonode.size() == 16);
        REQUIRE(tonode[0] == std::vector<size_t>{0});
        REQUIRE(tonode[1] == std::vector<size_t>{0, 1});
        REQUIRE(tonode[2] == std::vector<size_t>{1, 2});
        REQUIRE(tonode[3] == std::vector<size_t>{2});
        REQUIRE(tonode[4] == std::vector<size_t>{0, 3});
        REQUIRE(tonode[5] == std::vector<size_t>{0, 1, 3, 4});
        REQUIRE(tonode[6] == std::vector<size_t>{1, 2, 4, 5});
        REQUIRE(tonode[7] == std::vector<size_t>{2, 5});
        REQUIRE(tonode[8] == std::vector<size_t>{3, 6});
        REQUIRE(tonode[9] == std::vector<size_t>{3, 4, 6, 7});
        REQUIRE(tonode[10] == std::vector<size_t>{4, 5, 7, 8});
        REQUIRE(tonode[11] == std::vector<size_t>{5, 8});
        REQUIRE(tonode[12] == std::vector<size_t>{6});
        REQUIRE(tonode[13] == std::vector<size_t>{6, 7});
        REQUIRE(tonode[14] == std::vector<size_t>{7, 8});
        REQUIRE(tonode[15] == std::vector<size_t>{8});
    }

    // SECTION("neighbours > 1 shared node")
    // {
    //     GooseFEM::Mesh::Quad4::Regular mesh(3, 3);
    //     auto neigh = GooseFEM::Mesh::neighbours(mesh.conn());
    //     REQUIRE(neigh.size() == 9);
    //     REQUIRE(neigh[0] == std::vector<size_t>{1, 3, 4});
    //     REQUIRE(neigh[1] == std::vector<size_t>{0, 2, 3, 4, 5});
    //     REQUIRE(neigh[2] == std::vector<size_t>{1, 4, 5});
    //     REQUIRE(neigh[3] == std::vector<size_t>{0, 1, 4, 6, 7});
    //     REQUIRE(neigh[4] == std::vector<size_t>{0, 1, 2, 3, 5, 6, 7, 8});
    //     REQUIRE(neigh[5] == std::vector<size_t>{1, 2, 4, 7, 8});
    //     REQUIRE(neigh[6] == std::vector<size_t>{3, 4, 7});
    //     REQUIRE(neigh[7] == std::vector<size_t>{3, 4, 5, 6, 8});
    //     REQUIRE(neigh[8] == std::vector<size_t>{4, 5, 7});
    // }

    // SECTION("neighbours > 1 shared edge")
    // {
    //     GooseFEM::Mesh::Quad4::Regular mesh(3, 3);
    //     auto neigh = GooseFEM::Mesh::neighbours(mesh.conn(), 1);
    //     REQUIRE(neigh.size() == 9);
    //     REQUIRE(neigh[0] == std::vector<size_t>{1, 3, 4});
    //     REQUIRE(neigh[1] == std::vector<size_t>{0, 2, 3, 4, 5});
    //     REQUIRE(neigh[2] == std::vector<size_t>{1, 4, 5});
    //     REQUIRE(neigh[3] == std::vector<size_t>{0, 1, 4, 6, 7});
    //     REQUIRE(neigh[4] == std::vector<size_t>{0, 1, 2, 3, 5, 6, 7, 8});
    //     REQUIRE(neigh[5] == std::vector<size_t>{1, 2, 4, 7, 8});
    //     REQUIRE(neigh[6] == std::vector<size_t>{3, 4, 7});
    //     REQUIRE(neigh[7] == std::vector<size_t>{3, 4, 5, 6, 8});
    //     REQUIRE(neigh[8] == std::vector<size_t>{4, 5, 7});
    // }
}
