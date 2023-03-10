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

#ifndef ATHENA2_MODEL_DESIGN_SHIP_H_
#define ATHENA2_MODEL_DESIGN_SHIP_H_

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dsl.h"
#include "model/component/aura.h"
#include "model/component/componentSet.h"
#include "model/component/computer.h"
#include "model/component/ftl.h"
#include "model/component/hull.h"
#include "model/component/reactor.h"
#include "model/component/sensor.h"
#include "model/component/sublight.h"
#include "model/design/section.h"
#include "nlohmann/json.hpp"
#include "util/named.h"

namespace athena2::model::design {
class Ship final : public util::Named {
 public:
  static Ship fromJson(nlohmann::json const &, component::ComponentSet const &,
                       EvalContext &);

  Ship(Ship const &) noexcept = default;
  Ship(Ship &&) noexcept = default;

  ~Ship() noexcept override = default;

  Ship &operator=(Ship const &) noexcept = default;
  Ship &operator=(Ship &&) noexcept = default;

  // ship design
  component::Hull const *hull;
  component::Reactor const *reactor;
  component::FTL const *ftl;
  component::Sublight const *sublight;
  component::Sensor const *sensor;
  component::Computer const *computer;
  component::Aura const *aura;
  std::vector<Section> const sections;

  // basic stats
  float const power;
  float const cost;
  float const speed;

  // defensive stats
  float const evasion;
  float const hullHealth;
  float const hullRegen;
  float const armourHealth;
  float const armourRegen;
  float const armourHardening;
  float const shieldHealth;
  float const shieldRegen;
  float const shieldHardening;
  float const disengageChances;
  float const disengageChanceModifier;

  // offensive stats
  float const chanceToHitBonus;
  float const trackingBonus;
  float const trackingModifier;
  float const fireRateModifier;
  float const explosiveWeaponsDamageModifier;
  float const weaponsRangeModifier;
  float const engagementRangeModifier;
  float const preferredRange;

 private:
  /**
   * @throws DesignException if ship design is invalid (e.g. not enough power)
   */
  Ship(std::string const &, component::Hull const &, component::Reactor const &,
       component::FTL const &, component::Sublight const &,
       component::Sensor const &, component::Computer const &,
       component::Aura const *, std::vector<Section> const &);
};
}  // namespace athena2::model::design

#endif  // ATHENA2_MODEL_DESIGN_SHIP_H_
