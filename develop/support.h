
#ifndef SUPPORT_H
#define SUPPORT_H

#include <catch2/catch.hpp>
#include <cppmat/cppmat.h>
#include <Eigen/Eigen>

#include "../include/GooseFEM/GooseFEM.h"

#define EQ(a,b) REQUIRE_THAT( (a), Catch::WithinAbs((b), 1.e-12) );

#endif
