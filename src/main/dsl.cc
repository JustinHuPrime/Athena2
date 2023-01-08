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

#include "dsl.h"

#include <fstream>
#include <iostream>
#include <numeric>

#include "model/component/componentSet.h"
#include "model/component/hull.h"
#include "util/string.h"

using namespace std;
using namespace athena2::util;
using namespace athena2::model::component;

namespace athena2 {
EvalContext::EvalContext(string const &root) noexcept : contextStack(1, root) {}
[[noreturn]] void EvalContext::error(string const &message) {
  string accumulator = "Error: ";
  string context = accumulate(
      contextStack.begin(), contextStack.end(), ""s,
      [](string const &a, string const &b) { return a + " > " + b; });
  accumulator += context.substr(3);
  accumulator += ": ";
  accumulator += message;
  throw EvalException(accumulator);
}
EvalContext::ScopeGuard EvalContext::push(string const &name) noexcept {
  contextStack.push_back(name);
  return EvalContext::ScopeGuard([this] { contextStack.pop_back(); });
}

EvalContext::ScopeGuard::ScopeGuard(function<void()> const &f_) noexcept
    : f(f_) {}
EvalContext::ScopeGuard::~ScopeGuard() noexcept { f(); }

EvalException::EvalException(string const &message_) noexcept
    : message(message_) {}
char const *EvalException::what() const noexcept { return message.c_str(); }

int eval(istream &in, EvalContext &context) noexcept {
  try {
    // TODO
  } catch (EvalException const &e) {
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}
}  // namespace athena2
