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

#ifndef ATHENA2_MODEL_DESIGN_FLEET_H_
#define ATHENA2_MODEL_DESIGN_FLEET_H_

#include <string>
#include <utility>
#include <vector>

#include "dsl.h"
#include "model/component/componentSet.h"
#include "model/design/ship.h"
#include "nlohmann/json.hpp"
#include "util/named.h"

namespace athena2::model::design {
class Fleet final : public util::Named {
 public:
  static Fleet fromJson(nlohmann::json const &, component::ComponentSet const &,
                        EvalContext &ctx);

  Fleet(Fleet const &) noexcept = default;
  Fleet(Fleet &&) noexcept = default;

  ~Fleet() noexcept = default;

  Fleet &operator=(Fleet const &) noexcept = default;
  Fleet &operator=(Fleet &&) noexcept = default;

  std::vector<std::pair<Ship, size_t>> const ships;
  float const cost;

 private:
  Fleet(std::string const &name,
        std::vector<std::pair<Ship, size_t>> const &ships) noexcept;
  float computeCost() const noexcept;
};
}  // namespace athena2::model::design

#endif  // ATHENA2_MODEL_DESIGN_FLEET_H_
