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

#include "model/entity/ship.h"

using namespace athena2::model::component;
using namespace athena2::model::entity;
using namespace athena2::model::design;

namespace athena2::model::entity {
Weapon::Weapon(component::Weapon const &weapon_, Ship const &ship_) noexcept
    : component(weapon_), ship(ship_) {}
Ship::Ship(design::Ship const &design_, float position_) noexcept
    : Entity(design_.hullHealth, design_.armourHealth, design_.shieldHealth,
             position_),
      weapons(),
      design(design_),
      disengageChancesRemaining(design.disengageChances) {}
}  // namespace athena2::model::entity
