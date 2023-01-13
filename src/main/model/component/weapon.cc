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

#include "model/component/weapon.h"

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
Weapon Weapon::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  string size = checkString(data, "size", ctx);
  {
    auto _ = ctx.push("size");
    if (size.size() != 1) ctx.error("expected size to be a single character");
  }
  string tag = checkString(data, "tag", ctx);
  float power = checkFloat(data, "power", ctx);
  float minDamage = checkFloat(data, "minDamage", ctx);
  float maxDamage = checkFloat(data, "maxDamage", ctx);
  float minRange = checkFloat(data, "minRange", ctx);
  float maxRange = checkFloat(data, "maxRange", ctx);
  float tracking = checkFloat(data, "tracking", ctx);
  float accuracy = checkFloat(data, "accuracy", ctx);
  float cooldown = checkFloat(data, "cooldown", ctx);
  optional<float> shieldDamageModifier =
      checkMaybeFloat(data, "shieldDamageModifier", ctx);
  optional<float> shieldSkipModifier =
      checkMaybeFloat(data, "shieldSkipModifier", ctx);
  optional<float> armourDamageModifier =
      checkMaybeFloat(data, "armourDamageModifier", ctx);
  optional<float> armourSkipModifier =
      checkMaybeFloat(data, "armourSkipModifier", ctx);
  optional<float> hullDamageModifier =
      checkMaybeFloat(data, "hullDamageModifier", ctx);
  optional<float> sizeDamageModifier =
      checkMaybeFloat(data, "sizeDamageModifier", ctx);
  json const &costData = checkObject(data, "cost", ctx);
  Cost cost = [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(costData, ctx);
  }();
  if (data.contains("projectileSpeed")) {
    // projectile weapon
    float projectileSpeed = checkFloat(data, "projectileSpeed", ctx);
    float projectileEvasion = checkFloat(data, "projectileEvasion", ctx);
    float projectileRetargetRange =
        checkFloat(data, "projectileRetargetRange", ctx);
    float projectileHull = checkFloat(data, "projectileHull", ctx);
    float projectileArmour = checkFloat(data, "projectileArmour", ctx);
    checkFields(data,
                {"name",
                 "size",
                 "tag",
                 "power",
                 "minDamage",
                 "maxDamage",
                 "minRange",
                 "maxRange",
                 "tracking",
                 "accuracy",
                 "cooldown",
                 "shieldDamageModifier",
                 "shieldSkipModifier",
                 "armourDamageModifier",
                 "armourSkipModifier",
                 "hullDamageModifier",
                 "sizeDamageModifier",
                 "projectileSpeed",
                 "projectileEvasion",
                 "projectileRetargetRange",
                 "projectileHull",
                 "projectileArmour",
                 "cost"},
                ctx);

    return Weapon(
        name, size, tag, power, minDamage, maxDamage, minRange, maxRange,
        tracking, accuracy, cooldown, shieldDamageModifier.value_or(0.f),
        shieldSkipModifier.value_or(0.f), armourDamageModifier.value_or(0.f),
        armourSkipModifier.value_or(0.f), hullDamageModifier.value_or(0.f),
        sizeDamageModifier.value_or(0.f), projectileSpeed, projectileEvasion,
        projectileRetargetRange, projectileHull, projectileArmour, cost);
  } else if (data.contains("unitsPerHangar")) {
    // hangar weapon
    float unitsPerHangar = checkFloat(data, "unitsPerHangar", ctx);
    float regenerationPerDay = checkFloat(data, "regenerationPerDay", ctx);
    float strikeCraftRange = checkFloat(data, "strikeCraftRange", ctx);
    float strikeCraftSpeed = checkFloat(data, "strikeCraftSpeed", ctx);
    float strikeCraftEvasion = checkFloat(data, "strikeCraftEvasion", ctx);
    float strikeCraftShield = checkFloat(data, "strikeCraftShield", ctx);
    float strikeCraftArmour = checkFloat(data, "strikeCraftArmour", ctx);
    float strikeCraftHull = checkFloat(data, "strikeCraftHull", ctx);
    checkFields(data,
                {"name",
                 "size",
                 "tag",
                 "power",
                 "minDamage",
                 "maxDamage",
                 "minRange",
                 "maxRange",
                 "tracking",
                 "accuracy",
                 "cooldown",
                 "shieldDamageModifier",
                 "shieldSkipModifier",
                 "armourDamageModifier",
                 "armourSkipModifier",
                 "hullDamageModifier",
                 "sizeDamageModifier",
                 "unitsPerHangar",
                 "regenerationPerDay",
                 "strikeCraftRange",
                 "strikeCraftSpeed",
                 "strikeCraftEvasion",
                 "strikeCraftShield",
                 "strikeCraftArmour",
                 "strikeCraftHull",
                 "cost"},
                ctx);

    return Weapon(
        name, size, tag, power, minDamage, maxDamage, minRange, maxRange,
        tracking, accuracy, cooldown, shieldDamageModifier.value_or(0.f),
        shieldSkipModifier.value_or(0.f), armourDamageModifier.value_or(0.f),
        armourSkipModifier.value_or(0.f), hullDamageModifier.value_or(0.f),
        sizeDamageModifier.value_or(0.f), unitsPerHangar, regenerationPerDay,
        strikeCraftRange, strikeCraftSpeed, strikeCraftEvasion,
        strikeCraftShield, strikeCraftArmour, strikeCraftHull, cost);
  } else {
    // regular weapon
    checkFields(
        data,
        {"name", "size", "tag", "power", "minDamage", "maxDamage", "minRange",
         "maxRange", "tracking", "accuracy", "cooldown", "shieldDamageModifier",
         "shieldSkipModifier", "armourDamageModifier", "armourSkipModifier",
         "hullDamageModifier", "sizeDamageModifier", "cost"},
        ctx);

    return Weapon(
        name, size, tag, power, minDamage, maxDamage, minRange, maxRange,
        tracking, accuracy, cooldown, shieldDamageModifier.value_or(0.f),
        shieldSkipModifier.value_or(0.f), armourDamageModifier.value_or(0.f),
        armourSkipModifier.value_or(0.f), hullDamageModifier.value_or(0.f),
        sizeDamageModifier.value_or(0.f), cost);
  }
}
Weapon::Weapon(string const &name_, string const &size_, string const &tag_,
               float power_, float minDamage_, float maxDamage_,
               float minRange_, float maxRange_, float tracking_,
               float accuracy_, float cooldown_, float shieldDamageModifier_,
               float shieldSkipModifier_, float armourDamageModifier_,
               float armourSkipModifier_, float hullDamageModifier_,
               float sizeDamageModifier_, Cost const &cost_) noexcept
    : Named(name_),
      size(size_),
      tag(tag_),
      power(power_),
      minDamage(minDamage_),
      maxDamage(maxDamage_),
      minRange(minRange_),
      maxRange(maxRange_),
      tracking(tracking_),
      accuracy(accuracy_),
      cooldown(cooldown_),
      shieldDamageModifier(shieldDamageModifier_),
      shieldSkipModifier(shieldSkipModifier_),
      armourDamageModifier(armourDamageModifier_),
      armourSkipModifier(armourSkipModifier_),
      hullDamageModifier(hullDamageModifier_),
      sizeDamageModifier(sizeDamageModifier_),
      type(Weapon::Type::REGULAR),
      data{.regularWeapon{}},
      cost(cost_) {}
