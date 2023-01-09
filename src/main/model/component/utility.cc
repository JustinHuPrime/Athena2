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

#include "model/component/utility.h"

#include <optional>

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
Utility Utility::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  string size = checkString(data, "size", ctx);
  {
    auto _ = ctx.push("size");
    if (size.size() != 1) ctx.error("expected size to be a single character");
  }
  float power = checkFloat(data, "power", ctx);
  optional<float> shieldHealth = checkMaybeFloat(data, "shieldHealth", ctx);
  optional<float> shieldRegen = checkMaybeFloat(data, "shieldRegen", ctx);
  optional<float> armourHealth = checkMaybeFloat(data, "armourHealth", ctx);
  optional<float> hullHealth = checkMaybeFloat(data, "hullHealth", ctx);
  json const &costData = checkObject(data, "cost", ctx);
  Cost cost = [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(costData, ctx);
  }();
  checkFields(data,
              {"name", "size", "power", "cost", "shieldHealth", "shieldRegen",
               "armourHealth", "hullHealth"},
              ctx);

  return Utility(name, size, power, shieldHealth.value_or(0.f),
                 shieldRegen.value_or(0.f), armourHealth.value_or(0.f),
                 hullHealth.value_or(0.f), cost);
}
Utility::Utility(string const &name_, string const &size_, float power_,
                 float shieldHealth_, float shieldRegen_, float armourHealth_,
                 float hullHealth_, Cost const &cost_) noexcept
    : Component(name_),
      size(size_),
      power(power_),
      shieldHealth(shieldHealth_),
      shieldRegen(shieldRegen_),
      armourHealth(armourHealth_),
      hullHealth(hullHealth_),
      cost(cost_) {}
}  // namespace athena2::model::component
