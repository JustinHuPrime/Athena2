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

#ifndef ATHENA2_MODEL_ENTITY_FLEET_H_
#define ATHENA2_MODEL_ENTITY_FLEET_H_

#include <vector>

#include "model/design/fleet.h"
#include "model/entity/projectile.h"
#include "model/entity/ship.h"
#include "model/entity/strikeCraft.h"

namespace athena2::model::entity {
class Fleet final {
 public:
  Fleet(design::Fleet const &design, float position) noexcept;

  Fleet(Fleet const &) noexcept = default;
  Fleet(Fleet &&) noexcept = default;

  ~Fleet() noexcept = default;

  Fleet &operator=(Fleet const &) noexcept = default;
  Fleet &operator=(Fleet &&) noexcept = default;

  std::vector<Ship> ships;
  std::vector<Projectile> projectiles;
  std::vector<StrikeCraft> strikeCraft;
  std::vector<Ship> destroyed;
  std::vector<Ship> disengaged;
};
}  // namespace athena2::model::entity

#endif  // ATHENA2_MODEL_ENTITY_FLEET_H_
