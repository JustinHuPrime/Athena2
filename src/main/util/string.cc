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

#include "util/string.h"

#include <sstream>

using namespace std;

namespace athena2::util {
vector<string> split(string const &s, char delim) noexcept {
  istringstream buffer = istringstream(s);
  vector<string> tokens;
  string token;

  while (getline(buffer, token, delim)) tokens.push_back(token);

  return tokens;
}

string fromStream(istream &stream) noexcept {
  stringstream buffer;
  buffer << stream.rdbuf();
  return buffer.str();
}
}  // namespace athena2::util
