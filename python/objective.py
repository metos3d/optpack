from python.model import model
from python.misfit import misfit
from python.store import store

def objective(u, ctx):
    print("# i:        {:03d}".format(ctx.i))
    
    y = model(u, ctx)
    J = misfit(y, ctx)
#    J = 0.0

    store(ctx)
    ctx.i = ctx.i+1

    return J


