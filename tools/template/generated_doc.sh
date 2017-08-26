# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# This file is supposed to be used by other scripts providing the process_one
# command.

process_one "$REFERENCE_GEN" --show_pragma_help docs/reference/pragmas.md '<!-- pragma_info -->'
process_one "$REFERENCE_GEN" --show_mdb_help docs/reference/mdb_commands.md '<!-- mdb_info -->'
process_one "$REFERENCE_GEN" --show_pdb_help docs/reference/pdb_commands.md '<!-- pdb_info -->'
process_one "$REFERENCE_GEN" --show_feature_help docs/reference/engines.md '<!-- feature_info -->'
process_one "$REFERENCE_GEN" --show_engine_help docs/reference/engines.md '<!-- engine_info -->'
process_one "$REFERENCE_GEN" --show_engine_features docs/reference/engines.md '<!-- feature_matrix -->'

