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

import os
import numpy as np

from python.option import option
from python.read_petsc_vector import read_petsc_vector

def model(u, ctx):
    for ui in u:
        print("# u:        {:.16e}".format(ui))

    # store current parameter set
    ctx.u = u

    # option
    optionfile = ctx.expname + "." + "{:d}".format(ctx.nexp) + "." + "{:03d}".format(ctx.i) + ".option.sh"
    optionfilepath = "model/option/" + optionfile
    option(optionfilepath, ctx)

    # run
    logfile = ctx.expname + "." + "{:d}".format(ctx.nexp) + "." + "{:03d}".format(ctx.i) + ".log.txt"
    logfilepath = "model/log/" + logfile
    runcmd = ". model/petsc.env.sh; " + \
                os.environ["MPIRUN"] + " ./model/metos3d-simpack-" + ctx.modname + ".exe " + \
              optionfilepath + " > " + logfilepath
    print("# run:      " + runcmd, flush=True)
    status = os.system(runcmd)

    # read result from scratch
    readpath = os.environ["SCRATCH"]
    nx = ctx.nx
    nt = ctx.nt
    y = np.zeros((nt,nx))       # note, c order
    for i in range(nt):
        filepath = readpath + "{:04d}".format(i) + "-N.petsc"
        if i%500==0: print("# {}".format(filepath), flush=True)
        y[i,:] = read_petsc_vector(filepath);

    # clean scratch
    cleancmd = "rm " + readpath + "*"
    print("# clean:    " + cleancmd, flush=True)
    status = os.system(cleancmd)

    return y


