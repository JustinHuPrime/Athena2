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

#include "util/string.h"

#include <catch2/catch_test_macros.hpp>
#include <fstream>

using namespace athena2::util;
using namespace std;

TEST_CASE("Split of empty string", "[util][string]") {
  REQUIRE(split("", ',').empty());
}

TEST_CASE("Split of one token", "[util][string]") {
  REQUIRE(split("a", ',') == vector<string>{"a"});
}

TEST_CASE("Split of many tokens", "[util][string]") {
  REQUIRE(split("a b c", ' ') == vector<string>{"a", "b", "c"});
}

TEST_CASE("Split with empty token", "[util][string]") {
  REQUIRE(split("a  c", ' ') == vector<string>{"a", "", "c"});
}

TEST_CASE("Split with empty token at end of string", "[util][string]") {
  REQUIRE(split("a b ", ' ') == vector<string>{"a", "b"});
}

TEST_CASE("Stream to string", "[util][string]") {
  ifstream fin("data/hull/0_corvette");
  REQUIRE(fin);
  REQUIRE(fromStream(fin) == R"(hull Corvette
size 1
coreSize K
sectionSizes KC
hullHealth 200
armourHealth 0
evasion 0.6
speed 160
disengageChanceModifier 1
cost 0
)");
}
