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

#ifndef ATHENA2_MODEL_ENTITY_PROJECTILE_H_
#define ATHENA2_MODEL_ENTITY_PROJECTILE_H_

#include "model/component/weapon.h"
#include "model/design/ship.h"
#include "model/entity/entity.h"
#include "model/entity/ship.h"

namespace athena2::model::entity {
class Projectile final : public Entity {
 public:
  Projectile(component::Weapon const &, Ship const &) noexcept;

  Projectile(Projectile const &) noexcept = default;
  Projectile(Projectile &&) noexcept = default;

  ~Projectile() noexcept = default;

  Projectile &operator=(Projectile const &) noexcept = default;
  Projectile &operator=(Projectile &&) noexcept = default;

  component::Weapon const &weapon;
  design::Ship const &ship;
};
}  // namespace athena2::model::entity

#endif  // ATHENA2_MODEL_ENTITY_PROJECTILE_H_
