function store(ctx)
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
% store(ctx)

    if ctx.i > 1;
        % update J, u and y
        logfile = matfile(ctx.logfile, 'writable', true);
        logfile.J(:,ctx.i) = ctx.J;
        logfile.u(:,ctx.i) = ctx.u';        % transposed
        logfile.y(:,:,ctx.i) = ctx.y;
    else;
        % we are at the beginning of the optimization and
        % store initially J, lb, u, ub, ud, y, yd
        J   = ctx.J;
        lb  = ctx.lb';                      % transposed
        u   = ctx.u';                       % transposed
        ub  = ctx.ub';                      % transposed
        ud  = ctx.ud';                      % transposed
        % this is a trick, we make the stored variable 3D,
        % so it can be extended during the other iteratons
        y   = cat(3,ctx.y,ctx.y);
        yd  = ctx.yd;
        save(ctx.logfile, 'J', 'lb', 'u', 'ub', 'ud', 'y', 'yd', '-v7.3');
    end;
end


