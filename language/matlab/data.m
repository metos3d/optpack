function yd = data(ctx)
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
% yd = data(ctx)

    datapath = ['../../../../twin-data/' ctx.modname '/work/'];
    nt = ctx.nt;
    nx = ctx.nx;
    y = zeros(nx, nt);
    for i = 1:nt;
        filepath  = [datapath num2str(i-1, '%04d') '-N.petsc'];
%        if mod(i,500)==0 disp(['# ' filepath]), end;
        y(:, i) = read_petsc_vector(filepath);
    end;
    yd = average(y,ctx);
end


