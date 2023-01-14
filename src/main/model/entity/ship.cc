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

using namespace std;
using namespace athena2::model::component;
using namespace athena2::model::entity;
using namespace athena2::model::design;

namespace athena2::model::entity {
Weapon::Weapon(component::Weapon const &weapon_,
               design::Ship const &ship_) noexcept
    : component(&weapon_), ship(&ship_) {
  switch (component->type) {
    case component::Weapon::Type::REGULAR: {
      data.regularWeapon.cooldown = 0.f;
      break;
    }
    case component::Weapon::Type::PROJECTILE: {
      data.projectileWeapon.cooldown = 0.f;
      break;
    }
    case component::Weapon::Type::HANGAR: {
      data.hangarWeapon.unitsStored =
          component->data.hangarWeapon.unitsPerHangar;
      break;
    }
  }
}
void Weapon::fire() noexcept {
  switch (component->type) {
    case component::Weapon::Type::REGULAR: {
      data.regularWeapon.cooldown = component->cooldown;
      break;
    }
    case component::Weapon::Type::PROJECTILE: {
      data.projectileWeapon.cooldown = component->cooldown;
      break;
    }
    case component::Weapon::Type::HANGAR: {
      --data.hangarWeapon.unitsStored;
      break;
    }
  }
}
Ship::Ship(design::Ship const &design_, float position_) noexcept
    : Entity(design_.hullHealth, design_.armourHealth, design_.armourHardening,
             design_.shieldHealth, design_.shieldHardening, design_.evasion,
             position_),
      weapons(),
      design(&design_),
      disengageChancesRemaining(design->disengageChances),
      willDisengage(false) {}
bool Ship::inRange(Weapon const &weapon, Entity const &target) const noexcept {
  float range = rangeTo(target);
  return weapon.component->minRange <= range &&
         range <=
             weapon.component->maxRange * (1.f + design->weaponsRangeModifier);
}
void Ship::checkRetreat(float hullDamage, mt19937_64 &rng) noexcept {
  if (hull > design->hullHealth * 0.5f)
    return;  // won't retreat when over 50% hull

  if (disengageChancesRemaining <= 0.f)
    return;  // can't disengage without a remaining chance

  disengageChancesRemaining -= 1.f;

  float disengageChance =
      hullDamage / design->hullHealth * 1.5f * design->disengageChanceModifier;
  willDisengage = bernoulli_distribution(disengageChance)(rng);
}
}  // namespace athena2::model::entity
