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

#include "model/evaluator.h"

#include <numeric>
#include <random>

#include "model/design/fleet.h"
#include "model/entity/fleet.h"

using namespace athena2::model::design;
using namespace athena2::model::entity;
using namespace athena2::model::component;
using namespace std;

namespace athena2::model {
namespace {
float engagementRange(design::Fleet const &fleet) {
  float maxEngagementRange = 0.f;
  for (auto const &[ship, _] : fleet.ships) {
    float maxWeaponRange = 0.f;
    for (design::Section const &section : ship.sections) {
      for (component::Weapon const *const &weapon : section.weapons) {
        if (weapon->maxRange > maxWeaponRange)
          maxWeaponRange = weapon->maxRange * (1.f + ship.weaponsRangeModifier);
      }
    }
    float shipEngagementRange =
        maxWeaponRange * (1.f + ship.engagementRangeModifier);
    if (shipEngagementRange > maxEngagementRange)
      maxEngagementRange = shipEngagementRange;
  }
  return maxEngagementRange;
}
}  // namespace

void fireWeapons(entity::Fleet &firing, entity::Fleet &targets,
                 mt19937_64 &rng) noexcept {
  // for each ship
  for (entity::Ship &ship : firing.ships) {
    // for each weapon
    for (entity::Weapon &weapon : ship.weapons) {
      switch (weapon.component->type) {
        case component::Weapon::Type::REGULAR: {
          // must have cooled down
          if (weapon.data.regularWeapon.cooldown > 0.f) break;

          // find closest target in range
          Entity *target = nullptr;
          for (Entity &candidate : targets.ships) {
            if (ship.inRange(weapon, candidate) &&
                (!target || ship.rangeTo(candidate) < ship.rangeTo(*target))) {
              target = &candidate;
            }
          }

          // also look at small targets for PD
          if (weapon.component->tag == "point-defence") {
            for (Entity &candidate : targets.projectiles) {
              if (ship.inRange(weapon, candidate) &&
                  (!target ||
                   ship.rangeTo(candidate) < ship.rangeTo(*target))) {
                target = &candidate;
              }
            }
            for (Entity &candidate : targets.strikeCraft) {
              if (ship.inRange(weapon, candidate) &&
                  (!target ||
                   ship.rangeTo(candidate) < ship.rangeTo(*target))) {
                target = &candidate;
              }
            }
          }

          // fire if we have a target
          if (target) {
            weapon.fire();
            target->takeDamage(*weapon.component, *ship.design, rng);
          }
          break;
        }
        case component::Weapon::Type::PROJECTILE: {
          // must have cooled down
          if (weapon.data.projectileWeapon.cooldown > 0.f) break;

          // fire if there's a ship in range
          if (any_of(targets.ships.begin(), targets.ships.end(),
                     [&weapon, &ship](Entity const &target) {
                       return ship.inRange(weapon, target);
                     })) {
            weapon.fire();
            // TODO: add a target to projectiles and implement the retarget
            // mechanic
            firing.projectiles.emplace_back(*weapon.component, ship);
          }
          break;
        }
        case component::Weapon::Type::HANGAR: {
          // must have craft available to deploy
          if (weapon.data.hangarWeapon.unitsStored < 1.f) break;

          // deploy all strike craft
          while (weapon.data.hangarWeapon.unitsStored >= 1.f) {
            weapon.fire();
            firing.strikeCraft.emplace_back(*weapon.component, ship);
          }
          break;
        }
      }
    }
  }
  // for each strike craft
  for (entity::StrikeCraft &strikeCraft : firing.strikeCraft) {
    // must have cooled down
    if (strikeCraft.cooldown > 0.f) break;

    // find closest target in range
    Entity *target = nullptr;
    for (Entity &candidate : targets.ships) {
      if (strikeCraft.inRange(candidate) &&
          (!target ||
           strikeCraft.rangeTo(candidate) < strikeCraft.rangeTo(*target))) {
        target = &candidate;
      }
    }

    // fire if we have a target
    if (target) {
      strikeCraft.fire();
      target->takeDamage(*strikeCraft.weapon, *strikeCraft.ship, rng);
    }
  }
}

void checkProjectiles(entity::Fleet &firing, entity::Fleet &targets,
                      mt19937_64 &rng) noexcept {
  for (entity::Projectile &projectile : firing.projectiles) {
    // is there any ship within retarget range?
    if (!any_of(targets.ships.begin(), targets.ships.end(),
                [&projectile](Entity const &target) {
                  return projectile.rangeTo(target) <=
                         projectile.weapon->data.projectileWeapon
                             .projectileRetargetRange;
                })) {
      // no targets; abort projectile
      projectile.hull = 0.f;
      continue;
    }

    // find closest ship in range
    Entity *target = nullptr;
    for (Entity &candidate : targets.ships) {
      if (projectile.inRange(candidate) &&
          (!target ||
           projectile.rangeTo(candidate) < projectile.rangeTo(*target))) {
        target = &candidate;
      }
    }

    // hit if we have a target
    if (target) {
      projectile.hull = 0.f;  // destroy projectile
      target->takeDamage(*projectile.weapon, *projectile.ship, rng);
    }
  }
}

void applyDisengageAndDestruction(entity::Fleet &fleet) {
  copy_if(fleet.ships.begin(), fleet.ships.end(),
          back_inserter(fleet.disengaged),
          [](entity::Ship const &ship) { return ship.willDisengage; });
  erase_if(fleet.ships, [](entity::Ship const &ship) {
    return ship.willDisengage || ship.hull <= 0.f;
  });
  erase_if(fleet.projectiles,
           [](Entity const &entity) { return entity.hull <= 0.f; });
  erase_if(fleet.strikeCraft,
           [](Entity const &entity) { return entity.hull <= 0.f; });
}

void tick(entity::Fleet &fleet) {
  for (Entity &entity : fleet.ships) entity.tick();
  for (Entity &entity : fleet.strikeCraft) entity.tick();
  for (Entity &entity : fleet.projectiles) entity.tick();
}

void move(entity::Fleet &moving, entity::Fleet const &opponent) {
  for (entity::Ship &ship : moving.ships) {
    // find closest ship
    Entity const *target = nullptr;
    for (Entity const &candidate : opponent.ships) {
      if (!target || ship.rangeTo(candidate) < ship.rangeTo(*target)) {
        target = &candidate;
      }

      if (target) {
        ship.moveToRange(*target, ship.design->preferredRange);
      }
    }
  }
  for (entity::StrikeCraft &strikeCraft : moving.strikeCraft) {
    // find closest ship
    Entity const *target = nullptr;
    for (Entity const &candidate : opponent.ships) {
      if (!target ||
          strikeCraft.rangeTo(candidate) < strikeCraft.rangeTo(*target)) {
        target = &candidate;
      }
    }

    if (target) {
      strikeCraft.moveToRange(*target, 0.f);
    }
  }
  for (entity::Projectile &projectile : moving.projectiles) {
    // find closest ship
    Entity const *target = nullptr;
    for (Entity const &candidate : opponent.ships) {
      if (!target ||
          projectile.rangeTo(candidate) < projectile.rangeTo(*target)) {
        target = &candidate;
      }
    }

    if (target) {
      projectile.moveToRange(*target, 0.f);
    }
  }
}

pair<float, float> evaluate(design::Fleet const &aDesign,
                            design::Fleet const &bDesign,
                            EvaluationSettings const &settings) noexcept {
  // instantiate fleets at max engagement range
  entity::Fleet a = entity::Fleet(aDesign, 0.f);
  entity::Fleet b = entity::Fleet(
      bDesign, fmaxf(engagementRange(aDesign), engagementRange(bDesign)));

  // RNG for this evaluation run
  mt19937_64 rng((random_device())());

  // for each tick
  bool moveOrder = false;
  for (float time = 0.f;
       time < settings.fightLengthLimit && !a.ships.empty() && !b.ships.empty();
       time += TIME_QUANTUM) {
    // fire weapons:
    //  - fire ship weapons
    //  - fire strike craft weapons
    fireWeapons(a, b, rng);
    fireWeapons(b, a, rng);

    // check for projectile hits
    checkProjectiles(a, b, rng);
    checkProjectiles(b, a, rng);

    // apply disengages and destruction
    applyDisengageAndDestruction(a);
    applyDisengageAndDestruction(b);

    // tick
    tick(a);
    tick(b);

    // move
    if (moveOrder) {
      move(a, b);
      move(b, a);
    } else {
      move(b, a);
      move(a, b);
    }
  }

  return pair(accumulate(a.destroyed.begin(), a.destroyed.end(), 0.f,
                         [](float rsf, entity::Ship const &ship) {
                           return rsf + ship.design->cost;
                         }) +
                  settings.withdrawMultiplier *
                      accumulate(a.disengaged.begin(), a.disengaged.end(), 0.f,
                                 [](float rsf, entity::Ship const &ship) {
                                   return rsf + ship.design->cost;
                                 }),
              accumulate(b.destroyed.begin(), b.destroyed.end(), 0.f,
                         [](float rsf, entity::Ship const &ship) {
                           return rsf + ship.design->cost;
                         }) +
                  settings.withdrawMultiplier *
                      accumulate(b.disengaged.begin(), b.disengaged.end(), 0.f,
                                 [](float rsf, entity::Ship const &ship) {
                                   return rsf + ship.design->cost;
                                 }));
}
}  // namespace athena2::model
