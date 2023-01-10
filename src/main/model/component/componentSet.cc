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

#include <string>
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
template <typename T>
optional<reference_wrapper<T const>> getFrom(vector<T> const &from,
                                             string const &name) {
  auto it = find_if(from.begin(), from.end(), [&name](T const &component) {
    return component.name == name;
  });
  if (it == from.end())
    return nullopt;
  else
    return *it;
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
optional<reference_wrapper<Hull const>> ComponentSet::getHull(
    string const &name) const noexcept {
  return getFrom(hulls, name);
}
optional<reference_wrapper<Section const>> ComponentSet::getSection(
    string const &name) const noexcept {
  return getFrom(sections, name);
}
optional<reference_wrapper<Reactor const>> ComponentSet::getReactor(
    string const &name) const noexcept {
  return getFrom(reactors, name);
}
optional<reference_wrapper<FTL const>> ComponentSet::getFTL(
    string const &name) const noexcept {
  return getFrom(ftls, name);
}
optional<reference_wrapper<Sublight const>> ComponentSet::getSublight(
    string const &name) const noexcept {
  return getFrom(sublights, name);
}
optional<reference_wrapper<Sensor const>> ComponentSet::getSensor(
    string const &name) const noexcept {
  return getFrom(sensors, name);
}
optional<reference_wrapper<Computer const>> ComponentSet::getComputer(
    string const &name) const noexcept {
  return getFrom(computers, name);
}
optional<reference_wrapper<Aura const>> ComponentSet::getAura(
    string const &name) const noexcept {
  return getFrom(auras, name);
}
optional<reference_wrapper<Utility const>> ComponentSet::getUtility(
    string const &name) const noexcept {
  return getFrom(utilities, name);
}
optional<reference_wrapper<Auxiliary const>> ComponentSet::getAuxiliary(
    string const &name) const noexcept {
  return getFrom(auxiliaries, name);
}
optional<reference_wrapper<Weapon const>> ComponentSet::getWeapon(
    string const &name) const noexcept {
  return getFrom(weapons, name);
}
}  // namespace athena2::model::component
