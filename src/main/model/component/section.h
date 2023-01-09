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

#ifndef ATHENA2_MODEL_COMPONENT_SECTION_H_
#define ATHENA2_MODEL_COMPONENT_SECTION_H_

#include <string>

#include "dsl.h"
#include "model/component/component.h"
#include "model/economy.h"
#include "nlohmann/json.hpp"

namespace athena2::model::component {
class Section final : public Component {
 public:
  static Section fromJson(nlohmann::json const &, EvalContext &);

  Section(Section const &) noexcept = delete;
  Section(Section &&) noexcept = default;

  ~Section() noexcept override = default;

  Section &operator=(Section const &) noexcept = delete;
  Section &operator=(Section &&) noexcept = default;

  std::string const size;
  std::string const weaponSlots;
  std::string const utilitySlots;
  Cost const cost;

 private:
  Section(std::string const &name, std::string const &size,
          std::string const &weaponSlots, std::string const &utilitySlots,
          Cost const &cost) noexcept;
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_SECTION_H_
