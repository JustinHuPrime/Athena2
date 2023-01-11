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
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <utility>

#include "model/component/aura.h"
#include "model/component/auxiliary.h"
#include "model/component/componentSet.h"
#include "model/component/computer.h"
#include "model/component/ftl.h"
#include "model/component/hull.h"
#include "model/component/reactor.h"
#include "model/component/section.h"
#include "model/component/sensor.h"
#include "model/component/sublight.h"
#include "model/component/utility.h"
#include "model/component/weapon.h"
#include "model/design/fleet.h"
#include "model/evaluator.h"
#include "nlohmann/json.hpp"
#include "util/json.h"
#include "version.h"

using namespace std;
using namespace athena2::model::design;
using namespace athena2::model::component;
using namespace nlohmann;
using namespace athena2::util;
using namespace athena2::model;

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

template <typename T>
void loadComponentsOfType(
    ComponentSet &components, json const &load, string const &key,
    function<T(json const &, EvalContext &)> const &fromJson,
    EvalContext &ctx) {
  vector<string> componentFiles = checkStringArray(load, key, ctx);
  auto _ = ctx.push(key);
  for_each(componentFiles.begin(), componentFiles.end(),
           [&ctx, &components, &fromJson](string const &filename) {
             auto _ = ctx.push(filename);
             ifstream file(filename);
             if (!file) ctx.error("could not open file");
             components.add(fromJson(parse(file, ctx), ctx), ctx);
           });
}
ComponentSet loadComponents(json const &load, EvalContext &ctx) {
  ComponentSet components;
  loadComponentsOfType<Hull>(components, load, "hulls", Hull::fromJson, ctx);
  loadComponentsOfType<athena2::model::component::Section>(
      components, load, "sections",
      athena2::model::component::Section::fromJson, ctx);
  loadComponentsOfType<Reactor>(components, load, "reactors", Reactor::fromJson,
                                ctx);
  loadComponentsOfType<FTL>(components, load, "ftls", FTL::fromJson, ctx);
  loadComponentsOfType<Sublight>(components, load, "sublights",
                                 Sublight::fromJson, ctx);
  loadComponentsOfType<Sensor>(components, load, "sensors", Sensor::fromJson,
                               ctx);
  loadComponentsOfType<Computer>(components, load, "computers",
                                 Computer::fromJson, ctx);
  loadComponentsOfType<Aura>(components, load, "auras", Aura::fromJson, ctx);
  loadComponentsOfType<Utility>(components, load, "utilities",
                                Utility::fromJson, ctx);
  loadComponentsOfType<Auxiliary>(components, load, "auxiliaries",
                                  Auxiliary::fromJson, ctx);
  loadComponentsOfType<Weapon>(components, load, "weapons", Weapon::fromJson,
                               ctx);
  checkFields(load,
              {"hulls", "sections", "reactors", "ftls", "sublights", "sensors",
               "computers", "auras", "utilities", "auxiliaries", "weapons"},
              ctx);
  return components;
}
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

DesignException::DesignException(string const &message_) noexcept
    : message(message_) {}
char const *DesignException::what() const noexcept { return message.c_str(); }

int eval(istream &in, EvalContext &ctx) noexcept {
  try {
    cout << "Athena II version " << ATHENA2_VERSION_MAJOR << "."
         << ATHENA2_VERSION_MINOR << "." << ATHENA2_VERSION_PATCH << "\n";

    if (!in) ctx.error("could not open file");

    json runspec = parse(in, ctx);
    checkObject(runspec, ctx);

    // load section
    Header loadHeader = Header("Loading components...");
    json const &load = checkObject(runspec, "load", ctx);
    ComponentSet components = [&ctx, &load]() {
      auto _ = ctx.push("load");
      return loadComponents(load, ctx);
    }();
    loadHeader.finish();

    // generic hyperparams
    Header paramsHeader = Header("Loading settings...");
    float fightLengthLimit =
        checkMaybeFloat(runspec, "fightLengthLimit", ctx).value_or(360.f);
    float withdrawMultiplier =
        checkMaybeFloat(runspec, "withdrawMultiplier", ctx).value_or(0.1f);
    EvaluationSettings evaluationSettings = {
        .fightLengthLimit = fightLengthLimit,
        .withdrawMultiplier = withdrawMultiplier,
    };
    string mode = checkString(runspec, "mode", ctx);
    paramsHeader.finish();

    if (mode == "auto") {
      // auto - do AI-based design

      // TODO
      cerr << "Auto mode not yet implemented!\n";
      abort();

      checkFields(runspec,
                  {"load", "mode", "fightLengthLimit", "withdrawMultiplier"},
                  ctx);
    } else if (mode == "manual") {
      // manual mode - read fleets and simulate combat
      Header loadFleetHeader = Header("Loading fleets...");
      json const &fleetData = checkArray(runspec, "fleets", ctx);
      checkFields(
          runspec,
          {"load", "mode", "fightLengthLimit", "withdrawMultiplier", "fleets"},
          ctx);

      vector<Fleet> fleets;
      for (auto const &[key, val] : fleetData.items()) {
        auto _ = ctx.push(key);
        fleets.push_back(Fleet::fromJson(val, components, ctx));
      }
      loadFleetHeader.finish();

      cout << "\n"
           << "Results\n"
           << "\n";
      for (size_t firstIdx = 0; firstIdx < fleets.size(); ++firstIdx) {
        for (size_t secondIdx = firstIdx + 1; secondIdx < fleets.size();
             ++secondIdx) {
          auto const &first = fleets[firstIdx];
          auto const &second = fleets[secondIdx];
          cout << first.name << " vs " << second.name << ": ";
          pair<float, float> result =
              evaluate(first, second, evaluationSettings);
          cout << first.name << ": " << -result.first << ", " << second.name
               << ": " << -result.second;
          // TODO: add out of cost (e.g. `${-result.first}/${first.cost}`)
          if (result.first < result.second) {
            cout << "; " << first.name << " wins by "
                 << result.second - result.first << "\n";
          } else if (result.second < result.first) {
            cout << "; " << second.name << " wins by "
                 << result.first - result.second << "\n";
          } else {
            cout << "; draw\n";
          }
        }
      }
    }
  } catch (EvalException const &e) {
    cerr << e.what() << endl;
    return 1;
  }

  return 0;
}
}  // namespace athena2
