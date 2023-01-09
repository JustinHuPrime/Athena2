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

#include "model/component/sublight.h"

#include "catch2/catch_test_macros.hpp"

using namespace athena2;
using namespace athena2::model::component;
using namespace std;

TEST_CASE("Sublight parsing", "[model][component][sublight]") {
  EvalContext ctx("root");
  Sublight sublight = Sublight::fromJson(R"({
  "name": "Chemical Thrusters",
  "sizes": ["KK", "FF", "MK"],
  "power": -10,
  "speedModifier": 0,
  "evasionBonus": 0,
  "cost": {
    "alloys": 3
  }
})"_json,
                                         ctx);
  REQUIRE(sublight.name == "Chemical Thrusters");
  REQUIRE(sublight.sizes == vector<string>{"KK", "FF", "MK"});
  REQUIRE(sublight.power == -10.f);
  REQUIRE(sublight.speedModifier == 0.f);
  REQUIRE(sublight.evasionBonus == 0.f);
  REQUIRE(sublight.cost == 6.f);
}
