function yavg = average(y,ctx)
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
% yavg = average(y,ctx)

    % annual
    nt = ctx.nt;
    ydan = 1./nt * sum(y(:, 1:nt), 2);

    % monthly
    start = 0000; ydjan = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 0240; ydfeb = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 0480; ydmar = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 0720; ydapr = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 0960; ydmay = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 1200; ydjun = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 1440; ydjul = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 1680; ydaug = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 1920; ydsep = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 2160; ydoct = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 2400; ydnov = 12./nt * sum(y(:, start+1:start+240), 2);
    start = 2640; yddec = 12./nt * sum(y(:, start+1:start+240), 2);

    % seasonal
    start = 0000; ydwin = 4./nt * sum(y(:, start+1:start+720), 2);
    start = 0720; ydspr = 4./nt * sum(y(:, start+1:start+720), 2);
    start = 1440; ydsum = 4./nt * sum(y(:, start+1:start+720), 2);
    start = 2160; ydaut = 4./nt * sum(y(:, start+1:start+720), 2);

    % store, same order as woa data, caution: matlab uses 1 indexed arrays
    % 00, annual
    % 01 - 12, monthly
    % 13 - 16, seasonal
    yavg = [ydan ydjan ydfeb ydmar ydapr ydmay ydjun ydjul ydaug ydsep ydoct ydnov yddec ydwin ydspr ydsum ydaut];
end





