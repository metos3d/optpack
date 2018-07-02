import os
import sys
import h5py
import numpy as np
import scipy.optimize as spo

sys.path.append(os.getcwd())
from python.context import context
from python.data import data
from python.objective import objective

# info
os.system("echo '# date:    ' `date`")
os.system("echo '# hostname:' `hostname`")
os.system("echo '# PPID:    ' $PPID")

# initialization
ctx         = context()

# id
ctx.expname = "{experiment[name]}"
ctx.nexp    = {nexp}

# model
ctx.modname = "{model[name]}"
ctx.ny      = {model[ny]}
ctx.nx      = {model[nx]}
ctx.nt      = {model[nt]}
ctx.y0      = "{model[y0]}"
ctx.yout    = "{model[yout]}"

# parameters
ctx.nu      = {parameter[nu]}
ctx.u0      = np.array({parameter[u0]})
ctx.ud      = np.array({parameter[ud]})    # not used, info only
ctx.lb      = np.array({parameter[lb]})
ctx.ub      = np.array({parameter[ub]})

# data
ctx.ndata   = 17
ctx.yd      = data(ctx)

# optimization
ctx.i       = 1

# log
ctx.logfile = os.path.join(ctx.expname, ctx.expname + "." + ctx.modname + "." + str(ctx.nexp) + ".h5")

# algorithm
options = dict()
options["method"] = "L-BFGS-B"
options["bounds"] = list(zip(ctx.lb, ctx.ub))
options["options"] = dict()
options["options"]["maxiter"] = {niter}
options["options"]["disp"] = True
ctx.options = options

# optimization run
r = spo.minimize(lambda u: objective(u, ctx), ctx.u0, **ctx.options)
print(r)

# store optimal parameter set and corresponding objective value
logfile = h5py.File(ctx.logfile, "a")
logfile.create_dataset("Jopt", data=np.array(r.fun)[None])
logfile.create_dataset("uopt", data=r.x[None,:])
logfile.close()





