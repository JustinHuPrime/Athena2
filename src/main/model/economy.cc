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

#include "model/economy.h"

#include <optional>
#include <unordered_map>
#include <vector>

#include "util/json.h"

using namespace std;
using namespace athena2::util;
using namespace nlohmann;

namespace athena2::model {
Cost Cost::fromJson(json const &data, EvalContext &ctx) {
  optional<float> alloys = checkMaybeFloat(data, "alloys", ctx);
  optional<float> minerals = checkMaybeFloat(data, "minerals", ctx);
  optional<float> darkMatter = checkMaybeFloat(data, "dark matter", ctx);
  optional<float> strategicResources =
      checkMaybeFloat(data, "strategic resources", ctx);
  optional<float> zro = checkMaybeFloat(data, "zro", ctx);
  optional<float> nanites = checkMaybeFloat(data, "nanites", ctx);

  return Cost(alloys.value_or(0.f), minerals.value_or(0.f),
              darkMatter.value_or(0.f), strategicResources.value_or(0.f),
              zro.value_or(0.f), nanites.value_or(0.f));
}

Cost::Cost(float alloys_, float minerals_, float darkMatter_,
           float strategicResources_, float zro_, float nanites_) noexcept
    : alloys(alloys_),
      minerals(minerals_),
      darkMatter(darkMatter_),
      strategicResources(strategicResources_),
      zro(zro_),
      nanites(nanites_) {}
Cost::Cost() noexcept : Cost(0.f, 0.f, 0.f, 0.f, 0.f, 0.f) {}
Cost::operator string() const noexcept {
  return to_string(operator float()) + " mineral-equivalents";
}
Cost::operator float() const noexcept {
  return alloys * 2.0f + minerals * 1.0f + darkMatter * 20.0f +
         strategicResources * 5.0f + zro * 20.0f + nanites * 40.0f;
}
}  // namespace athena2::model
