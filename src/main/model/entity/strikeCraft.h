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

#ifndef ATHENA2_MODEL_ENTITY_STRIKECRAFT_H_
#define ATHENA2_MODEL_ENTITY_STRIKECRAFT_H_

#include "model/component/weapon.h"
#include "model/design/ship.h"
#include "model/entity/entity.h"
#include "model/entity/ship.h"

namespace athena2::model::entity {
class StrikeCraft final : public Entity {
 public:
  StrikeCraft(component::Weapon const &, entity::Ship const &) noexcept;

  StrikeCraft(StrikeCraft const &) noexcept = default;
  StrikeCraft(StrikeCraft &&) noexcept = default;

  ~StrikeCraft() noexcept = default;

  StrikeCraft &operator=(StrikeCraft const &) noexcept = default;
  StrikeCraft &operator=(StrikeCraft &&) noexcept = default;

  component::Weapon const &weapon;
  design::Ship const &ship;
};
}  // namespace athena2::model::entity

#endif  // ATHENA2_MODEL_ENTITY_STRIKECRAFT_H_
