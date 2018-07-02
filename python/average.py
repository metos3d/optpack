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

def average(y, ctx):
    # annual
    nt = ctx.nt
    yan = 1./nt * np.sum(y[0:nt,:], axis=0)
    
    # monthly
    start = 0;    yjan = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 240;  yfeb = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 480;  ymar = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 720;  yapr = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 960;  ymay = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1200; yjun = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1440; yjul = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1680; yaug = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1920; ysep = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 2160; yoct = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 2400; ynov = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 2640; ydec = 12./nt * np.sum(y[start:start+240,:], axis=0)
    
    # seasonal
    start = 0;    ywin = 4./nt * np.sum(y[start:start+720,:], axis=0)
    start = 720;  yspr = 4./nt * np.sum(y[start:start+720,:], axis=0)
    start = 1440; ysum = 4./nt * np.sum(y[start:start+720,:], axis=0)
    start = 2160; yaut = 4./nt * np.sum(y[start:start+720,:], axis=0)

    # store, same order as woa data, caution: matlab uses 1 indexed arrays
    # 00, annual
    # 01 - 12, monthly
    # 13 - 16, seasonal
    yavg = np.array([yan,yjan,yfeb,ymar,yapr,ymay,yjun,yjul,yaug,ysep,yoct,ynov,ydec,ywin,yspr,ysum,yaut])

    return yavg


