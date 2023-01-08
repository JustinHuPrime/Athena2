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

#include <optional>
#include <vector>

#include "nlohmann/json.hpp"
#include "util/json.h"

using namespace std;
using namespace athena2::util;
using namespace nlohmann;

namespace athena2::model::component {
Hull Hull::fromJson(nlohmann::json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  float size = checkFloat(data, "size", ctx);
  string coreSize = checkString(data, "coreSize", ctx);
  vector<string> sectionSizes = checkStringArray(data, "sectionSizes", ctx);
  float hullHealth = checkFloat(data, "hullHealth", ctx);
  optional<float> armourHealth = checkMaybeFloat(data, "armourHealth", ctx);
  float evasion = checkFloat(data, "evasion", ctx);
  float speed = checkFloat(data, "speed", ctx);
  float disengageChanceModifier =
      checkFloat(data, "disengageChanceModifier", ctx);
  json const &costData = checkObject(data, "cost", ctx);
  Cost cost = [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(costData, ctx);
  }();

  return Hull(name, size, coreSize, sectionSizes, hullHealth,
              armourHealth.value_or(0.f), evasion, speed,
              disengageChanceModifier, cost);
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