Weapon::Weapon(string const &name_, string const &size_, string const &tag_,
               float power_, float minDamage_, float maxDamage_,
               float minRange_, float maxRange_, float tracking_,
               float accuracy_, float cooldown_, float shieldDamageModifier_,
               float shieldSkipModifier_, float armourDamageModifier_,
               float armourSkipModifier_, float hullDamageModifier_,
               float sizeDamageModifier_, float projectileSpeed_,
               float projectileEvasion_, float projectileRetargetRange_,
               float projectileHull_, float projectileArmour_,
               Cost const &cost_) noexcept
    : Named(name_),
      size(size_),
      tag(tag_),
      power(power_),
      minDamage(minDamage_),
      maxDamage(maxDamage_),
      minRange(minRange_),
      maxRange(maxRange_),
      tracking(tracking_),
      accuracy(accuracy_),
      cooldown(cooldown_),
      shieldDamageModifier(shieldDamageModifier_),
      shieldSkipModifier(shieldSkipModifier_),
      armourDamageModifier(armourDamageModifier_),
      armourSkipModifier(armourSkipModifier_),
      hullDamageModifier(hullDamageModifier_),
      sizeDamageModifier(sizeDamageModifier_),
      type(Weapon::Type::PROJECTILE),
      data{
          .projectileWeapon{
              .projectileSpeed = projectileSpeed_,
              .projectileEvasion = projectileEvasion_,
              .projectileRetargetRange = projectileRetargetRange_,
              .projectileHull = projectileHull_,
              .projectileArmour = projectileArmour_,
          },
      },
      cost(cost_) {}
Weapon::Weapon(string const &name_, string const &size_, string const &tag_,
               float power_, float minDamage_, float maxDamage_,
               float minRange_, float maxRange_, float tracking_,
               float accuracy_, float cooldown_, float shieldDamageModifier_,
               float shieldSkipModifier_, float armourDamageModifier_,
               float armourSkipModifier_, float hullDamageModifier_,
               float sizeDamageModifier_, float unitsPerHangar_,
               float regenerationPerDay_, float strikeCraftRange_,
               float strikeCraftSpeed_, float strikeCraftEvasion_,
               float strikeCraftShield_, float strikeCraftArmour_,
               float strikeCraftHull_, Cost const &cost_) noexcept
    : Named(name_),
      size(size_),
      tag(tag_),
      power(power_),
      minDamage(minDamage_),
      maxDamage(maxDamage_),
      minRange(minRange_),
      maxRange(maxRange_),
      tracking(tracking_),
      accuracy(accuracy_),
      cooldown(cooldown_),
      shieldDamageModifier(shieldDamageModifier_),
      shieldSkipModifier(shieldSkipModifier_),
      armourDamageModifier(armourDamageModifier_),
      armourSkipModifier(armourSkipModifier_),
      hullDamageModifier(hullDamageModifier_),
      sizeDamageModifier(sizeDamageModifier_),
      type(Weapon::Type::HANGAR),
      data{
          .hangarWeapon{
              .unitsPerHangar = unitsPerHangar_,
              .regenerationPerDay = regenerationPerDay_,
              .strikeCraftRange = strikeCraftRange_,
              .strikeCraftSpeed = strikeCraftSpeed_,
              .strikeCraftEvasion = strikeCraftEvasion_,
              .strikeCraftShield = strikeCraftShield_,
              .strikeCraftArmour = strikeCraftArmour_,
              .strikeCraftHull = strikeCraftHull_,
          },
      },
      cost(cost_) {}
}  // namespace athena2::model::component
