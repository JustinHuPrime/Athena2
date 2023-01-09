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
  optional<float> hullHealthModifier =
      checkMaybeFloat(data, "hullHealthModifier", ctx);
  optional<float> armourHealth = checkMaybeFloat(data, "armourHealth", ctx);
  float evasion = checkFloat(data, "evasion", ctx);
  optional<float> evasionModifier =
      checkMaybeFloat(data, "evasionModifier", ctx);
  optional<float> trackingModifier =
      checkMaybeFloat(data, "trackingModifier", ctx);
  float speed = checkFloat(data, "speed", ctx);
  float disengageChanceModifier =
      checkFloat(data, "disengageChanceModifier", ctx);
  optional<bool> includeComponentCost =
      checkMaybeBool(data, "includeComponentCost", ctx);
  optional<reference_wrapper<json const>> costData =
      checkMaybeObject(data, "cost", ctx);
  Cost cost = costData ? [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(*costData, ctx);
  }() : Cost();

  return Hull(name, size, coreSize, sectionSizes, hullHealth,
              hullHealthModifier.value_or(0.f), armourHealth.value_or(0.f),
              evasion, evasionModifier.value_or(0.f),
              trackingModifier.value_or(0.f), speed, disengageChanceModifier,
              includeComponentCost.value_or(false), cost);
}

Hull::Hull(string const &name_, float size_, string const &coreSize_,
           vector<string> const &sectionSizes_, float hullHealth_,
           float hullHealthModifier_, float armourHealth_, float evasion_,
           float evasionModifier_, float trackingModifier_, float speed_,
           float disengageChanceModifier_, bool includeComponentCost_,
           Cost const &cost_) noexcept
    : Component(name_),
      size(size_),
      coreSize(coreSize_),
      sectionSizes(sectionSizes_),
      hullHealth(hullHealth_),
      hullHealthModifier(hullHealthModifier_),
      armourHealth(armourHealth_),
      evasion(evasion_),
      evasionModifier(evasionModifier_),
      trackingModifier(trackingModifier_),
      speed(speed_),
      disengageChanceModifier(disengageChanceModifier_),
      includeComponentCost(includeComponentCost_),
      cost(cost_) {}
}  // namespace athena2::model::component
