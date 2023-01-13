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

#ifndef ATHENA2_MODEL_COMPONENT_WEAPON_H_
#define ATHENA2_MODEL_COMPONENT_WEAPON_H_

#include <string>

#include "dsl.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"
#include "util/named.h"

namespace athena2::model::component {
class Weapon : public util::Named {
 public:
  enum class Type {
    REGULAR,
    PROJECTILE,
    HANGAR,
  };

  static Weapon fromJson(nlohmann::json const &, EvalContext &);

  Weapon(Weapon const &) noexcept = delete;
  Weapon(Weapon &&) noexcept = default;

  ~Weapon() noexcept override = default;

  Weapon &operator=(Weapon const &) noexcept = delete;
  Weapon &operator=(Weapon &&) noexcept = default;

  std::string const size;
  std::string const tag;
  float const power;
  float const minDamage;
  float const maxDamage;
  float const minRange;
  float const maxRange;
  float const tracking;
  float const accuracy;
  float const cooldown;
  float const shieldDamageModifier;
  float const shieldSkipModifier;
  float const armourDamageModifier;
  float const armourSkipModifier;
  float const hullDamageModifier;
  float const sizeDamageModifier;
  Type type;
  union {
    struct {
    } regularWeapon;
    struct {
      float const projectileSpeed;
      float const projectileEvasion;
      float const projectileRetargetRange;
      float const projectileHull;
      float const projectileArmour;
    } projectileWeapon;
    struct {
      float const unitsPerHangar;
      float const regenerationPerDay;
      float const strikeCraftRange;
      float const strikeCraftSpeed;
      float const strikeCraftEvasion;
      float const strikeCraftShield;
      float const strikeCraftArmour;
      float const strikeCraftHull;
    } hangarWeapon;
  } data;
  Cost const cost;

 private:
  Weapon(std::string const &name, std::string const &size,
         std::string const &tag, float power, float minDamage, float maxDamage,
         float minRange, float maxRange, float tracking, float accuracy,
         float cooldown, float shieldDamageModifier, float shieldSkipModifier,
         float armourDamageModifier, float armourSkipModifier,
         float hullDamageModifier, float sizeDamageModifier,
         Cost const &cost) noexcept;
  Weapon(std::string const &name, std::string const &size,
         std::string const &tag, float power, float minDamage, float maxDamage,
         float minRange, float maxRange, float tracking, float accuracy,
         float cooldown, float shieldDamageModifier, float shieldSkipModifier,
         float armourDamageModifier, float armourSkipModifier,
         float hullDamageModifier, float sizeDamageModifier,
         float projectileSpeed, float projectileEvasion,
         float projectileRetargetRange, float projectileHull,
         float projectileArmour, Cost const &cost) noexcept;
  Weapon(std::string const &name, std::string const &size,
         std::string const &tag, float power, float minDamage, float maxDamage,
         float minRange, float maxRange, float tracking, float accuracy,
         float cooldown, float shieldDamageModifier, float shieldSkipModifier,
         float armourDamageModifier, float armourSkipModifier,
         float hullDamageModifier, float sizeDamageModifier,
         float unitsPerHangar, float regenerationPerDay, float strikeCraftRange,
         float strikeCraftSpeed, float strikeCraftEvasion,
         float strikeCraftShield, float strikeCraftArmour,
         float strikeCraftHull, Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_WEAPON_H_
