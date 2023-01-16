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

#include "model/design/ship.h"

#include <algorithm>

#include "util/json.h"

using namespace athena2::util;
using namespace std;
using namespace athena2::model::component;
using namespace nlohmann;

namespace athena2::model::design {
Ship Ship::fromJson(json const &data, ComponentSet const &components,
                    EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  string hullName = checkString(data, "hull", ctx);
  string reactorName = checkString(data, "reactor", ctx);
  string ftlName = checkString(data, "ftl", ctx);
  string sublightName = checkString(data, "sublight", ctx);
  string sensorName = checkString(data, "sensor", ctx);
  string computerName = checkString(data, "computer", ctx);
  optional<string> auraName = checkMaybeString(data, "aura", ctx);
  json const &sectionsData = checkArray(data, "sections", ctx);
  checkFields(data,
              {"name", "hull", "reactor", "ftl", "sublight", "sensor",
               "computer", "aura", "sections"},
              ctx);

  Hull const &hull = [&components, &hullName, &ctx]() -> Hull const & {
    auto _ = ctx.push("hull");
    auto hull = components.getHull(hullName);
    if (!hull) ctx.error("no such hull '" + hullName + "'");
    return *hull;
  }();
  Reactor const &reactor = [&components, &reactorName,
                            &ctx]() -> Reactor const & {
    auto _ = ctx.push("reactor");
    auto reactor = components.getReactor(reactorName);
    if (!reactor) ctx.error("no such reactor '" + reactorName + "'");
    return *reactor;
  }();
  FTL const &ftl = [&components, &ftlName, &ctx]() -> FTL const & {
    auto _ = ctx.push("ftl");
    auto ftl = components.getFTL(ftlName);
    if (!ftl) ctx.error("no such ftl '" + ftlName + "'");
    return *ftl;
  }();
  Sublight const &sublight = [&components, &sublightName,
                              &ctx]() -> Sublight const & {
    auto _ = ctx.push("sublight");
    auto sublight = components.getSublight(sublightName);
    if (!sublight) ctx.error("no such sublight '" + sublightName + "'");
    return *sublight;
  }();
  Sensor const &sensor = [&components, &sensorName, &ctx]() -> Sensor const & {
    auto _ = ctx.push("sensor");
    auto sensor = components.getSensor(sensorName);
    if (!sensor) ctx.error("no such sensor '" + sensorName + "'");
    return *sensor;
  }();
  Computer const &computer = [&components, &computerName,
                              &ctx]() -> Computer const & {
    auto _ = ctx.push("computer");
    auto computer = components.getComputer(computerName);
    if (!computer) ctx.error("no such computer '" + computerName + "'");
    return *computer;
  }();
  Aura const *aura = auraName
                         ? ([&components, &auraName, &ctx]() -> Aura const * {
                             auto _ = ctx.push("aura");
                             auto aura = components.getAura(*auraName);
                             if (!aura)
                               ctx.error("no such aura '" + *auraName + "'");
                             return aura;
                           }())
                         : nullptr;
  vector<Section> sections;
  for (auto const &[key, val] : sectionsData.items()) {
    auto _ = ctx.push(key);
    sections.push_back(Section::fromJson(val, components, ctx));
  }

  try {
    return Ship(name, hull, reactor, ftl, sublight, sensor, computer, aura,
                sections);
  } catch (DesignException const &e) {
    ctx.error("invalid ship design"s + e.what());
  }
}
Ship::Ship(string const &name_, Hull const &hull_, Reactor const &reactor_,
           FTL const &ftl_, Sublight const &sublight_, Sensor const &sensor_,
           Computer const &computer_, component::Aura const *aura_,
           vector<Section> const &sections_)
    : Named(name_ + "-class " + hull_.name),
      hull(&hull_),
      reactor(&reactor_),
      ftl(&ftl_),
      sublight(&sublight_),
      sensor(&sensor_),
      computer(&computer_),
      aura(aura_),
      sections(sections_),
      power(reactor->power + ftl->power + sublight->power + sensor->power +
            computer->power +
            accumulate(
                sections.begin(), sections.end(), 0.0f,
                [](float acc, Section const &section) {
                  return acc +
                         accumulate(section.weapons.begin(),
                                    section.weapons.end(), 0.f,
                                    [](float rsf, Weapon const *weapon) {
                                      return rsf + weapon->power;
                                    }) +
                         accumulate(section.utilities.begin(),
                                    section.utilities.end(), 0.f,
                                    [](float rsf, Utility const *utility) {
                                      return rsf + utility->power;
                                    }) +
                         accumulate(section.auxiliaries.begin(),
                                    section.auxiliaries.end(), 0.f,
                                    [](float rsf, Auxiliary const *auxiliary) {
                                      return rsf + auxiliary->power;
                                    });
                })),
      cost(hull->includeComponentCost
               ? hull->cost + reactor->cost + ftl->cost + sublight->cost +
                     sensor->cost + computer->cost +
                     accumulate(
                         sections.begin(), sections.end(), 0.f,
                         [](float rsf, Section const &section) {
                           return rsf +
                                  accumulate(
                                      section.weapons.begin(),
                                      section.weapons.end(), 0.f,
                                      [](float rsf, Weapon const *weapon) {
                                        return rsf + weapon->cost;
                                      }) +
                                  accumulate(
                                      section.utilities.begin(),
                                      section.utilities.end(), 0.f,
                                      [](float rsf, Utility const *utility) {
                                        return rsf + utility->cost;
                                      }) +
                                  accumulate(section.auxiliaries.begin(),
                                             section.auxiliaries.end(), 0.f,
                                             [](float rsf,
                                                Auxiliary const *auxiliary) {
                                               return rsf + auxiliary->cost;
                                             });
                         })
               : hull->cost),
      speed(hull->speed *
            (1.f + sublight->sublightSpeedModifier +
             computer->sublightSpeedModifier +
             accumulate(sections.begin(), sections.end(), 0.f,
                        [](float rsf, Section const &section) {
                          return rsf +
                                 accumulate(
                                     section.auxiliaries.begin(),
                                     section.auxiliaries.end(), 0.f,
                                     [](float rsf, Auxiliary const *auxiliary) {
                                       return rsf +
                                              auxiliary->sublightSpeedModifier;
                                     });
                        }))),
      evasion(
          (hull->evasion + sublight->evasionBonus) *
          (1.f + hull->evasionModifier + computer->evasionModifier +
           accumulate(sections.begin(), sections.end(), 0.f,
                      [](float rsf, Section const &section) {
                        return rsf +
                               accumulate(
                                   section.auxiliaries.begin(),
                                   section.auxiliaries.end(), 0.f,
                                   [](float rsf, Auxiliary const *auxiliary) {
                                     return rsf + auxiliary->evasionModifier;
                                   });
                      }))),
      hullHealth(
          (hull->hullHealth +
           accumulate(sections.begin(), sections.end(), 0.f,
                      [](float rsf, Section const &section) {
                        return rsf + accumulate(
                                         section.utilities.begin(),
                                         section.utilities.end(), 0.f,
                                         [](float rsf, Utility const *utility) {
                                           return rsf + utility->hullHealth;
                                         });
                      })) *
          (1.f + hull->hullHealthModifier)),
      hullRegen(
          hullHealth *
          accumulate(sections.begin(), sections.end(), 0.f,
                     [](float rsf, Section const &section) {
                       return rsf +
                              accumulate(
                                  section.auxiliaries.begin(),
                                  section.auxiliaries.end(), 0.f,
                                  [](float rsf, Auxiliary const *auxiliary) {
                                    return rsf + auxiliary->hullRegenModifier;
                                  });
                     })),
      armourHealth(
          hull->armourHealth +
          accumulate(sections.begin(), sections.end(), 0.f,
                     [](float rsf, Section const &section) {
                       return rsf +
                              accumulate(section.utilities.begin(),
                                         section.utilities.end(), 0.f,
                                         [](float rsf, Utility const *utility) {
                                           return rsf + utility->armourHealth;
                                         });
                     })),
      armourRegen(
          armourHealth *
          accumulate(sections.begin(), sections.end(), 0.f,
                     [](float rsf, Section const &section) {
                       return rsf +
                              accumulate(
                                  section.auxiliaries.begin(),
                                  section.auxiliaries.end(), 0.f,
                                  [](float rsf, Auxiliary const *auxiliary) {
                                    return rsf + auxiliary->armourRegenModifier;
                                  });
                     })),
      armourHardening(accumulate(
          sections.begin(), sections.end(), 0.f,
          [](float rsf, Section const &section) {
            return rsf + accumulate(section.auxiliaries.begin(),
                                    section.auxiliaries.end(), 0.f,
                                    [](float rsf, Auxiliary const *auxiliary) {
                                      return rsf +
                                             auxiliary->armourHardeningBonus;
                                    });
          })),
      shieldHealth(
          accumulate(sections.begin(), sections.end(), 0.f,
                     [](float rsf, Section const &section) {
                       return rsf +
                              accumulate(section.utilities.begin(),
                                         section.utilities.end(), 0.f,
                                         [](float rsf, Utility const *utility) {
                                           return rsf + utility->shieldHealth;
                                         });
                     }) *
          (1.f +
           accumulate(sections.begin(), sections.end(), 0.f,
                      [](float rsf, Section const &section) {
                        return rsf +
                               accumulate(
                                   section.auxiliaries.begin(),
                                   section.auxiliaries.end(), 0.f,
                                   [](float rsf, Auxiliary const *auxiliary) {
                                     return rsf +
                                            auxiliary->shieldHealthModifier;
                                   });
                      }))),
      shieldRegen(accumulate(
          sections.begin(), sections.end(), 0.f,
          [](float rsf, Section const &section) {
            return rsf + accumulate(section.utilities.begin(),
                                    section.utilities.end(), 0.f,
                                    [](float rsf, Utility const *utility) {
                                      return rsf + utility->shieldRegen;
                                    });
          })),
      shieldHardening(accumulate(
          sections.begin(), sections.end(), 0.f,
          [](float rsf, Section const &section) {
            return rsf + accumulate(section.auxiliaries.begin(),
                                    section.auxiliaries.end(), 0.f,
                                    [](float rsf, Auxiliary const *auxiliary) {
                                      return rsf +
                                             auxiliary->shieldHardeningBonus;
                                    });
          })),
      disengageChances(
          ftl->disengageChances +
          accumulate(sections.begin(), sections.end(), 0.f,
                     [](float rsf, Section const &section) {
                       return rsf +
                              accumulate(
                                  section.auxiliaries.begin(),
                                  section.auxiliaries.end(), 0.f,
                                  [](float rsf, Auxiliary const *auxiliary) {
                                    return rsf +
                                           auxiliary->disengageChancesBonus;
                                  });
                     })),
      disengageChanceModifier(hull->disengageChanceModifier),
      chanceToHitBonus(
          computer->chanceToHitBonus +
          accumulate(sections.begin(), sections.end(), 0.f,
                     [](float rsf, Section const &section) {
                       return rsf +
                              accumulate(
                                  section.auxiliaries.begin(),
                                  section.auxiliaries.end(), 0.f,
                                  [](float rsf, Auxiliary const *auxiliary) {
                                    return rsf + auxiliary->chanceToHitBonus;
                                  });
                     })),
      trackingBonus(
          computer->trackingBonus + sensor->trackingBonus +
          accumulate(sections.begin(), sections.end(), 0.f,
                     [](float rsf, Section const &section) {
                       return rsf +
                              accumulate(
                                  section.auxiliaries.begin(),
                                  section.auxiliaries.end(), 0.f,
                                  [](float rsf, Auxiliary const *auxiliary) {
                                    return rsf + auxiliary->trackingBonus;
                                  });
                     })),
      trackingModifier(hull->trackingModifier),
      fireRateModifier(computer->fireRateModifier),
      explosiveWeaponsDamageModifier(computer->explosiveWeaponsDamageModifier),
      weaponsRangeModifier(computer->weaponsRangeModifier),
      engagementRangeModifier(computer->engagementRangeModifier),
      preferredRange([&computer_, &sections_]() {
        switch (computer_.tactics) {
          case Computer::Tactics::SWARM:
          case Computer::Tactics::TORPEDO: {
            return 0.f;
          }
          case Computer::Tactics::PICKET:
          case Computer::Tactics::LINE: {
            vector<float> averageRanges;
            for (Section const &section : sections_) {
              for (Weapon const *weapon : section.weapons) {
                averageRanges.push_back((weapon->minRange + weapon->maxRange) /
                                        2.f);
              }
            }
            if (averageRanges.empty()) {
              return 0.f;
            }
            sort(averageRanges.begin(), averageRanges.end());
            return averageRanges[averageRanges.size() / 2];
          }
          case Computer::Tactics::ARTILLERY:
          case Computer::Tactics::CARRIER: {
            vector<float> averageRanges;
            for (Section const &section : sections_) {
              for (Weapon const *weapon : section.weapons) {
                averageRanges.push_back((weapon->minRange + weapon->maxRange) /
                                        2.f);
              }
            }
            if (averageRanges.empty()) {
              return 0.f;
            }
            sort(averageRanges.begin(), averageRanges.end());
            return averageRanges.back();
          }
          default: {
            abort();  // invalid enum
          }
        }
      }()) {
  vector<string> sectionSizes;
  transform(sections.begin(), sections.end(), back_inserter(sectionSizes),
            [](Section const &section) { return section.section->size; });
  sort(sectionSizes.begin(), sectionSizes.end());
  vector<string> unmatched;
  set_difference(sectionSizes.begin(), sectionSizes.end(),
                 hull->sectionSizes.begin(), hull->sectionSizes.end(),
                 back_inserter(unmatched));
  if (!unmatched.empty())
    throw DesignException(to_string(unmatched.size()) +
                          "sections requriring missing slots");

  if (find(reactor->sizes.begin(), reactor->sizes.end(), hull->coreSize) ==
      reactor->sizes.end())
    throw DesignException("reactor does not fit hull");

  if (find(sublight->sizes.begin(), sublight->sizes.end(), hull->coreSize) ==
      sublight->sizes.end())
    throw DesignException("sublight does not fit hull");

  if (find(computer->sizes.begin(), computer->sizes.end(), hull->coreSize) ==
      computer->sizes.end())
    throw DesignException("computer does not fit hull");

  if (aura && aura->size != hull->coreSize)
    throw DesignException("aura does not fit hull");

  if (power < 0) throw DesignException("insufficient power");
}
}  // namespace athena2::model::design
