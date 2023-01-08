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

#include "dsl.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using namespace athena2;

TEST_CASE("Root case use of EvalContext", "[evalcontext]") {
  EvalContext ctx("root");
  REQUIRE_THROWS_WITH(ctx.error("test"), "Error: root: test");
}

TEST_CASE("Scoped case of EvalContext", "[evalcontext]") {
  EvalContext ctx("root");
  {
    auto _ = ctx.push("child");
    REQUIRE_THROWS_WITH(ctx.error("test1"), "Error: root > child: test1");
  }
  REQUIRE_THROWS_WITH(ctx.error("test2"), "Error: root: test2");
}

TEST_CASE("Deeply scoped case of EvalContext", "[evalcontext]") {
  EvalContext ctx("root");
  {
    auto _ = ctx.push("child1");
    {
      auto _ = ctx.push("child2");
      REQUIRE_THROWS_WITH(ctx.error("test1"),
                          "Error: root > child1 > child2: test1");
    }
    REQUIRE_THROWS_WITH(ctx.error("test2"), "Error: root > child1: test2");
  }
  REQUIRE_THROWS_WITH(ctx.error("test3"), "Error: root: test3");
}
