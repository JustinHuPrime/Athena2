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

#ifndef ATHENA2_MODEL_COMPONENT_AUXILIARY_H_
#define ATHENA2_MODEL_COMPONENT_AUXILIARY_H_

#include <string>

#include "dsl.h"
#include "model/component/component.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"

namespace athena2::model::component {
class Auxiliary final : public Component {
 public:
  static Auxiliary fromJson(nlohmann::json const &, EvalContext &);

  Auxiliary(Auxiliary const &) noexcept = delete;
  Auxiliary(Auxiliary &&) noexcept = default;

  ~Auxiliary() noexcept override = default;

  Auxiliary &operator=(Auxiliary const &) noexcept = delete;
  Auxiliary &operator=(Auxiliary &&) noexcept = default;

  std::string const size;
  float const power;
  float const sublightSpeedModifier;
  float const evasionModifier;
  float const armourHardeningBonus;
  float const hullRegenModifier;
  float const armourRegenModifier;
  float const shieldHealthModifier;
  float const shieldHardeningBonus;
  float const chanceToHitBonus;
  float const trackingBonus;
  float const disengageChancesBonus;
  Cost const cost;

 private:
  Auxiliary(std::string const &name, std::string const &size, float power,
            float sublightSpeedModifier, float evasionModifier,
            float armourHardeningBonus, float hullRegenModifier,
            float armourRegenModifier, float shieldHealthModifier,
            float shieldHardeningBonus, float chanceToHitBonus,
            float trackingBonus, float disengageChancesBonus,
            Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_AUXILIARY_H_
