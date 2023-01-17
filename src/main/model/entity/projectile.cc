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

#include "model/entity/projectile.h"

#include "model/evaluator.h"

using namespace athena2::model::component;
using namespace athena2::model::entity;
using namespace athena2::model::design;
using namespace nlohmann;

namespace athena2::model::entity {
Projectile::Projectile(component::Weapon const &weapon_,
                       Ship const &ship_) noexcept
    : Entity(weapon_.data.projectileWeapon.projectileHull,
             weapon_.data.projectileWeapon.projectileArmour, 0.f, 0.f, 0.f,
             weapon_.data.projectileWeapon.projectileEvasion, ship_.position,
             weapon_.data.projectileWeapon.projectileSpeed),
      weapon(&weapon_),
      ship(ship_.design) {}
bool Projectile::inRange(Entity const &target) const noexcept {
  return rangeTo(target) <=
         weapon->data.projectileWeapon.projectileSpeed * TIME_QUANTUM;
}
void to_json(json &j, Projectile const &p) noexcept {
  return to_json(j, static_cast<Entity const &>(p));
}
}  // namespace athena2::model::entity
