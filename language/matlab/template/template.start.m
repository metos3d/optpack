%
% Metos3D: A Marine Ecosystem Toolkit for Optimization and Simulation in 3-D
% Copyright (C) 2018  Jaroslaw Piwonski, CAU, jpi@informatik.uni-kiel.de
%
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.
%

% info
format compact
system('echo ''# date:    '' `date`');
system('echo ''# hostname:'' `hostname`');
system('echo ''# PPID:    '' $PPID');

% paths
addpath('matlab');

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
ctx.u0      = [{parameter[u0]}];
ctx.ud      = [{parameter[ud]}];    % not used, info only
ctx.lb      = [{parameter[lb]}];
ctx.ub      = [{parameter[ub]}];

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


