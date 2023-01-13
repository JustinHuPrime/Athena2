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

#ifndef ATHENA2_MODEL_ENTITY_ENTITY_H_
#define ATHENA2_MODEL_ENTITY_ENTITY_H_

namespace athena2::model::entity {
class Entity {
 public:
  Entity(float hull, float armour, float shields, float position) noexcept;
  Entity(Entity const &) noexcept = default;
  Entity(Entity &&) noexcept = default;

  ~Entity() noexcept = default;

  Entity &operator=(Entity const &) noexcept = default;
  Entity &operator=(Entity &&) noexcept = default;

  float hull;
  float armour;
  float shields;
  float position;
};
}  // namespace athena2::model::entity

#endif  // ATHENA2_MODEL_ENTITY_ENTITY_H_
