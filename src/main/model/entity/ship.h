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

#ifndef ATHENA2_MODEL_ENTITY_SHIP_H_
#define ATHENA2_MODEL_ENTITY_SHIP_H_

#include <vector>

#include "model/component/weapon.h"
#include "model/design/ship.h"
#include "model/entity/entity.h"

namespace athena2::model::entity {
class Ship;
class Weapon final {
 public:
  Weapon(component::Weapon const &, design::Ship const &) noexcept;

  Weapon(Weapon const &) noexcept = default;
  Weapon(Weapon &&) noexcept = default;

  ~Weapon() noexcept = default;

  Weapon &operator=(Weapon const &) noexcept = default;
  Weapon &operator=(Weapon &&) noexcept = default;

  void fire() noexcept;
  void tick(Ship const &) noexcept;

  component::Weapon const *component;
  design::Ship const *ship;
  union {
    struct {
      float cooldown;
    } regularWeapon;
    struct {
      float cooldown;
    } projectileWeapon;
    struct {
      float unitsStored;
    } hangarWeapon;
  } data;
};
void to_json(nlohmann::json &, Weapon const &) noexcept;
class Ship final : public Entity {
 public:
  Ship(design::Ship const &, float position) noexcept;

  Ship(Ship const &) noexcept = default;
  Ship(Ship &&) noexcept = default;

  ~Ship() noexcept override = default;

  Ship &operator=(Ship const &) noexcept = default;
  Ship &operator=(Ship &&) noexcept = default;

  bool inRange(Weapon const &weapon, Entity const &target) const noexcept;
  void checkRetreat(float hullDamage, std::mt19937_64 &rng) noexcept override;
  void tick() noexcept override;

  std::vector<Weapon> weapons;
  design::Ship const *design;
  float disengageChancesRemaining;
  bool willDisengage;
};
void to_json(nlohmann::json &, Ship const &) noexcept;
}  // namespace athena2::model::entity

#endif  // ATHENA2_MODEL_ENTITY_SHIP_H_
