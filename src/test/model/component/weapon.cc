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

#include "model/component/weapon.h"

#include "catch2/catch_test_macros.hpp"

using namespace athena2;
using namespace athena2::model::component;
using namespace std;

TEST_CASE("Weapon parsing", "[model][component][weapon]") {
  EvalContext ctx("root");
  Weapon weapon = Weapon::fromJson(R"({
  "name": "Small Red Laser",
  "size": "S",
  "tag": "energy",
  "power": -5,
  "minDamage": 6,
  "maxDamage": 16,
  "cooldown": 4.25,
  "accuracy": 0.9,
  "tracking": 0.5,
  "minRange": 0,
  "maxRange": 40,
  "shieldDamageModifier": -0.5,
  "armourDamageModifier": 0.5,
  "hullDamageModifier": 0.25,
  "cost": {
    "alloys": 10
  }
})"_json,
                                   ctx);
  REQUIRE(weapon.name == "Small Red Laser");
  REQUIRE(weapon.size == "S");
  REQUIRE(weapon.tag == "energy");
  REQUIRE(weapon.power == -5.f);
  REQUIRE(weapon.minDamage == 6.f);
  REQUIRE(weapon.maxDamage == 16);
  REQUIRE(weapon.cooldown == 4.25f);
  REQUIRE(weapon.accuracy == 0.9f);
  REQUIRE(weapon.tracking == 0.5f);
  REQUIRE(weapon.minRange == 0.f);
  REQUIRE(weapon.maxRange == 40.f);
  REQUIRE(weapon.shieldDamageModifier == -0.5f);
  REQUIRE(weapon.shieldSkipModifier == 0.f);
  REQUIRE(weapon.armourDamageModifier == 0.5f);
  REQUIRE(weapon.armourSkipModifier == 0.f);
  REQUIRE(weapon.hullDamageModifier == 0.25f);
  REQUIRE(weapon.cost == 20.f);
}
