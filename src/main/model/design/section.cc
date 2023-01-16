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

#include "model/design/section.h"

#include <algorithm>
#include <numeric>
#include <string>

#include "util/json.h"

using namespace athena2::util;
using namespace std;
using namespace athena2::model::component;
using namespace nlohmann;

namespace athena2::model::design {
Section Section::fromJson(json const &data, ComponentSet const &components,
                          EvalContext &ctx) {
  checkObject(data, ctx);
  string sectionName = checkString(data, "section", ctx);
  vector<string> weaponNames = checkStringArray(data, "weapons", ctx);
  vector<string> utilityNames = checkStringArray(data, "utilities", ctx);
  vector<string> auxiliaryNames = checkStringArray(data, "auxiliaries", ctx);
  checkFields(data, {"section", "weapons", "utilities", "auxiliaries"}, ctx);

  component::Section const &section = [&components, &sectionName,
                                       &ctx]() -> component::Section const & {
    auto _ = ctx.push("section");
    auto section = components.getSection(sectionName);
    if (!section) ctx.error("no such section '" + sectionName + "'");
    return *section;
  }();
  vector<reference_wrapper<Weapon const>> weapons;
  {
    auto _ = ctx.push("weapons");
    for (size_t idx = 0; idx < weaponNames.size(); ++idx) {
      auto _ = ctx.push(to_string(idx));
      auto weapon = components.getWeapon(weaponNames[idx]);
      if (!weapon) ctx.error("no such weapon '" + weaponNames[idx] + "'");
      weapons.push_back(*weapon);
    }
  }
  vector<reference_wrapper<Utility const>> utilities;
  {
    auto _ = ctx.push("utilities");
    for (size_t idx = 0; idx < utilityNames.size(); ++idx) {
      auto _ = ctx.push(to_string(idx));
      auto utility = components.getUtility(utilityNames[idx]);
      if (!utility)
        ctx.error("no such utility component '" + utilityNames[idx] + "'");
      utilities.push_back(*utility);
    }
  }
  vector<reference_wrapper<Auxiliary const>> auxiliaries;
  {
    auto _ = ctx.push("auxiliaries");
    for (size_t idx = 0; idx < auxiliaryNames.size(); ++idx) {
      auto _ = ctx.push(to_string(idx));
      auto auxiliary = components.getAuxiliary(auxiliaryNames[idx]);
      if (!auxiliary)
        ctx.error("no such auxiliary component '" + auxiliaryNames[idx] + "'");
      auxiliaries.push_back(*auxiliary);
    }
  }

  try {
    return Section(section, weapons, utilities, auxiliaries);
  } catch (DesignException const &e) {
    ctx.error("invalid section design: "s + e.what());
  }
}

Section::Section(
    component::Section const &section_,
    vector<reference_wrapper<component::Weapon const>> const &weapons_,
    vector<reference_wrapper<component::Utility const>> const &utilities_,
    vector<reference_wrapper<component::Auxiliary const>> const &auxiliaries_)
    : section(&section_) {
  transform(weapons_.begin(), weapons_.end(), back_inserter(weapons),
            [](component::Weapon const &weapon) { return &weapon; });
  transform(utilities_.begin(), utilities_.end(), back_inserter(utilities),
            [](component::Utility const &utility) { return &utility; });
  transform(auxiliaries_.begin(), auxiliaries_.end(),
            back_inserter(auxiliaries),
            [](component::Auxiliary const &auxiliary) { return &auxiliary; });

  string weaponSizes = accumulate(weapons.begin(), weapons.end(), ""s,
                                  [](string const &rsf, Weapon const *weapon) {
                                    return rsf + weapon->size;
                                  });
  sort(weaponSizes.begin(), weaponSizes.end());
  string unmatched;
  set_difference(weaponSizes.begin(), weaponSizes.end(),
                 section->weaponSlots.begin(), section->weaponSlots.end(),
                 back_inserter(unmatched));
  if (!unmatched.empty())
    throw DesignException("weapons requiring missing slots (" + unmatched +
                          ") found in section design");

  string utilitySizes =
      accumulate(utilities.begin(), utilities.end(), ""s,
                 [](string const &rsf, Utility const *utility) {
                   return rsf + utility->size;
                 }) +
      accumulate(auxiliaries.begin(), auxiliaries.end(), ""s,
                 [](string const &rsf, Auxiliary const *auxiliary) {
                   return rsf + auxiliary->size;
                 });
  sort(utilitySizes.begin(), utilitySizes.end());
  set_difference(utilitySizes.begin(), utilitySizes.end(),
                 section->utilitySlots.begin(), section->utilitySlots.end(),
                 back_inserter(unmatched));
  if (!unmatched.empty())
    throw DesignException("utility components requiring missing slots (" +
                          unmatched + ") found in section design");
}
}  // namespace athena2::model::design
