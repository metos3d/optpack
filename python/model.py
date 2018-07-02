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
    optionfile = ctx.expname + "." + ctx.modname + "." + "{:d}".format(ctx.nexp) + "." + "{:03d}".format(ctx.i) + ".option.sh"
    optionfilepath = ctx.expname + "/option/" + optionfile
    option(optionfilepath, ctx)

    # run
    logfile = ctx.expname + "." + ctx.modname + "." + "{:d}".format(ctx.nexp) + "." + "{:03d}".format(ctx.i) + ".log.txt"
    logfilepath = ctx.expname + "/log/" + logfile
    runcmd = ". model/de.uni-kiel.rz.nesh-fe.petsc-3.3-p7.opt.sh; " + \
                "mpiexec " + os.environ["NQSII_MPIOPTS"] + " ./model/metos3d-simpack-" + ctx.modname + ".exe " + \
              optionfilepath + " > " + logfilepath
#    print("# run:      " + runcmd)
    status = os.system(runcmd)

    # read result from scratch
    readpath = os.environ["SCRATCH"]
    nx = ctx.nx
    nt = ctx.nt
    y = np.zeros((nt,nx))       # note, c order
    for i in range(nt):
        filepath = readpath + "{:04d}".format(i) + "-N.petsc"
#        if i%500==0: print("# {}".format(filepath))
        y[i,:] = read_petsc_vector(filepath);

    # clean scratch
    cleancmd = "rm " + readpath + "*"
#    print("# clean:    " + cleancmd)
    status = os.system(cleancmd)

    return y


