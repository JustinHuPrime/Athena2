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

#include "model/entity/fleet.h"

using namespace athena2::model::component;
using namespace athena2::model::design;
using namespace athena2::model::entity;
using namespace nlohmann;

namespace athena2::model::entity {
Fleet::Fleet(design::Fleet const &design, float position) noexcept {
  for (auto const &[ship, count] : design.ships) {
    for (size_t idx = 0; idx < count; ++idx) {
      ships.emplace_back(ship, position);
    }
  }
}
void to_json(json &j, Fleet const &f) noexcept {
  j = json{{"ships", static_cast<json>(f.ships)},
           {"projectiles", static_cast<json>(f.projectiles)},
           {"strikeCraft", static_cast<json>(f.strikeCraft)},
           {"destroyed", static_cast<json>(f.destroyed)},
           {"disengaged", static_cast<json>(f.disengaged)}};
}
}  // namespace athena2::model::entity
