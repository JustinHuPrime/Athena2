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

#ifndef ATHENA2_MODEL_COMPONENT_REACTOR_H_
#define ATHENA2_MODEL_COMPONENT_REACTOR_H_

#include <string>
#include <vector>

#include "dsl.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"
#include "util/named.h"

namespace athena2::model::component {
class Reactor final : public util::Named {
 public:
  static Reactor fromJson(nlohmann::json const &, EvalContext &);

  Reactor(Reactor const &) noexcept = delete;
  Reactor(Reactor &&) noexcept = default;

  ~Reactor() noexcept override = default;

  Reactor &operator=(Reactor const &) noexcept = delete;
  Reactor &operator=(Reactor &&) noexcept = default;

  std::vector<std::string> const sizes;
  float const power;
  Cost const cost;

 private:
  Reactor(std::string const &name, std::vector<std::string> const &sizes,
          float power, Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_REACTOR_H_
