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

#ifndef ATHENA2_DSL_H_
#define ATHENA2_DSL_H_

#include <functional>
#include <istream>
#include <string>
#include <vector>

namespace athena2 {
class EvalContext final {
 public:
  class ScopeGuard final {
   public:
    explicit ScopeGuard(std::function<void()> const &f) noexcept;
    ScopeGuard(ScopeGuard const &) noexcept = delete;
    ScopeGuard(ScopeGuard &&) noexcept = default;

    ~ScopeGuard() noexcept;

    ScopeGuard &operator=(ScopeGuard const &) noexcept = delete;
    ScopeGuard &operator=(ScopeGuard &&) noexcept = default;

    void reset() noexcept;

   private:
    std::function<void()> f;
  };

  explicit EvalContext(std::string const &root) noexcept;
  EvalContext(EvalContext const &) noexcept = default;
  EvalContext(EvalContext &&) noexcept = default;

  ~EvalContext() noexcept = default;

  EvalContext &operator=(EvalContext const &) noexcept = default;
  EvalContext &operator=(EvalContext &&) noexcept = default;

  [[noreturn]] void error(std::string const &message);

  ScopeGuard push(std::string const &name) noexcept;

 private:
  std::vector<std::string> contextStack;
};

class EvalException final : public std::exception {
 public:
  explicit EvalException(std::string const &) noexcept;
  EvalException(EvalException const &) noexcept = default;
  EvalException(EvalException &&) noexcept = default;

  ~EvalException() noexcept override = default;

  EvalException &operator=(EvalException const &) noexcept = default;
  EvalException &operator=(EvalException &&) noexcept = default;

  char const *what() const noexcept override;

 private:
  std::string message;
};

int eval(std::istream &, EvalContext &) noexcept;
}  // namespace athena2

#endif  // ATHENA2_DSL_H_
