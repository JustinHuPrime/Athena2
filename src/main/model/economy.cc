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

#include "model/economy.h"

#include <unordered_map>
#include <vector>

#include "util/string.h"

using namespace std;
using namespace athena2::util;

namespace athena2::model {
Cost Cost::fromLine(string const &line, EvalContext &ctx) {
  auto _ = ctx.push("cost");

  vector<string> tokens = split(line, ' ');
  if (tokens.empty()) {
    ctx.error("cost line is empty");
  } else if (tokens[0] != "cost") {
    ctx.error("cost line does not start with 'cost'");
  }

  if (line == "cost 0") return Cost(0, 0, 0, 0, 0, 0);

  unordered_map<string, float> values;
  values["alloys"] = 0;
  values["minerals"] = 0;
  values["darkMatter"] = 0;
  values["strategicResources"] = 0;
  values["zro"] = 0;
  values["nanites"] = 0;
  auto curr = tokens.begin() + 1;
  auto end = tokens.end();
  while (curr != end) {
    if (curr + 1 == end)
      ctx.error("cost line has resource not followed by value");

    string const &key = *curr;
    string const &value = *(curr + 1);
    if (values.find(key) == values.end())
      ctx.error("cost line has unknown resource '" + key + "'");

    char *endptr;
    float parsed = strtof(value.c_str(), &endptr);
    if (*endptr != '\0')
      ctx.error("cost line has invalid value '" + value + "'");

    values[key] = parsed;

    curr += 2;
  }

  return Cost(values["alloys"], values["minerals"], values["darkMatter"],
              values["strategicResources"], values["zro"], values["nanites"]);
}

Cost::Cost(float alloys_, float minerals_, float darkMatter_,
           float strategicResources_, float zro_, float nanites_) noexcept
    : alloys(alloys_),
      minerals(minerals_),
      darkMatter(darkMatter_),
      strategicResources(strategicResources_),
      zro(zro_),
      nanites(nanites_) {}
Cost::operator string() const noexcept {
  return to_string(operator float()) + " mineral-equivalents";
}
Cost::operator float() const noexcept {
  return alloys * 2.0f + minerals * 1.0f + darkMatter * 20.0f +
         strategicResources * 5.0f + zro * 20.0f + nanites * 40.0f;
}
}  // namespace athena2::model
