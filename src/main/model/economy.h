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

#ifndef ATHENA2_MODEL_ECONOMY_H_
#define ATHENA2_MODEL_ECONOMY_H_

#include <string>

#include "dsl.h"

namespace athena2::model {
class Cost final {
 public:
  static Cost fromLine(std::string const &, EvalContext &);

  Cost(Cost const &) noexcept = default;
  Cost(Cost &&) noexcept = default;

  ~Cost() noexcept = default;

  Cost &operator=(Cost const &) noexcept = default;
  Cost &operator=(Cost &&) noexcept = default;

  operator std::string() const noexcept;
  operator float() const noexcept;

  float const alloys;
  float const minerals;
  float const darkMatter;
  float const strategicResources;
  float const zro;
  float const nanites;

 private:
  Cost(float alloys, float minerals, float darkMatter, float strategicResources,
       float zro, float nanites) noexcept;
};
}  // namespace athena2::model

#endif  // ATHENA2_MODEL_ECONOMY_H_
