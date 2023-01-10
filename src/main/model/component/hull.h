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

#ifndef ATHENA2_MODEL_COMPONENT_HULL_H_
#define ATHENA2_MODEL_COMPONENT_HULL_H_

#include <cstdint>
#include <istream>
#include <string>
#include <vector>

#include "dsl.h"
#include "model/economy.h"
#include "util/named.h"

namespace athena2::model::component {
class Hull final : public util::Named {
 public:
  static Hull fromJson(nlohmann::json const &, EvalContext &);

  Hull(Hull const &) noexcept = delete;
  Hull(Hull &&) noexcept = default;

  ~Hull() noexcept override = default;

  Hull &operator=(Hull const &) noexcept = delete;
  Hull &operator=(Hull &&) noexcept = default;

  float const size;
  std::string const coreSize;
  std::vector<std::string> const sectionSizes;
  float const hullHealth;
  float const hullHealthModifier;
  float const armourHealth;
  float const evasion;
  float const evasionModifier;
  float const trackingModifier;
  float const speed;
  float const disengageChanceModifier;
  bool const includeComponentCost;
  Cost const cost;

 private:
  Hull(std::string const &name, float size, std::string const &coreSize,
       std::vector<std::string> const &sectionSizes, float hullHealth,
       float hullHealthModifier, float armourHealth, float evasion,
       float evasionModifier, float trackingModifier, float speed,
       float disengageChanceModifier, bool includeComponentCost,
       Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_HULL_H_
