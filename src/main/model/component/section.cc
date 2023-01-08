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

#include "model/component/section.h"

#include <algorithm>

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
Section Section::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  string size = checkString(data, "size", ctx);
  string weaponSlots = checkString(data, "weaponSlots", ctx);
  sort(weaponSlots.begin(), weaponSlots.end());
  string utilitySlots = checkString(data, "utilitySlots", ctx);
  sort(utilitySlots.begin(), utilitySlots.end());
  optional<reference_wrapper<json const>> costData =
      checkMaybeObject(data, "cost", ctx);
  Cost cost = costData ? [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(*costData, ctx);
  }() : Cost();
  return Section(name, size, weaponSlots, utilitySlots, cost);
}
Section::Section(string const &name, string const &size,
                 string const &weaponSlots, string const &utilitySlots,
                 Cost const &cost) noexcept
    : Component(name),
      size(size),
      weaponSlots(weaponSlots),
      utilitySlots(utilitySlots),
      cost(cost) {}
}  // namespace athena2::model::component
