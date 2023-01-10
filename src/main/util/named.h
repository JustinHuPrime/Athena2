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

#ifndef ATHENA2_UTIL_NAMED_H_
#define ATHENA2_UTIL_NAMED_H_

#include <string>

namespace athena2::util {
class Named {
 public:
  Named(Named const &) noexcept = default;
  Named(Named &&) noexcept = default;

  virtual ~Named() noexcept = default;

  Named &operator=(Named const &) noexcept = default;
  Named &operator=(Named &&) noexcept = default;

  operator std::string() const noexcept;

  std::string const name;

 protected:
  explicit Named(std::string const &name) noexcept;
};
}  // namespace athena2::util

#endif  // ATHENA2_UTIL_NAMED_H_
