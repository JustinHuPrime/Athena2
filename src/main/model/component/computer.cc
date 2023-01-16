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

#include "model/component/computer.h"

#include <optional>
#include <unordered_map>

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
namespace {
unordered_map<string, Computer::Tactics> tacticsNames{
    {"swarm", Computer::Tactics::SWARM},
    {"torpedo", Computer::Tactics::TORPEDO},
    {"picket", Computer::Tactics::PICKET},
    {"line", Computer::Tactics::LINE},
    {"artillery", Computer::Tactics::ARTILLERY},
};
}

Computer Computer::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  vector<string> sizes = checkStringArray(data, "sizes", ctx);
  string tacticsName = checkString(data, "tactics", ctx);
  auto tactics = tacticsNames.find(tacticsName);
  {
    auto _ = ctx.push("tactics");
    if (tactics == tacticsNames.end())
      ctx.error("invalid tactics '" + tacticsName + "'");
  }
  float power = checkFloat(data, "power", ctx);
  optional<float> fireRateModifier =
      checkMaybeFloat(data, "fireRateModifier", ctx);
  optional<float> evasionModifier =
      checkMaybeFloat(data, "evasionModifier", ctx);
  optional<float> sublightSpeedModifier =
      checkMaybeFloat(data, "sublightSpeedModifier", ctx);
  optional<float> explosiveWeaponsDamageModifier =
      checkMaybeFloat(data, "explosiveWeaponsDamageModifier", ctx);
  optional<float> trackingBonus = checkMaybeFloat(data, "trackingBonus", ctx);
  optional<float> chanceToHitBonus =
      checkMaybeFloat(data, "chanceToHitBonus", ctx);
  optional<float> weaponsRangeModifier =
      checkMaybeFloat(data, "weaponsRangeModifier", ctx);
  optional<float> engagementRangeModifier =
      checkMaybeFloat(data, "engagementRangeModifier", ctx);
  json const &costData = checkObject(data, "cost", ctx);
  Cost cost = [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(costData, ctx);
  }();
  checkFields(
      data,
      {"name", "sizes", "tactics", "power", "fireRateModifier",
       "evasionModifier", "sublightSpeedModifier",
       "explosiveWeaponsDamageModifier", "trackingBonus", "chanceToHitBonus",
       "weaponsRangeModifier", "engagementRangeModifier", "cost"},
      ctx);

  return Computer(
      name, sizes, tactics->second, power, fireRateModifier.value_or(0.f),
      evasionModifier.value_or(0.f), sublightSpeedModifier.value_or(0.f),
      explosiveWeaponsDamageModifier.value_or(0.f), trackingBonus.value_or(0.f),
      chanceToHitBonus.value_or(0.f), weaponsRangeModifier.value_or(0.f),
      engagementRangeModifier.value_or(0.f), cost);
}
Computer::Computer(string const &name_, vector<string> const &sizes_,
                   Computer::Tactics tactics_, float power_,
                   float fireRateModifier_, float evasionModifier_,
                   float sublightSpeedModifier_,
                   float explosiveWeaponsDamageModifier_, float trackingBonus_,
                   float chanceToHitBonus_, float weaponsRangeModifier_,
                   float engagementRangeModifier_, Cost const &cost_) noexcept
    : Named(name_),
      sizes(sizes_),
      tactics(tactics_),
      power(power_),
      fireRateModifier(fireRateModifier_),
      evasionModifier(evasionModifier_),
      sublightSpeedModifier(sublightSpeedModifier_),
      explosiveWeaponsDamageModifier(explosiveWeaponsDamageModifier_),
      trackingBonus(trackingBonus_),
      chanceToHitBonus(chanceToHitBonus_),
      weaponsRangeModifier(weaponsRangeModifier_),
      engagementRangeModifier(engagementRangeModifier_),
      cost(cost_) {}
}  // namespace athena2::model::component
