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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "dsl.h"
#include "version.h"

using namespace std;
using namespace athena2;

int main(int argc, char **argv) {
  vector<string> args;
  copy(argv + 1, argv + argc, back_inserter(args));

  if (args.size() != 1 ||
      find_if(args.begin(), args.end(), [](string const &s) {
        return s == "--help" || s == "-h" || s == "-?";
      }) != args.end()) {
    cout << "Usage: \n"
         << argv[0] << " <runspec>: read the file <runspec>\n"
         << argv[0] << " -        : read from stdin\n";
    return 0;
  } else if (find_if(args.begin(), args.end(), [](string const &s) {
               return s == "--version" || s == "-v";
             }) != args.end()) {
    cout << "Athena II version " << ATHENA2_VERSION_MAJOR << "."
         << ATHENA2_VERSION_MINOR << "." << ATHENA2_VERSION_PATCH << "\n"
         << "Copyright 2023 Justin Hu\n"
         << "This is free software; see the source for copying conditions. "
            "There is NO\nwarranty; not even for MERCHANTABILITY or FITNESS "
            "FOR A PARTICULAR PURPOSE.\n";
    return 0;
  }

  if (args[0] == "-") {
    EvalContext context = EvalContext("(stdin)");
    return eval(cin, context);
  } else {
    EvalContext context = EvalContext(args[0]);
    ifstream in(args[0]);
    return eval(in, context);
  }
}
