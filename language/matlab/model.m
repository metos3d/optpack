function y = model(u,ctx)
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
% y = model(u,ctx)

    disp(num2str(u, '# u:        %.16e\n'))

    % store current parameter set
    ctx.u = u;

    % option
    optionfile = [ctx.expname '.' num2str(ctx.nexp) '.' num2str(ctx.i, '%03d') '.option.sh'];
    optionfilepath = ['model/option/' optionfile];
    option(optionfilepath,ctx);

    % run
    logfile = [ctx.expname '.' num2str(ctx.nexp) '.' num2str(ctx.i, '%03d') '.log.txt'];
    logfilepath = ['model/log/' logfile];
    runcmd = ['. model/petsc.env.sh; '...
              getenv('MPIRUN') ' ./model/metos3d-simpack-' ctx.modname '.exe ' ...
              optionfilepath ' > ' logfilepath];
    disp(['# run:      ' runcmd])
    [status, result] = system(runcmd);
%    disp(['# status:   ' num2str(status)])

    % read result from scratch
    readpath = getenv('SCRATCH');
    nx = ctx.nx;
    nt = ctx.nt;
    y = zeros(nx, nt);
    for i = 1:nt;
        filepath = [readpath num2str(i-1, '%04d') '-N.petsc'];
%        if mod(i,500)==0 disp(['# ' filepath]), end;
        y(:, i) = read_petsc_vector(filepath);
    end;

    % clean scratch
    cleancmd = ['rm ' readpath '*'];
    disp(['# clean:    ' cleancmd])
    [status, result] = system(cleancmd);
%    disp(['# status:   ' num2str(status)])
end


