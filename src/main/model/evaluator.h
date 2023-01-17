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

#ifndef ATHENA2_MODEL_EVALUATOR_H_
#define ATHENA2_MODEL_EVALUATOR_H_

#include <utility>

#include "model/design/fleet.h"

namespace athena2::model {
struct EvaluationSettings {
  float fightLengthLimit;
  float withdrawMultiplier;
  bool debugDump;
};
constexpr float TIME_QUANTUM = 0.1f;
/**
 * score two fleets against each other
 *
 * return value indicates the damage dealt to the first fleet and the damage
 * dealt to the second fleet, in mineral-equivalents lost/withdrawn
 */
std::pair<float, float> evaluate(design::Fleet const &a, design::Fleet const &b,
                                 EvaluationSettings const &settings) noexcept;
}  // namespace athena2::model

#endif  // ATHENA2_MODEL_EVALUATOR_H_
