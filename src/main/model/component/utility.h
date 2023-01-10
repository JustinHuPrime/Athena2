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

#ifndef ATHENA2_MODEL_COMPONENT_UTILITY_H_
#define ATHENA2_MODEL_COMPONENT_UTILITY_H_

#include <string>

#include "dsl.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"
#include "util/named.h"

namespace athena2::model::component {
class Utility final : public util::Named {
 public:
  static Utility fromJson(nlohmann::json const &, EvalContext &);

  Utility(Utility const &) noexcept = delete;
  Utility(Utility &&) noexcept = default;

  ~Utility() noexcept override = default;

  Utility &operator=(Utility const &) noexcept = delete;
  Utility &operator=(Utility &&) noexcept = default;

  std::string const size;
  float const power;
  float const shieldHealth;
  float const shieldRegen;
  float const armourHealth;
  float const hullHealth;
  Cost const cost;

 private:
  Utility(std::string const &name, std::string const &size, float power,
          float shieldHealth, float shieldRegen, float armourHealth,
          float hullHealth, Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_UTILITY_H_
