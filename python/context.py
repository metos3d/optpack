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

class context:
    # id
    expname = []        # name of experiment
    nexp    = []        # experiment number
    # model
    modname = []        # name of bgc model
    ny      = []        # number of tracers
    nx      = []        # vector length
    nt      = []        # number of time steps
    y0      = []        # initial concentration
    yout    = []        # output file name(s)
    # parameters
    nu      = []        # number of parameters
    u0      = []        # initial parameter set
    ud      = []        # reference parameter set
    lb      = []        # lower bounds
    ub      = []        # upper bounds
    # data
    ndata   = []        # number of data
    yd      = []        # data
    # optimization
    options = []        # algorithm options
    i       = []        # current iteration
    u       = []        # current parameter set
    y       = []        # current state
    J       = []        # current objective
    # log
    logfile = []        # name of log file


