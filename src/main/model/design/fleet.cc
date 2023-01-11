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

#include "model/design/fleet.h"

#include "util/json.h"

using namespace std;
using namespace athena2::model::component;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::design {
Fleet Fleet::fromJson(json const &data, ComponentSet const &components,
                      EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  json const &shipsData = checkArray(data, "ships", ctx);
  checkFields(data, {"name", "ships"}, ctx);

  vector<pair<Ship, size_t>> ships;
  for (auto const &[key, val] : shipsData.items()) {
    auto _ = ctx.push(key);
    checkObject(val, ctx);
    size_t count = checkUnsignedInteger(val, "count", ctx);
    json const &ship = checkField(val, "ship", ctx);
    ships.emplace_back(
        [&ship, &components, &ctx]() {
          auto _ = ctx.push("ship");
          return Ship::fromJson(ship, components, ctx);
        }(),
        count);
  }

  return Fleet(name, ships);
}

Fleet::Fleet(std::string const &name_,
             vector<pair<Ship, size_t>> const &ships_) noexcept
    : Named(name_), ships(ships_), cost(computeCost()) {}
float Fleet::computeCost() const noexcept {
  return accumulate(ships.begin(), ships.end(), 0.f,
                    [](float rsf, pair<Ship, size_t> const &element) {
                      return rsf + element.first.cost * element.second;
                    });
}
}  // namespace athena2::model::design
