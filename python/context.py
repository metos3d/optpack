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


