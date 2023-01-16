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

#include "model/component/computer.h"

#include "catch2/catch_test_macros.hpp"

using namespace athena2;
using namespace athena2::model::component;
using namespace std;

TEST_CASE("Computer parsing", "[model][component][computer]") {
  EvalContext ctx("root");
  Computer computer = Computer::fromJson(R"({
  "name": "Basic Combat Computer",
  "sizes": ["KK", "MK"],
  "tactics": "swarm",
  "power": -5,
  "cost": {}
}
)"_json,
                                         ctx);
  REQUIRE(computer.name == "Basic Combat Computer");
  REQUIRE(computer.sizes == vector<string>{"KK", "MK"});
  REQUIRE(computer.tactics == Computer::Tactics::SWARM);
  REQUIRE(computer.power == -5.f);
  REQUIRE(computer.fireRateModifier == 0.f);
  REQUIRE(computer.evasionModifier == 0.f);
  REQUIRE(computer.sublightSpeedModifier == 0.f);
  REQUIRE(computer.explosiveWeaponsDamageModifier == 0.f);
  REQUIRE(computer.trackingBonus == 0.f);
  REQUIRE(computer.chanceToHitBonus == 0.f);
  REQUIRE(computer.weaponsRangeModifier == 0.f);
  REQUIRE(computer.engagementRangeModifier == 0.f);
  REQUIRE(computer.cost == 0.f);
}

TEST_CASE("Computer parsing with all fields", "[model][component][computer]") {
  EvalContext ctx("root");
  Computer computer = Computer::fromJson(R"({
  "name": "Basic Combat Computer",
  "sizes": ["KK", "MK"],
  "tactics": "swarm",
  "power": -5,
  "fireRateModifier": 1,
  "evasionModifier": 1,
  "sublightSpeedModifier": 1,
  "explosiveWeaponsDamageModifier": 1,
  "trackingBonus": 0,
  "chanceToHitBonus": 0,
  "weaponsRangeModifier": 1,
  "engagementRangeModifier": 1,
  "cost": {}
  }
)"_json,
                                         ctx);
  REQUIRE(computer.name == "Basic Combat Computer");
  REQUIRE(computer.sizes == vector<string>{"KK", "MK"});
  REQUIRE(computer.tactics == Computer::Tactics::SWARM);
  REQUIRE(computer.power == -5.f);
  REQUIRE(computer.fireRateModifier == 1.f);
  REQUIRE(computer.evasionModifier == 1.f);
  REQUIRE(computer.sublightSpeedModifier == 1.f);
  REQUIRE(computer.explosiveWeaponsDamageModifier == 1.f);
  REQUIRE(computer.trackingBonus == 0.f);
  REQUIRE(computer.chanceToHitBonus == 0.f);
  REQUIRE(computer.weaponsRangeModifier == 1.f);
  REQUIRE(computer.engagementRangeModifier == 1.f);
  REQUIRE(computer.cost == 0.f);
}
