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

#ifndef ATHENA2_MODEL_ENTITY_ENTITY_H_
#define ATHENA2_MODEL_ENTITY_ENTITY_H_

#include <random>

#include "model/component/weapon.h"
#include "model/design/ship.h"

namespace athena2::model::entity {
class Entity {
 public:
  Entity(float hull, float armour, float armourHardening, float shields,
         float shieldHardening, float evasion, float position,
         float speed) noexcept;
  Entity(Entity const &) noexcept = default;
  Entity(Entity &&) noexcept = default;

  virtual ~Entity() noexcept = default;

  Entity &operator=(Entity const &) noexcept = default;
  Entity &operator=(Entity &&) noexcept = default;

  float rangeTo(Entity const &target) const noexcept;

  void takeDamage(component::Weapon const &weapon, design::Ship const &ship,
                  std::mt19937_64 &rng) noexcept;
  virtual void checkRetreat(float damage, std::mt19937_64 &rng) noexcept;
  virtual void tick() noexcept;

  void moveToRange(Entity const &target, float range) noexcept;

  float hull;
  float armour;
  float armourHardening;
  float shields;
  float shieldHardening;
  float evasion;
  float position;
  float speed;
};
}  // namespace athena2::model::entity

#endif  // ATHENA2_MODEL_ENTITY_ENTITY_H_
