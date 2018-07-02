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
#        if i%500==0: print("# {}".format(filepath))
        y[i,:] = read_petsc_vector(filepath)
    yd = average(y, ctx)

    return yd


