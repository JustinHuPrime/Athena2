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

#include "model/entity/entity.h"

#include "model/evaluator.h"

using namespace std;
using namespace athena2::model::component;
using namespace athena2::model::design;

namespace athena2::model::entity {
Entity::Entity(float hull_, float armour_, float armourHardening_,
               float shields_, float shieldHardening_, float evasion_,
               float position_, float speed_) noexcept
    : hull(hull_),
      armour(armour_),
      armourHardening(armourHardening_),
      shields(shields_),
      shieldHardening(shieldHardening_),
      position(position_),
      evasion(evasion_),
      speed(speed_) {}
float Entity::rangeTo(Entity const &target) const noexcept {
  return fabs(position - position);
}
void Entity::takeDamage(Weapon const &weapon, Ship const &ship,
                        mt19937_64 &rng) noexcept {
  // chanceToHit = probability of a hit from 0 to 1
  float tracking =
      (weapon.tracking + ship.trackingBonus) * (1.f + ship.trackingModifier);
  float chanceToHit =
      fminf(1.f, fmaxf(0.f, weapon.accuracy - fmaxf(0.f, evasion - tracking)) +
                     ship.chanceToHitBonus);
  if (!bernoulli_distribution(clamp(chanceToHit, 0.f, 1.f))(rng)) {
    // missed; no damage done
    return;
  }

  // damage = weapons damage on a hit
  float damage =
      uniform_real_distribution(weapon.minDamage, weapon.maxDamage)(rng);
  if (weapon.tag == "explosive")
    damage *= (1.f + ship.explosiveWeaponsDamageModifier);

  // shield layer
  float shieldSkipped = damage * weapon.shieldSkipModifier;
  float shieldDamaging = damage * (1.f - weapon.shieldSkipModifier);
  shieldDamaging += shieldSkipped * shieldHardening;
  shieldSkipped -= shieldSkipped * shieldHardening;
  if (shieldDamaging < shields / weapon.shieldDamageModifier) {
    shields -= shieldDamaging * weapon.shieldDamageModifier;
  } else {
    shields = 0;
    shieldSkipped += shieldDamaging - shields / weapon.shieldDamageModifier;
  }

  float armourSkipped = shieldSkipped * weapon.armourSkipModifier;
  float armourDamaging = shieldSkipped * (1.f - weapon.armourSkipModifier);
  armourDamaging += armourSkipped * armourHardening;
  armourSkipped -= armourSkipped * armourHardening;
  if (armourDamaging < armour / weapon.armourDamageModifier) {
    armour -= armourDamaging * weapon.armourDamageModifier;
  } else {
    armour = 0;
    armourSkipped += armourDamaging - armour / weapon.armourDamageModifier;
  }

  if (armourSkipped < hull / weapon.hullDamageModifier) {
    float hullDamage = armourSkipped * weapon.hullDamageModifier;
    hull -= hullDamage;
    checkRetreat(hullDamage, rng);
  } else {
    hull = 0;
    // float overkill = armourSkipped - hull /
    // component.hullDamageModifier;
  }
}
void Entity::checkRetreat(float, mt19937_64 &) noexcept {}
void Entity::tick() noexcept {}
void Entity::moveToRange(Entity const &target, float range) noexcept {
  // find target location
  float smallerCandidate = target.position - range;
  float largerCandidate = target.position + range;
  float destination =
      fabs(position - smallerCandidate) < fabs(position - largerCandidate)
          ? smallerCandidate
          : largerCandidate;
  if (fabs(destination - position) <= TIME_QUANTUM * speed) {
    position = destination;
  } else if (destination < position) {
    position -= TIME_QUANTUM * speed;
  } else {
    position += TIME_QUANTUM * speed;
  }
}
}  // namespace athena2::model::entity
