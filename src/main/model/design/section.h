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

#ifndef ATHENA2_MODEL_DESIGN_SECTION_H_
#define ATHENA2_MODEL_DESIGN_SECTION_H_

#include <utility>
#include <vector>

#include "dsl.h"
#include "model/component/auxiliary.h"
#include "model/component/componentSet.h"
#include "model/component/section.h"
#include "model/component/utility.h"
#include "model/component/weapon.h"
#include "nlohmann/json.hpp"

namespace athena2::model::design {
class Section final {
 public:
  static Section fromJson(nlohmann::json const &,
                          component::ComponentSet const &, EvalContext &ctx);

  Section(Section const &) noexcept = default;
  Section(Section &&) noexcept = default;

  ~Section() noexcept = default;

  Section &operator=(Section const &) noexcept = default;
  Section &operator=(Section &&) noexcept = default;

  component::Section const &section;
  std::vector<std::reference_wrapper<component::Weapon const>> const weapons;
  std::vector<std::reference_wrapper<component::Utility const>> const utilities;
  std::vector<std::reference_wrapper<component::Auxiliary const>> const
      auxiliaries;
  float const power;
  float const cost;

 private:
  /**
   * @throws DesignException if section design is invalid (e.g. weapon doesn't
   * have a slot to go in)
   */
  Section(
      component::Section const &,
      std::vector<std::reference_wrapper<component::Weapon const>> const &,
      std::vector<std::reference_wrapper<component::Utility const>> const &,
      std::vector<std::reference_wrapper<component::Auxiliary const>> const &);
  float computePower() const noexcept;
  float computeCost() const noexcept;
};
}  // namespace athena2::model::design

#endif  // ATHENA2_MODEL_DESIGN_SHIP_H_
