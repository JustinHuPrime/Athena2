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

#ifndef ATHENA2_MODEL_COMPONENT_SUBLIGHT_H_
#define ATHENA2_MODEL_COMPONENT_SUBLIGHT_H_

#include <string>
#include <vector>

#include "dsl.h"
#include "model/component/component.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"

namespace athena2::model::component {
class Sublight final : public Component {
 public:
  static Sublight fromJson(nlohmann::json const &, EvalContext &);

  std::vector<std::string> const sizes;
  float const power;
  float const speedModifier;
  float const evasionBonus;
  Cost const cost;

 private:
  Sublight(std::string const &name, std::vector<std::string> const &sizes,
           float power, float speedModifier, float evasionBonus,
           Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_SUBLIGHT_H_
