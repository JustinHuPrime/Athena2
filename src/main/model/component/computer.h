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

#ifndef ATHENA2_MODEL_COMPONENT_COMPUTER_H_
#define ATHENA2_MODEL_COMPONENT_COMPUTER_H_

#include <string>
#include <vector>

#include "dsl.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"
#include "util/named.h"

namespace athena2::model::component {
class Computer final : public util::Named {
 public:
  enum class Tactics {
    SWARM,
    TORPEDO,
    PICKET,
    LINE,
    ARTILLERY,
    CARRIER,
  };

  static Computer fromJson(nlohmann::json const &, EvalContext &);

  Computer(Computer const &) noexcept = delete;
  Computer(Computer &&) noexcept = default;

  ~Computer() noexcept override = default;

  Computer &operator=(Computer const &) noexcept = delete;
  Computer &operator=(Computer &&) noexcept = default;

  std::vector<std::string> const sizes;
  Tactics tactics;
  float const power;
  float const fireRateModifier;
  float const evasionModifier;
  float const sublightSpeedModifier;
  float const explosiveWeaponsDamageModifier;
  float const trackingBonus;
  float const chanceToHitBonus;
  float const weaponsRangeModifier;
  float const engagementRangeModifier;
  Cost const cost;

 private:
  Computer(std::string const &name, std::vector<std::string> const &sizes,
           Tactics tactics, float power, float fireRateModifier,
           float evasionModifier, float sublightSpeedModifier,
           float explosiveWeaponsDamageModifier, float trackingBonus,
           float chanceToHitBonus, float weaponsRangeModifier,
           float engagementRangeModifier, Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_COMPUTER_H_
