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

#include "model/component/utility.h"

#include "catch2/catch_test_macros.hpp"

using namespace athena2;
using namespace athena2::model::component;
using namespace std;

TEST_CASE("Utility parsing", "[model][component][utility]") {
  EvalContext ctx("root");
  Utility utility = Utility::fromJson(R"({
  "name": "Small Deflectors",
  "size": "S",
  "power": -15,
  "shieldHealth": 75,
  "shieldRegen": 0.5,
  "cost": {"alloys": 5}
})"_json,
                                      ctx);
  REQUIRE(utility.name == "Small Deflectors");
  REQUIRE(utility.size == "S");
  REQUIRE(utility.power == -15.f);
  REQUIRE(utility.shieldHealth == 75.f);
  REQUIRE(utility.shieldRegen == 0.5f);
  REQUIRE(utility.cost == 10.f);
}
