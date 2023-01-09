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

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
Sublight Sublight::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  vector<string> sizes = checkStringArray(data, "sizes", ctx);
  float power = checkFloat(data, "power", ctx);
  float speedModifier = checkFloat(data, "speedModifier", ctx);
  float evasionBonus = checkFloat(data, "evasionBonus", ctx);
  json const &costData = checkObject(data, "cost", ctx);
  Cost cost = [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(costData, ctx);
  }();
  checkFields(
      data, {"name", "sizes", "power", "speedModifier", "evasionBonus", "cost"},
      ctx);

  return Sublight(name, sizes, power, speedModifier, evasionBonus, cost);
}
Sublight::Sublight(string const &name_, vector<string> const &sizes_,
                   float power_, float speedModifier_, float evasionBonus_,
                   Cost const &cost_) noexcept
    : Component(name_),
      sizes(sizes_),
      power(power_),
      speedModifier(speedModifier_),
      evasionBonus(evasionBonus_),
      cost(cost_) {}
}  // namespace athena2::model::component
