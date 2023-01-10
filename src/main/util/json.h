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

#ifndef ATHENA2_UTIL_JSON_H_
#define ATHENA2_UTIL_JSON_H_

#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dsl.h"

namespace athena2::util {
inline nlohmann::json parse(std::istream &stream, EvalContext &ctx) {
  try {
    return nlohmann::json::parse(stream);
  } catch (nlohmann::json::parse_error &e) {
    ctx.error("invalid JSON: " + static_cast<std::string>(e.what()));
  }
}
inline nlohmann::json const &checkType(nlohmann::json const &json,
                                       bool (nlohmann::json::*typechecker)()
                                           const,
                                       std::string const &type,
                                       EvalContext &ctx) {
  if (!(json.*typechecker)()) ctx.error("expected " + type);
  return json;
}
inline nlohmann::json const &checkFieldType(
    nlohmann::json const &json, std::string const &key,
    bool (nlohmann::json::*typechecker)() const, std::string const &type,
    EvalContext &ctx) {
  if (!json.contains(key)) {
    ctx.error("missing field " + key);
  } else if (!(json[key].*typechecker)()) {
    auto _ = ctx.push(key);
    ctx.error("expected field of type " + type);
  }

  return json[key];
}
inline std::optional<std::reference_wrapper<nlohmann::json const>>
checkMaybeFieldType(nlohmann::json const &json, std::string const &key,
                    bool (nlohmann::json::*typechecker)() const,
                    std::string const &type, EvalContext &ctx) {
  if (!json.contains(key)) {
    return std::nullopt;
  } else if (!(json[key].*typechecker)()) {
    ctx.error("expected " + type + " for field " + key);
  }

  return json[key];
}
inline std::string checkString(nlohmann::json const &json,
                               std::string const &key, EvalContext &ctx) {
  return checkFieldType(json, key, &nlohmann::json::is_string, "string", ctx)
      .get<std::string>();
}
inline float checkFloat(nlohmann::json const &json, std::string const &key,
                        EvalContext &ctx) {
  return checkFieldType(json, key, &nlohmann::json::is_number, "number", ctx)
      .get<float>();
}
inline std::optional<float> checkMaybeFloat(nlohmann::json const &json,
                                            std::string const &key,
                                            EvalContext &ctx) {
  auto maybeField =
      checkMaybeFieldType(json, key, &nlohmann::json::is_number, "number", ctx);
  if (!maybeField)
    return std::nullopt;
  else
    return maybeField->get().get<float>();
}
inline std::optional<bool> checkMaybeBool(nlohmann::json const &json,
                                          std::string const &key,
                                          EvalContext &ctx) {
  auto maybeField = checkMaybeFieldType(json, key, &nlohmann::json::is_boolean,
                                        "boolean", ctx);
  if (!maybeField)
    return std::nullopt;
  else
    return maybeField.value().get().get<bool>();
}
inline nlohmann::json const &checkArray(nlohmann::json const &json,
                                        std::string const &key,
                                        EvalContext &ctx) {
  return checkFieldType(json, key, &nlohmann::json::is_array, "array", ctx);
}
inline std::vector<std::string> checkStringArray(nlohmann::json const &json,
                                                 std::string const &key,
                                                 EvalContext &ctx) {
  nlohmann::json array =
      checkFieldType(json, key, &nlohmann::json::is_array, "array", ctx);
  auto _ = ctx.push(key);
  std::vector<std::string> result;
  for (auto const &[key, val] : array.items()) {
    auto _ = ctx.push(key);
    result.push_back(checkType(val, &nlohmann::json::is_string, "string", ctx)
                         .get<std::string>());
  }
  return result;
}
inline nlohmann::json const &checkObject(nlohmann::json const &json,
                                         std::string const &key,
                                         EvalContext &ctx) {
  return checkFieldType(json, key, &nlohmann::json::is_object, "object", ctx);
}
inline std::optional<std::reference_wrapper<nlohmann::json const>>
checkMaybeObject(nlohmann::json const &json, std::string const &key,
                 EvalContext &ctx) {
  return checkMaybeFieldType(json, key, &nlohmann::json::is_object, "object",
                             ctx);
}
inline nlohmann::json const &checkObject(nlohmann::json const &json,
                                         EvalContext &ctx) {
  return checkType(json, &nlohmann::json::is_object, "object", ctx);
}
inline void checkFields(nlohmann::json const &json,
                        std::vector<std::string> const &allowedFields,
                        EvalContext &ctx) {
  for (auto const &[key, _] : json.items()) {
    if (std::find(allowedFields.begin(), allowedFields.end(), key) ==
        allowedFields.end()) {
      ctx.error("unexpected field " + key);
    }
  }
}
}  // namespace athena2::util

#endif  // ATHENA2_UTIL_JSON_H_
