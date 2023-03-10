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

#include "model/component/aura.h"

#include <optional>

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
Aura Aura::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  string size = checkString(data, "size", ctx);
  optional<float> enemyFireRateModifier =
      checkMaybeFloat(data, "enemyFireRateModifier", ctx);
  optional<float> enemyShieldHealthModifier =
      checkMaybeFloat(data, "enemyShieldHealthModifier", ctx);
  optional<float> enemyDisengageChanceModifier =
      checkMaybeFloat(data, "enemyDisengageChanceModifier", ctx);
  optional<float> allyFireRateModifier =
      checkMaybeFloat(data, "allyFireRateModifier", ctx);
  optional<float> allyHullRegenModifier =
      checkMaybeFloat(data, "allyHullRegenModifier", ctx);
  optional<float> allyArmourRegenModifier =
      checkMaybeFloat(data, "allyArmourRegenModifier", ctx);
  optional<float> allyTrackingBonus =
      checkMaybeFloat(data, "allyTrackingBonus", ctx);
  optional<float> enemyPointDefenceDamageModifier =
      checkMaybeFloat(data, "enemyPointDefenceDamageModifier", ctx);
  optional<float> enemyPointDefenceFireRateModifier =
      checkMaybeFloat(data, "enemyPointDefenceFireRateModifier", ctx);
  optional<float> allyStrikeCraftDamageModifier =
      checkMaybeFloat(data, "allyStrikeCraftDamageModifier", ctx);
  optional<float> allyStrikeCraftSpeedModifier =
      checkMaybeFloat(data, "allyStrikeCraftSpeedModifier", ctx);
  optional<float> allyWeaponsRangeModifier =
      checkMaybeFloat(data, "allyWeaponsRangeModifier", ctx);
  checkFields(
      data,
      {"name", "size", "enemyFireRateModifier", "enemyShieldHealthModifier",
       "enemyDisengageChanceModifier", "allyFireRateModifier",
       "allyHullRegenModifier", "allyArmourRegenModifier", "allyTrackingBonus",
       "enemyPointDefenceDamageModifier", "enemyPointDefenceFireRateModifier",
       "allyStrikeCraftDamageModifier", "allyStrikeCraftSpeedModifier",
       "allyWeaponsRangeModifier"},
      ctx);

  return Aura(
      name, size, enemyFireRateModifier.value_or(0.f),
      enemyShieldHealthModifier.value_or(0.f),
      enemyDisengageChanceModifier.value_or(0.f),
      allyFireRateModifier.value_or(0.f), allyHullRegenModifier.value_or(0.f),
      allyArmourRegenModifier.value_or(0.f), allyTrackingBonus.value_or(0.0f),
      enemyPointDefenceDamageModifier.value_or(0.f),
      enemyPointDefenceFireRateModifier.value_or(0.f),
      allyStrikeCraftDamageModifier.value_or(0.f),
      allyStrikeCraftSpeedModifier.value_or(0.f),
      allyWeaponsRangeModifier.value_or(0.f));
}
Aura::Aura(string const &name_, string const &size_,
           float enemyFireRateModifier_, float enemyShieldHealthModifier_,
           float enemyDisengageChanceModifier_, float allyFireRateModifier_,
           float allyHullRegenModifier_, float allyArmourRegenModifier_,
           float allyTrackingBonus_, float enemyPointDefenceDamageModifier_,
           float enemyPointDefenceFireRateModifier_,
           float allyStrikeCraftDamageModifier_,
           float allyStrikeCraftSpeedModifier_,
           float allyWeaponsRangeModifier_) noexcept
    : Named(name_),
      size(size_),
      enemyFireRateModifier(enemyFireRateModifier_),
      enemyShieldHealthModifier(enemyShieldHealthModifier_),
      enemyDisengageChanceModifier(enemyDisengageChanceModifier_),
      allyFireRateModifier(allyFireRateModifier_),
      allyHullRegenModifier(allyHullRegenModifier_),
      allyArmourRegenModifier(allyArmourRegenModifier_),
      allyTrackingBonus(allyTrackingBonus_),
      enemyPointDefenceDamageModifier(enemyPointDefenceDamageModifier_),
      enemyPointDefenceFireRateModifier(enemyPointDefenceFireRateModifier_),
      allyStrikeCraftDamageModifier(allyStrikeCraftDamageModifier_),
      allyStrikeCraftSpeedModifier(allyStrikeCraftSpeedModifier_),
      allyWeaponsRangeModifier(allyWeaponsRangeModifier_) {}
}  // namespace athena2::model::component
