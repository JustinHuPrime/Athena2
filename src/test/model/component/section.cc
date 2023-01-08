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

#include "model/component/section.h"

#include "catch2/catch_test_macros.hpp"

using namespace athena2;
using namespace athena2::model::component;
using namespace std;

TEST_CASE("Section parsing", "[model][component][section]") {
  EvalContext ctx("root");
  Section interceptor = Section::fromJson(R"({
  "name": "Interceptor",
  "size": "KC",
  "weaponSlots": "SSS",
  "utilitySlots": "SSSA"
}
)"_json,
                                          ctx);
  REQUIRE(interceptor.name == "Interceptor");
  REQUIRE(interceptor.size == "KC");
  REQUIRE(interceptor.weaponSlots == "SSS");
  REQUIRE(interceptor.utilitySlots == "ASSS");
  REQUIRE(interceptor.cost == 0.f);
}

TEST_CASE("Section parsing with all fields", "[model][component][section]") {
  EvalContext ctx("root");
  Section interceptor = Section::fromJson(R"({
  "name": "Interceptor",
  "size": "KC",
  "weaponSlots": "SSS",
  "utilitySlots": "SSSA",
  "cost": {"alloys": 30}
}
)"_json,
                                          ctx);
  REQUIRE(interceptor.name == "Interceptor");
  REQUIRE(interceptor.size == "KC");
  REQUIRE(interceptor.weaponSlots == "SSS");
  REQUIRE(interceptor.utilitySlots == "ASSS");
  REQUIRE(interceptor.cost == 60.f);
}
