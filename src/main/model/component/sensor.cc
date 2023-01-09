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

#include "model/component/sensor.h"

#include "util/json.h"

using namespace std;
using namespace nlohmann;
using namespace athena2::util;

namespace athena2::model::component {
Sensor Sensor::fromJson(json const &data, EvalContext &ctx) {
  checkObject(data, ctx);
  string name = checkString(data, "name", ctx);
  float power = checkFloat(data, "power", ctx);
  float trackingBonus = checkFloat(data, "trackingBonus", ctx);
  json const &costData = checkObject(data, "cost", ctx);
  Cost cost = [&ctx, &costData]() {
    auto _ = ctx.push("cost");
    return Cost::fromJson(costData, ctx);
  }();
  checkFields(data, {"name", "power", "trackingBonus", "cost"}, ctx);

  return Sensor(name, power, trackingBonus, cost);
}
Sensor::Sensor(string const &name_, float power_, float trackingBonus_,
               Cost const &cost_) noexcept
    : Component(name_),
      power(power_),
      trackingBonus(trackingBonus_),
      cost(cost_) {}
}  // namespace athena2::model::component
