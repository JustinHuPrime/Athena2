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

#ifndef ATHENA2_MODEL_COMPONENT_COMPONENTSET_H_
#define ATHENA2_MODEL_COMPONENT_COMPONENTSET_H_

#include <vector>

#include "model/component/aura.h"
#include "model/component/auxiliary.h"
#include "model/component/computer.h"
#include "model/component/ftl.h"
#include "model/component/hull.h"
#include "model/component/reactor.h"
#include "model/component/section.h"
#include "model/component/sensor.h"
#include "model/component/sublight.h"
#include "model/component/utility.h"
#include "model/component/weapon.h"

namespace athena2::model::component {
class ComponentSet final {
 public:
  ComponentSet() noexcept = default;
  ComponentSet(ComponentSet const &) = delete;
  ComponentSet(ComponentSet &&) = default;

  ~ComponentSet() noexcept = default;

  ComponentSet &operator=(ComponentSet const &) = delete;
  ComponentSet &operator=(ComponentSet &&) = default;

  std::vector<Hull> hulls;
  std::vector<Section> sections;
  std::vector<Reactor> reactors;
  std::vector<FTL> ftls;
  std::vector<Sublight> sublights;
  std::vector<Sensor> sensors;
  std::vector<Computer> computers;
  std::vector<Aura> auras;
  std::vector<Utility> utilities;
  std::vector<Auxiliary> auxiliaries;
  std::vector<Weapon> weapons;

 private:
};
}  // namespace athena2::model::component

#endif  // ATHENA2_MODEL_COMPONENT_COMPONENTSET_H_
