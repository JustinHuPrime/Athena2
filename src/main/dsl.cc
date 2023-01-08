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
#include "nlohmann/json.hpp"
#include "util/json.h"
#include "version.h"

using namespace std;
using namespace athena2::model::component;
using namespace nlohmann;
using namespace athena2::util;

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
EvalContext::ScopeGuard::~ScopeGuard() noexcept {
  if (f) f();
}
void EvalContext::ScopeGuard::reset() noexcept {
  if (f) f();
  f = function<void()>();
}

EvalException::EvalException(string const &message_) noexcept
    : message(message_) {}
char const *EvalException::what() const noexcept { return message.c_str(); }

int eval(istream &in, EvalContext &ctx) noexcept {
  try {
    cout << "Athena II version " << ATHENA2_VERSION_MAJOR << "."
         << ATHENA2_VERSION_MINOR << "." << ATHENA2_VERSION_PATCH << "\n";

    if (!in) ctx.error("could not open file");

    json runspec = parse(in, ctx);
    checkObject(runspec, ctx);

    // load section
    cout << "Loading components...\n";
    ComponentSet components;
    json const &load = checkObject(runspec, "load", ctx);
    {
      auto _ = ctx.push("load");
      vector<string> hulls = checkStringArray(load, "hulls", ctx);
      for_each(
          hulls.begin(), hulls.end(),
          [&ctx, &components](string const &filename) {
            auto _ = ctx.push(filename);
            ifstream file(filename);
            if (!file) ctx.error("could not open file");
            components.hulls.push_back(Hull::fromJson(parse(file, ctx), ctx));
          });
    }
    cout << "\x1b[A\x1b[22Cdone!\n";
  } catch (EvalException const &e) {
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}
}  // namespace athena2
