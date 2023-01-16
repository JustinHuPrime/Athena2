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

#include "model/entity/strikeCraft.h"

#include "model/evaluator.h"

using namespace athena2::model::component;
using namespace athena2::model::entity;
using namespace athena2::model::design;

namespace athena2::model::entity {
StrikeCraft::StrikeCraft(component::Weapon const &weapon_,
                         Ship const &ship_) noexcept
    : Entity(weapon_.data.hangarWeapon.strikeCraftHull,
             weapon_.data.hangarWeapon.strikeCraftArmour, 0.f,
             weapon_.data.hangarWeapon.strikeCraftHull, 0.f,
             weapon_.data.hangarWeapon.strikeCraftEvasion, ship_.position,
             weapon_.data.hangarWeapon.strikeCraftSpeed),
      weapon(&weapon_),
      ship(ship_.design) {}
bool StrikeCraft::inRange(Entity const &target) const noexcept {
  return rangeTo(target) < weapon->data.hangarWeapon.strikeCraftRange *
                               (1.f + ship->weaponsRangeModifier);
}
void StrikeCraft::fire() noexcept { cooldown = weapon->cooldown; }
void StrikeCraft::tick() noexcept {
  cooldown -= TIME_QUANTUM * (1.f + ship->fireRateModifier);
}
}  // namespace athena2::model::entity
