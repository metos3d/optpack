% info
format compact
system('echo ''# date:    '' `date`');
system('echo ''# hostname:'' `hostname`');
system('echo ''# PPID:    '' $PPID');

% paths
addpath('matlab');
addpath('model');

% initialization
ctx         = context;

% id
ctx.expname = '{experiment[name]}';
ctx.nexp    = {nexp};

% algorithm
options     = [];
options     = optimset(options, 'algorithm', 'interior-point');
options     = optimset(options, 'display', 'iter');
options     = optimset(options, 'maxiter', {niter});
ctx.options = options;

% model
ctx.modname = '{model[name]}';
ctx.ny      = {model[ny]};
ctx.nx      = {model[nx]};
ctx.nt      = {model[nt]};
ctx.y0      = '{model[y0]}';
ctx.yout    = '{model[yout]}';

% parameters
ctx.nu      = {parameter[nu]};
if ctx.nexp == 1;
    % inital experiment, use hard-coded parameter set
    ctx.u0 = {parameter[u0]};
else;
    % consecutive experiment, load optimal parameter set from previous optimization
    logfile = [ctx.expname filesep ctx.expname '.' ctx.modname '.' num2str(ctx.nexp-1) '.mat'];
    load(logfile, 'uopt');
    ctx.u0 = uopt';   % transposed
end;
ctx.ud      = {parameter[ud]};    % not used, info only
ctx.lb      = {parameter[lb]};
ctx.ub      = {parameter[ub]};

% data
ctx.ndata   = 17;
ctx.yd      = data(ctx);

% optimization
ctx.i       = 1;

% log
ctx.logfile = [ctx.expname filesep ctx.expname '.' ctx.modname '.' num2str(ctx.nexp) '.mat'];

% optimization
[uopt,Jopt,exitflag,output,lambda,grad,hessian] = fmincon(@(u) objective(u,ctx),ctx.u0,[],[],[],[],ctx.lb,ctx.ub,[],ctx.options)

% store optimal parameter set and corresponding objective value
uopt = uopt';   % transposed
save(ctx.logfile, 'uopt', 'Jopt', '-append')


