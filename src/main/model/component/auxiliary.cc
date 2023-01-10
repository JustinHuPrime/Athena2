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

#include "model/component/auxiliary.h"

#include <optional>

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
Auxiliary Auxiliary::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  float power = checkFloat(data, "power", ctx);
  optional<float> sublightSpeedModifier =
      checkMaybeFloat(data, "sublightSpeedModifier", ctx);
  optional<float> evasionModifier =
      checkMaybeFloat(data, "evasionModifier", ctx);
  optional<float> armourHardeningBonus =
      checkMaybeFloat(data, "armourHardeningBonus", ctx);
  optional<float> hullRegenModifier =
      checkMaybeFloat(data, "hullRegenModifier", ctx);
  optional<float> armourRegenModifier =
      checkMaybeFloat(data, "armourRegenModifier", ctx);
  optional<float> shieldHealthModifier =
      checkMaybeFloat(data, "shieldHealthModifier", ctx);
  optional<float> shieldHardeningBonus =
      checkMaybeFloat(data, "shieldHardeningBonus", ctx);
  optional<float> chanceToHitBonus =
      checkMaybeFloat(data, "chanceToHitBonus", ctx);
  optional<float> trackingBonus = checkMaybeFloat(data, "trackingBonus", ctx);
  optional<float> disengageChancesBonus =
      checkMaybeFloat(data, "disengageChancesBonus", ctx);
  json const &costData = checkObject(data, "cost", ctx);
  Cost cost = [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(costData, ctx);
  }();
  checkFields(
      data,
      {"name", "power", "cost", "sublightSpeedModifier", "evasionModifier",
       "armourHardeningBonus", "hullRegenModifier", "armourRegenModifier",
       "shieldHealthModifier", "shieldHardeningBonus", "chanceToHitBonus",
       "trackingBonus", "disengageChancesBonus"},
      ctx);

  return Auxiliary(
      name, "A", power, sublightSpeedModifier.value_or(0.f),
      evasionModifier.value_or(0.f), armourHardeningBonus.value_or(0.f),
      hullRegenModifier.value_or(0.f), armourRegenModifier.value_or(0.f),
      shieldHealthModifier.value_or(0.f), shieldHardeningBonus.value_or(0.f),
      chanceToHitBonus.value_or(0.f), trackingBonus.value_or(0.f),
      disengageChancesBonus.value_or(0.f), cost);
}
Auxiliary::Auxiliary(string const &name_, string const &size_, float power_,
                     float sublightSpeedModifier_, float evasionModifier_,
                     float armourHardeningBonus_, float hullRegenModifier_,
                     float armourRegenModifier_, float shieldHealthModifier_,
                     float shieldHardeningBonus_, float chanceToHitBonus_,
                     float trackingBonus_, float disengageChancesBonus_,
                     Cost const &cost_) noexcept
    : Named(name_),
      size(size_),
      power(power_),
      sublightSpeedModifier(sublightSpeedModifier_),
      evasionModifier(evasionModifier_),
      armourHardeningBonus(armourHardeningBonus_),
      hullRegenModifier(hullRegenModifier_),
      armourRegenModifier(armourRegenModifier_),
      shieldHealthModifier(shieldHealthModifier_),
      shieldHardeningBonus(shieldHardeningBonus_),
      chanceToHitBonus(chanceToHitBonus_),
      trackingBonus(trackingBonus_),
      disengageChancesBonus(disengageChancesBonus_),
      cost(cost_) {}
}  // namespace athena2::model::component
