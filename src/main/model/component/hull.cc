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

#include <vector>

#include "util/string.h"

using namespace std;
using namespace athena2::util;

namespace athena2::model::component {
Hull Hull::fromFile(string const &file, EvalContext &ctx) {
  vector<string> lines = split(file, '\n');

  auto curr = lines.begin();
  auto end = lines.end();

  // expect first line to be hull <name>
  if (curr == end) ctx.error("hull file is empty");
  vector<string> tokens = split(*curr, ' ');
  if (tokens.size() != 2) ctx.error("hull line is not 'hull <name>'");
  if (tokens[0] != "hull") ctx.error("hull line does not start with 'hull'");
  string name = tokens[1];

  // expect second line to be size <size>
  ++curr;
  if (curr == end) ctx.error("hull file has no size line");
  tokens = split(*curr, ' ');
  if (tokens.size() != 2) ctx.error("size line is not 'size <size>'");
  if (tokens[0] != "size") ctx.error("size line does not start with 'size'");
  char *endptr;
  float size = strtof(tokens[1].c_str(), &endptr);
  if (*endptr != '\0') ctx.error("size line has invalid size");

  // expect third line to be coreSize <coreSize>
  ++curr;
  if (curr == end) ctx.error("hull file has no coreSize line");
  tokens = split(*curr, ' ');
  if (tokens.size() != 2)
    ctx.error("coreSize line is not 'coreSize <coreSize>'");
  if (tokens[0] != "coreSize")
    ctx.error("coreSize line does not start with 'coreSize'");
  string coreSize = tokens[1];

  // expect fourth line to be sectionSizes <sectionSize...>
  ++curr;
  if (curr == end) ctx.error("hull file has no sectionSizes line");
  tokens = split(*curr, ' ');
  if (tokens.size() < 2)
    ctx.error("sectionSizes line is not 'sectionSizes <sectionSize...>'");
  if (tokens[0] != "sectionSizes")
    ctx.error("sectionSizes line does not start with 'sectionSizes'");
  vector<string> sectionSizes =
      vector<string>(tokens.begin() + 1, tokens.end());

  // expect fifth line to be hullHealth <hullHealth>
  ++curr;
  if (curr == end) ctx.error("hull file has no hullHealth line");
  tokens = split(*curr, ' ');
  if (tokens.size() != 2)
    ctx.error("hullHealth line is not 'hullHealth <hullHealth>'");
  if (tokens[0] != "hullHealth")
    ctx.error("hullHealth line does not start with 'hullHealth'");
  float hullHealth = strtof(tokens[1].c_str(), &endptr);
  if (*endptr != '\0') ctx.error("hullHealth line has invalid hullHealth");

  // expect sixth line to be armourHealth <armourHealth>
  ++curr;
  if (curr == end) ctx.error("hull file has no armourHealth line");
  tokens = split(*curr, ' ');
  if (tokens.size() != 2)
    ctx.error("armourHealth line is not 'armourHealth <armourHealth>'");
  if (tokens[0] != "armourHealth")
    ctx.error("armourHealth line does not start with 'armourHealth'");
  float armourHealth = strtof(tokens[1].c_str(), &endptr);
  if (*endptr != '\0') ctx.error("armourHealth line has invalid armourHealth");

  // expect seventh line to be evasion <evasion>
  ++curr;
  if (curr == end) ctx.error("hull file has no evasion line");
  tokens = split(*curr, ' ');
  if (tokens.size() != 2) ctx.error("evasion line is not 'evasion <evasion>'");
  if (tokens[0] != "evasion")
    ctx.error("evasion line does not start with 'evasion'");
  float evasion = strtof(tokens[1].c_str(), &endptr);
  if (*endptr != '\0') ctx.error("evasion line has invalid evasion");

  // expect eighth line to be speed <speed>
  ++curr;
  if (curr == end) ctx.error("hull file has no speed line");
  tokens = split(*curr, ' ');
  if (tokens.size() != 2) ctx.error("speed line is not 'speed <speed>'");
  if (tokens[0] != "speed") ctx.error("speed line does not start with 'speed'");
  float speed = strtof(tokens[1].c_str(), &endptr);
  if (*endptr != '\0') ctx.error("speed line has invalid speed");

  // expect ninth line to be disengageChanceModifier <modifier>
  ++curr;
  if (curr == end) ctx.error("hull file has no disengageChanceModifier line");
  tokens = split(*curr, ' ');
  if (tokens.size() != 2)
    ctx.error(
        "disengageChanceModifier line is not 'disengageChanceModifier "
        "<modifier>'");
  if (tokens[0] != "disengageChanceModifier")
    ctx.error(
        "disengageChanceModifier line does not start with "
        "'disengageChanceModifier'");
  float disengageChanceModifier = strtof(tokens[1].c_str(), &endptr);
  if (*endptr != '\0')
    ctx.error("disengageChanceModifier line has invalid modifier");

  // expect tenth line to be cost line
  ++curr;
  if (curr == end) ctx.error("hull file has no cost line");
  Cost cost = Cost::fromLine(*curr, ctx);

  // end of input
  ++curr;
  if (curr != end) ctx.error("hull file has extra lines after cost line");

  return Hull(name, size, coreSize, sectionSizes, hullHealth, armourHealth,
              evasion, speed, disengageChanceModifier, cost);
}

Hull::Hull(string const &name_, float size_, string const &coreSize_,
           vector<string> const &sectionSizes_, float hullHealth_,
           float armourHealth_, float evasion_, float speed_,
           float disengageChanceModifier_, Cost const &cost_)
    : Component(name_),
      size(size_),
      coreSize(coreSize_),
      sectionSizes(sectionSizes_),
      hullHealth(hullHealth_),
      armourHealth(armourHealth_),
      evasion(evasion_),
      speed(speed_),
      disengageChanceModifier(disengageChanceModifier_),
      cost(cost_) {}
}  // namespace athena2::model::component
