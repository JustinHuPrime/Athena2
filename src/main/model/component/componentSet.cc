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

#include "model/component/componentSet.h"

#include <utility>

using namespace std;

namespace athena2::model::component {
namespace {
template <typename T>
void addChecked(vector<T> &destination, T &&component, EvalContext &ctx) {
  if (find_if(destination.begin(), destination.end(),
              [&component](T const &compared) {
                return compared.name == component.name;
              }) != destination.end())
    ctx.error("duplicate component " + component.name);

  destination.emplace_back(move(component));
}
}  // namespace

ComponentSet &ComponentSet::add(Hull &&hull, EvalContext &ctx) {
  addChecked(hulls, move(hull), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Section &&section, EvalContext &ctx) {
  addChecked(sections, move(section), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Reactor &&reactor, EvalContext &ctx) {
  addChecked(reactors, move(reactor), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(FTL &&ftl, EvalContext &ctx) {
  addChecked(ftls, move(ftl), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Sublight &&sublight, EvalContext &ctx) {
  addChecked(sublights, move(sublight), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Sensor &&sensor, EvalContext &ctx) {
  addChecked(sensors, move(sensor), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Computer &&computer, EvalContext &ctx) {
  addChecked(computers, move(computer), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Aura &&aura, EvalContext &ctx) {
  addChecked(auras, move(aura), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Utility &&utility, EvalContext &ctx) {
  addChecked(utilities, move(utility), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Auxiliary &&auxiliary, EvalContext &ctx) {
  addChecked(auxiliaries, move(auxiliary), ctx);
  return *this;
}
ComponentSet &ComponentSet::add(Weapon &&weapon, EvalContext &ctx) {
  addChecked(weapons, move(weapon), ctx);
  return *this;
}
}  // namespace athena2::model::component
