// Copyright 2023 Justin Hu
//
// This file is part of Athena II.
//
// Athena II is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// Athena II is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Athena II. If not, see <https://www.gnu.org/licenses/>.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "model/economy.h"

#include <catch2/catch_test_macros.hpp>

using namespace athena2;
using namespace athena2::model;
using namespace std;

TEST_CASE("Parse zero cost", "[model][economy]") {
  EvalContext ctx("root");
  REQUIRE(Cost::fromLine("cost 0", ctx) == 0.0f);
}

TEST_CASE("Parse alloys cost", "[model][economy]") {
  EvalContext ctx("root");
  REQUIRE(Cost::fromLine("cost alloys 10", ctx) == 20.0f);
}

TEST_CASE("Parse minerals cost", "[model][economy]") {
  EvalContext ctx("root");
  REQUIRE(Cost::fromLine("cost minerals 10", ctx) == 10.0f);
}

TEST_CASE("Parse compound cost", "[model][economy]") {
  EvalContext ctx("root");
  REQUIRE(Cost::fromLine("cost minerals 5 alloys 10", ctx) == 25.0f);
}
