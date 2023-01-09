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

#ifndef ATHENA2_MODEL_COMPONENT_AURA_H_
#define ATHENA2_MODEL_COMPONENT_AURA_H_

#include <string>

#include "dsl.h"
#include "model/component/component.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"

namespace athena2::model::component {
class Aura final : public Component {
 public:
  static Aura fromJson(nlohmann::json const &, EvalContext &);

  std::string const size;
  float const range;
  float const enemyFireRateModifier;
  float const enemyShieldHealthModifier;
  float const enemyDisengageChanceModifier;
  float const allyFireRateModifier;
  float const allyHullRegenModifier;
  float const allyArmourRegenModifier;
  float const allyTrackingBonus;
  float const enemyPointDefenceDamageModifier;
  float const enemyPointDefenceFireRateModifier;
  float const allyStrikeCraftDamageModifier;
  float const allyStrikeCraftSpeedModifier;
  float const allyWeaponsRangeModifier;

 private:
  Aura(std::string const &name, std::string const &size, float range,
       float enemyFireRateModifier, float enemyShieldHealthModifier,
       float enemyDisengageChanceModifier, float allyFireRateModifier,
       float allyHullRegenModifier, float allyArmourRegenModifier,
       float allyTrackingBonus, float enemyPointDefenceDamageModifier,
       float enemyPointDefenceFireRateModifier,
       float allyStrikeCraftDamageModifier, float allyStrikeCraftSpeedModifier,
       float allyWeaponsRangeModifier) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_AURA_H_
