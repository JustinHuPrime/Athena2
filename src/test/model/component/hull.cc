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

#include "model/component/hull.h"

#include "catch2/catch_test_macros.hpp"

using namespace athena2;
using namespace athena2::model::component;
using namespace std;

TEST_CASE("Hull parsing", "[model][component][hull]") {
  EvalContext ctx("root");
  Hull corvette = Hull::fromJson(R"({
  "name": "Corvette",
  "size": 1,
  "coreSize": "KK",
  "sectionSizes": ["KC"],
  "hullHealth": 200,
  "evasion": 0.6,
  "speed": 160,
  "disengageChanceModifier": 0
}
)"_json,
                                 ctx);
  REQUIRE(corvette.name == "Corvette");
  REQUIRE(corvette.size == 1.f);
  REQUIRE(corvette.coreSize == "KK");
  REQUIRE(corvette.sectionSizes == vector<string>{"KC"});
  REQUIRE(corvette.hullHealth == 200.f);
  REQUIRE(corvette.hullHealthModifier == 0.f);
  REQUIRE(corvette.armourHealth == 0.f);
  REQUIRE(corvette.evasion == 0.6f);
  REQUIRE(corvette.evasionModifier == 0.f);
  REQUIRE(corvette.trackingModifier == 0.f);
  REQUIRE(corvette.speed == 160.f);
  REQUIRE(corvette.disengageChanceModifier == 0.f);
  REQUIRE(corvette.includeComponentCost == true);
  REQUIRE(corvette.cost == 0.f);
}

TEST_CASE("Hull parsing with all fields", "[model][component][hull]") {
  EvalContext ctx("root");
  Hull corvette = Hull::fromJson(R"({
  "name": "Corvette",
  "size": 1,
  "coreSize": "KK",
  "sectionSizes": ["KC"],
  "hullHealth": 200,
  "hullHealthModifier": 0,
  "armourHealth": 0,
  "evasion": 0.6,
  "evasionModifier": 0,
  "trackingModifier": 0,
  "speed": 160,
  "disengageChanceModifier": 0,
  "includeComponentCost": true,
  "cost": {}
}
)"_json,
                                 ctx);
  REQUIRE(corvette.name == "Corvette");
  REQUIRE(corvette.size == 1.f);
  REQUIRE(corvette.coreSize == "KK");
  REQUIRE(corvette.sectionSizes == vector<string>{"KC"});
  REQUIRE(corvette.hullHealth == 200.f);
  REQUIRE(corvette.hullHealthModifier == 0.f);
  REQUIRE(corvette.armourHealth == 0.f);
  REQUIRE(corvette.evasion == 0.6f);
  REQUIRE(corvette.evasionModifier == 0.f);
  REQUIRE(corvette.trackingModifier == 0.f);
  REQUIRE(corvette.speed == 160.f);
  REQUIRE(corvette.disengageChanceModifier == 0.f);
  REQUIRE(corvette.includeComponentCost == true);
  REQUIRE(corvette.cost == 0.f);
}
