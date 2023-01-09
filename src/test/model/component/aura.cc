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

#include "model/component/aura.h"

#include "catch2/catch_test_macros.hpp"

using namespace athena2;
using namespace athena2::model::component;
using namespace std;

TEST_CASE("Aura parsing", "[model][component][aura]") {
  EvalContext ctx("root");
  Aura aura = Aura::fromJson(R"({
  "name": "Quantum Destabilizer",
  "size": "TT",
  "range": 80,
  "enemyFireRateModifier": -0.1
})"_json,
                             ctx);
  REQUIRE(aura.name == "Quantum Destabilizer");
  REQUIRE(aura.size == "TT");
  REQUIRE(aura.enemyFireRateModifier == -0.1f);
  REQUIRE(aura.enemyShieldHealthModifier == 0.f);
  REQUIRE(aura.enemyDisengageChanceModifier == 0.f);
  REQUIRE(aura.allyFireRateModifier == 0.f);
  REQUIRE(aura.allyHullRegenModifier == 0.f);
  REQUIRE(aura.allyArmourRegenModifier == 0.f);
  REQUIRE(aura.allyTrackingBonus == 0.f);
  REQUIRE(aura.enemyPointDefenceDamageModifier == 0.f);
  REQUIRE(aura.enemyPointDefenceFireRateModifier == 0.f);
  REQUIRE(aura.allyStrikeCraftDamageModifier == 0.f);
  REQUIRE(aura.allyStrikeCraftSpeedModifier == 0.f);
  REQUIRE(aura.allyWeaponsRangeModifier == 0.f);
}
