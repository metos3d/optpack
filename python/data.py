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

import numpy as np

from python.read_petsc_vector import read_petsc_vector
from python.average import average

def data(ctx):
    datapath = "../../../../twin-data/N/work/"
    nx = ctx.nx
    nt = ctx.nt
    y = np.zeros((nt,nx))   # note, c order
    for i in range(nt):
        filepath = datapath + "{:04d}".format(i) + "-N.petsc"
#        if i%500==0: print("# {}".format(filepath), flush=True)
        y[i,:] = read_petsc_vector(filepath)
    yd = average(y, ctx)

    return yd


