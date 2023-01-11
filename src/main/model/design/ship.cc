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

#include "model/design/ship.h"

#include "util/json.h"

using namespace athena2::util;
using namespace std;
using namespace athena2::model::component;
using namespace nlohmann;

namespace athena2::model::design {
Ship Ship::fromJson(json const &data, ComponentSet const &components,
                    EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  string hullName = checkString(data, "hull", ctx);
  string reactorName = checkString(data, "reactor", ctx);
  string ftlName = checkString(data, "ftl", ctx);
  string sublightName = checkString(data, "ftl", ctx);
  string sensorName = checkString(data, "sensor", ctx);
  string computerName = checkString(data, "computer", ctx);
  optional<string> auraName = checkMaybeString(data, "aura", ctx);
  json const &sectionsData = checkArray(data, "sections", ctx);
  checkFields(data,
              {"name", "hull", "reactor", "ftl", "sublight", "sensor",
               "computer", "aura", "sections"},
              ctx);

  Hull const &hull = [&components, &hullName, &ctx]() {
    auto _ = ctx.push("hull");
    auto hull = components.getHull(hullName);
    if (!hull) ctx.error("no such hull '" + hullName + "'");
    return *hull;
  }();
  Reactor const &reactor = [&components, &reactorName, &ctx]() {
    auto _ = ctx.push("reactor");
    auto reactor = components.getReactor(reactorName);
    if (!reactor) ctx.error("no such reactor '" + reactorName + "'");
    return *reactor;
  }();
  FTL const &ftl = [&components, &ftlName, &ctx]() {
    auto _ = ctx.push("ftl");
    auto ftl = components.getFTL(ftlName);
    if (!ftl) ctx.error("no such ftl '" + ftlName + "'");
    return *ftl;
  }();
  Sublight const &sublight = [&components, &sublightName, &ctx]() {
    auto _ = ctx.push("sublight");
    auto sublight = components.getSublight(sublightName);
    if (!sublight) ctx.error("no such sublight '" + sublightName + "'");
    return *sublight;
  }();
  Sensor const &sensor = [&components, &sensorName, &ctx]() {
    auto _ = ctx.push("sensor");
    auto sensor = components.getSensor(sensorName);
    if (!sensor) ctx.error("no such sensor '" + sensorName + "'");
    return *sensor;
  }();
  Computer const &computer = [&components, &computerName, &ctx]() {
    auto _ = ctx.push("computer");
    auto computer = components.getComputer(computerName);
    if (!computer) ctx.error("no such computer '" + computerName + "'");
    return *computer;
  }();
  optional<reference_wrapper<Aura const>> aura =
      auraName ? ([&components, &auraName,
                   &ctx]() -> optional<reference_wrapper<Aura const>> {
        auto _ = ctx.push("aura");
        auto aura = components.getAura(*auraName);
        if (!aura) ctx.error("no such aura '" + *auraName + "'");
        return *aura;
      }())
               : nullopt;
  vector<Section> sections;
  for (auto const &[key, val] : sectionsData.items()) {
    auto _ = ctx.push(key);
    sections.push_back(Section::fromJson(val, components, ctx));
  }

  try {
    return Ship(name, hull, reactor, ftl, sublight, sensor, computer, aura,
                sections);
  } catch (DesignException const &e) {
    ctx.error("invalid ship design"s + e.what());
  }
}
Ship::Ship(string const &name_, Hull const &hull_, Reactor const &reactor_,
           FTL const &ftl_, Sublight const &sublight_, Sensor const &sensor_,
           Computer const &computer_,
           optional<reference_wrapper<component::Aura const>> const &aura_,
           vector<Section> const &sections_)
    : Named(name_ + "-class " + hull_.name),
      hull(hull_),
      reactor(reactor_),
      ftl(ftl_),
      sublight(sublight_),
      sensor(sensor_),
      computer(computer_),
      aura(aura_),
      sections(sections_),
      power(computePower()) {
  if (find(reactor.sizes.begin(), reactor.sizes.end(), hull.coreSize) ==
      reactor.sizes.end())
    throw DesignException("reactor does not fit hull");

  if (find(sublight.sizes.begin(), sublight.sizes.end(), hull.coreSize) ==
      sublight.sizes.end())
    throw DesignException("sublight does not fit hull");

  if (find(computer.sizes.begin(), computer.sizes.end(), hull.coreSize) ==
      computer.sizes.end())
    throw DesignException("computer does not fit hull");

  if (aura && aura->get().size != hull.coreSize)
    throw DesignException("aura does not fit hull");

  if (power < 0) throw DesignException("insufficient power");
}
float Ship::computePower() const noexcept {
  return reactor.power + ftl.power + sublight.power + sensor.power +
         computer.power +
         accumulate(sections.begin(), sections.end(), 0.0f,
                    [](float acc, Section const &section) {
                      return acc + section.power;
                    });
}
}  // namespace athena2::model::design
