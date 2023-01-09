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

#include "model/component/aura.h"
#include "model/component/componentSet.h"
#include "model/component/computer.h"
#include "model/component/ftl.h"
#include "model/component/hull.h"
#include "model/component/reactor.h"
#include "model/component/section.h"
#include "model/component/sensor.h"
#include "model/component/sublight.h"
#include "model/component/utility.h"
#include "nlohmann/json.hpp"
#include "util/json.h"
#include "version.h"

using namespace std;
using namespace athena2::model::component;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2 {
namespace {
class Header final {
 public:
  Header(std::string const &header) noexcept : finished(false) {
    cout << header;
  }
  Header(Header const &) noexcept = delete;
  Header(Header &&other) noexcept = delete;

  ~Header() noexcept {
    if (!finished) cout << "\n";
  }

  Header &operator=(Header const &) noexcept = delete;
  Header &operator=(Header &&other) noexcept = delete;

  void finish() noexcept {
    cout << " done!\n";
    finished = true;
  }

 private:
  bool finished;
};
}  // namespace

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
    Header loadHeader = Header("Loading components...");
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
      vector<string> sections = checkStringArray(load, "sections", ctx);
      for_each(sections.begin(), sections.end(),
               [&ctx, &components](string const &filename) {
                 auto _ = ctx.push(filename);
                 ifstream file(filename);
                 if (!file) ctx.error("could not open file");
                 components.sections.push_back(
                     Section::fromJson(parse(file, ctx), ctx));
               });
      vector<string> reactors = checkStringArray(load, "reactors", ctx);
      for_each(reactors.begin(), reactors.end(),
               [&ctx, &components](string const &filename) {
                 auto _ = ctx.push(filename);
                 ifstream file(filename);
                 if (!file) ctx.error("could not open file");
                 components.reactors.push_back(
                     Reactor::fromJson(parse(file, ctx), ctx));
               });
      vector<string> ftls = checkStringArray(load, "ftls", ctx);
      for_each(
          ftls.begin(), ftls.end(),
          [&ctx, &components](string const &filename) {
            auto _ = ctx.push(filename);
            ifstream file(filename);
            if (!file) ctx.error("could not open file");
            components.ftls.push_back(FTL::fromJson(parse(file, ctx), ctx));
          });
      vector<string> sublights = checkStringArray(load, "sublights", ctx);
      for_each(sublights.begin(), sublights.end(),
               [&ctx, &components](string const &filename) {
                 auto _ = ctx.push(filename);
                 ifstream file(filename);
                 if (!file) ctx.error("could not open file");
                 components.sublights.push_back(
                     Sublight::fromJson(parse(file, ctx), ctx));
               });
      vector<string> sensors = checkStringArray(load, "sensors", ctx);
      for_each(sensors.begin(), sensors.end(),
               [&ctx, &components](string const &filename) {
                 auto _ = ctx.push(filename);
                 ifstream file(filename);
                 if (!file) ctx.error("could not open file");
                 components.sensors.push_back(
                     Sensor::fromJson(parse(file, ctx), ctx));
               });
      vector<string> computers = checkStringArray(load, "computers", ctx);
      for_each(computers.begin(), computers.end(),
               [&ctx, &components](string const &filename) {
                 auto _ = ctx.push(filename);
                 ifstream file(filename);
                 if (!file) ctx.error("could not open file");
                 components.computers.push_back(
                     Computer::fromJson(parse(file, ctx), ctx));
               });
      vector<string> auras = checkStringArray(load, "auras", ctx);
      for_each(
          auras.begin(), auras.end(),
          [&ctx, &components](string const &filename) {
            auto _ = ctx.push(filename);
            ifstream file(filename);
            if (!file) ctx.error("could not open file");
            components.auras.push_back(Aura::fromJson(parse(file, ctx), ctx));
          });
      vector<string> utilities = checkStringArray(load, "utilities", ctx);
      for_each(utilities.begin(), utilities.end(),
               [&ctx, &components](string const &filename) {
                 auto _ = ctx.push(filename);
                 ifstream file(filename);
                 if (!file) ctx.error("could not open file");
                 components.utilities.push_back(
                     Utility::fromJson(parse(file, ctx), ctx));
               });
    }
    loadHeader.finish();
  } catch (EvalException const &e) {
    cout << e.what() << endl;
    return 1;
  }

  return 0;
}
}  // namespace athena2
