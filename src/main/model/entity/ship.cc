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
    : component(weapon_), ship(ship_), cooldown(0.f) {}
void Weapon::doDamage(Entity &target) noexcept {
  // reset cooldown
  cooldown = component.cooldown;

  // chanceToHit = probability of a hit from 0 to 1
  float tracking = (component.tracking + ship.design.trackingBonus) *
                   (1.f + ship.design.trackingModifier);
  float chanceToHit = fminf(
      1.f,
      fmaxf(0.f, component.accuracy - fmaxf(0.f, target.evasion - tracking)) +
          ship.design.chanceToHitBonus);

  // baseDamage = weapons damage on a hit
  float baseDamage = ((component.minDamage + component.maxDamage) / 2);
  if (component.tag == "explosive")
    baseDamage *= (1.f + ship.design.explosiveWeaponsDamageModifier);

  // damage = expected damage per hit
  float damage = baseDamage * chanceToHit;

  // shield layer
  float shieldSkipped = damage * component.shieldSkipModifier;
  float shieldDamaging = damage * (1.f - component.shieldSkipModifier);
  shieldDamaging += shieldSkipped * target.shieldHardening;
  shieldSkipped -= shieldSkipped * target.shieldHardening;
  if (shieldDamaging < target.shields / component.shieldDamageModifier) {
    target.shields -= shieldDamaging * component.shieldDamageModifier;
  } else {
    target.shields = 0;
    shieldSkipped +=
        shieldDamaging - target.shields / component.shieldDamageModifier;
  }

  float armourSkipped = shieldSkipped * component.armourSkipModifier;
  float armourDamaging = shieldSkipped * (1.f - component.armourSkipModifier);
  armourDamaging += armourSkipped * target.armourHardening;
  armourSkipped -= armourSkipped * target.armourHardening;
  if (armourDamaging < target.armour / component.armourDamageModifier) {
    target.armour -= armourDamaging * component.armourDamageModifier;
  } else {
    target.armour = 0;
    armourSkipped +=
        armourDamaging - target.armour / component.armourDamageModifier;
  }

  if (armourSkipped < target.hull / component.hullDamageModifier) {
    target.hull -= armourSkipped * component.hullDamageModifier;
  } else {
    target.hull = 0;
    // float overkill = armourSkipped - target.hull /
    // component.hullDamageModifier;
  }
}
Ship::Ship(design::Ship const &design_, float position_) noexcept
    : Entity(design_.hullHealth, design_.armourHealth, design_.armourHardening,
             design_.shieldHealth, design_.shieldHardening, design_.evasion,
             position_),
      weapons(),
      design(design_),
      disengageChancesRemaining(design.disengageChances) {}
}  // namespace athena2::model::entity
