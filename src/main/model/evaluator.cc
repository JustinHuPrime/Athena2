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
      for (component::Weapon const &weapon : section.weapons) {
        if (weapon.maxRange > maxWeaponRange) maxWeaponRange = weapon.maxRange;
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

pair<float, float> evaluate(design::Fleet const &aDesign,
                            design::Fleet const &bDesign,
                            EvaluationSettings const &settings) noexcept {
  // instantiate fleets at max engagement range
  entity::Fleet a = entity::Fleet(aDesign, 0.f);
  entity::Fleet b = entity::Fleet(
      bDesign, fmaxf(engagementRange(aDesign), engagementRange(bDesign)));

  // for each tick
  for (float tick = 0.f;
       tick < settings.fightLengthLimit && !a.ships.empty() && !b.ships.empty();
       tick += 0.1f) {
    // fire weapons:
    //  - fire ship weapons
    //  - fire strike craft weapons
    // check for projectile hits
    // apply damage
    //  - update damage penalties to fire rate
    //  - check for disengage
    // tick weapons
    // tick regen
    // move ships
  }

  return pair(accumulate(a.destroyed.begin(), a.destroyed.end(), 0.f,
                         [](float rsf, entity::Ship const &ship) {
                           return rsf + ship.design.cost;
                         }) +
                  settings.withdrawMultiplier *
                      accumulate(a.withdrawn.begin(), a.withdrawn.end(), 0.f,
                                 [](float rsf, entity::Ship const &ship) {
                                   return rsf + ship.design.cost;
                                 }),
              accumulate(b.destroyed.begin(), b.destroyed.end(), 0.f,
                         [](float rsf, entity::Ship const &ship) {
                           return rsf + ship.design.cost;
                         }) +
                  settings.withdrawMultiplier *
                      accumulate(b.withdrawn.begin(), b.withdrawn.end(), 0.f,
                                 [](float rsf, entity::Ship const &ship) {
                                   return rsf + ship.design.cost;
                                 }));
}
}  // namespace athena2::model
