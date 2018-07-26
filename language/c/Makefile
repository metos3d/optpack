#
# Metos3D: A Marine Ecosystem Toolkit for Optimization and Simulation in 3-D
# Copyright (C) 2018  Jaroslaw Piwonski, CAU, jpi@informatik.uni-kiel.de
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
#

# environment variables to set
#   PROGRAM         the name of the executable
#   OBJECT_FILE     the name of the object file

# object files
OPTPACK = \
	c/average.o \
	c/data.o \
	c/misfit.o \
	c/model.o \
	c/objective.o \
	c/option.o \
	c/store.o

ALL: $(PROGRAM)
CFLAGS = -I c/
FFLAGS =
CLEANFILES = $(OPTPACK) $(OBJECT_FILE) $(PROGRAM)

include $(PETSC_DIR)/lib/petsc/conf/variables
include $(PETSC_DIR)/lib/petsc/conf/rules

$(PROGRAM): $(OPTPACK) $(OBJECT_FILE) chkopts
	-$(CLINKER) -o $@ $(OPTPACK) $(OBJECT_FILE) $(PETSC_LIB)


