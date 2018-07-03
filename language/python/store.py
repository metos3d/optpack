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

import h5py

def store(ctx):
    if ctx.i==1:
        # we are at the beginning of the optimization and
        # store initially J, lb, u, ub, ud, y, yd
        logfile = h5py.File(ctx.logfile, "w")
        logfile.create_dataset("J", data=ctx.J[None,:], maxshape=(None,ctx.ndata))
        logfile.create_dataset("lb", data=ctx.lb[None,:])
        logfile.create_dataset("u", data=ctx.u[None,:], maxshape=(None,ctx.nu))
        logfile.create_dataset("ub", data=ctx.ub[None,:])
        logfile.create_dataset("ud", data=ctx.ud[None,:])
        logfile.create_dataset("y", data=ctx.y[None,:,:], maxshape=(None,ctx.ndata,ctx.nx), compression="gzip")
        logfile.create_dataset("yd", data=ctx.yd, compression="gzip")
        logfile.close()
    else:
        # update J, u and y
        logfile = h5py.File(ctx.logfile, "a")
        logfile["J"].resize(ctx.i, axis=0)
        logfile["J"][ctx.i-1,:] = data=ctx.J
        logfile["u"].resize(ctx.i, axis=0)
        logfile["u"][ctx.i-1,:] = data=ctx.u
        logfile["y"].resize(ctx.i, axis=0)
        logfile["y"][ctx.i-1,:,:] = data=ctx.y
        logfile.close()


