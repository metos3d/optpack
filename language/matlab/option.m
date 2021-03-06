function option(filepath,ctx)
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
% option(filepath,ctx)

    ny      = num2str(ctx.ny);
    y0      = ctx.y0;
    ypath   = getenv('SCRATCH');
    yout    = ctx.yout;
    nu      = num2str(ctx.nu);
    u       = num2str(ctx.u, '%.16e,');
    u       = u(1:end-1);
    text    = '';

    % debug
    text = [text '-Metos3DDebugLevel                                  1' '\n'];

    % geometry
    text = [text '-Metos3DGeometryType                                Profile' '\n'];
    text = [text '-Metos3DProfileInputDirectory                       model/data/TMM/2.8/Geometry/' '\n'];
    text = [text '-Metos3DProfileMaskFile                             landSeaMask.petsc' '\n'];
    text = [text '-Metos3DProfileVolumeFile                           volumes.petsc' '\n'];

    % tracer
    text = [text '-Metos3DTracerCount                                 ' ny '\n'];
    text = [text '-Metos3DTracerInitValue                             ' y0 '\n'];
    text = [text '-Metos3DTracerOutputDirectory                       ' ypath '\n'];
    text = [text '-Metos3DTracerOutputFile                            ' yout '\n'];

    % diagnostic
    text = [text '-Metos3DDiagnosticCount                             0' '\n'];

    % parameter
    text = [text '-Metos3DParameterCount                              ' nu '\n'];
    text = [text '-Metos3DParameterValue                              ' u '\n'];

    % boundary
    text = [text '-Metos3DBoundaryConditionCount                      2' '\n'];
    text = [text '-Metos3DBoundaryConditionInputDirectory             model/data/TMM/2.8/Forcing/BoundaryCondition/' '\n'];
    text = [text '-Metos3DBoundaryConditionName                       Latitude,IceCover' '\n'];
    text = [text '-Metos3DLatitudeCount                               1' '\n'];
    text = [text '-Metos3DLatitudeFileFormat                          latitude.petsc' '\n'];
    text = [text '-Metos3DIceCoverCount                               12' '\n'];
    text = [text '-Metos3DIceCoverFileFormat                          fice_$02d.petsc' '\n'];

    % domain
    text = [text '-Metos3DDomainConditionCount                        2' '\n'];
    text = [text '-Metos3DDomainConditionInputDirectory               model/data/TMM/2.8/Forcing/DomainCondition/' '\n'];
    text = [text '-Metos3DDomainConditionName                         LayerDepth,LayerHeight' '\n'];
    text = [text '-Metos3DLayerDepthCount                             1' '\n'];
    text = [text '-Metos3DLayerDepthFileFormat                        z.petsc' '\n'];
    text = [text '-Metos3DLayerHeightCount                            1' '\n'];
    text = [text '-Metos3DLayerHeightFileFormat                       dz.petsc' '\n'];

    % transport
    text = [text '-Metos3DTransportType                               Matrix' '\n'];
    text = [text '-Metos3DMatrixInputDirectory                        model/data/TMM/2.8/Transport/Matrix5_4/1dt/' '\n'];
    text = [text '-Metos3DMatrixCount                                 12' '\n'];
    text = [text '-Metos3DMatrixExplicitFileFormat                    Ae_$02d.petsc' '\n'];
    text = [text '-Metos3DMatrixImplicitFileFormat                    Ai_$02d.petsc' '\n'];

    % time step
    text = [text '-Metos3DTimeStepStart                               0.0' '\n'];
    text = [text '-Metos3DTimeStepCount                               2880' '\n'];
    text = [text '-Metos3DTimeStep                                    0.0003472222222222' '\n'];

    % solver
    text = [text '-Metos3DSolverType                                  Spinup' '\n'];
    text = [text '-Metos3DSpinupCount                                 3000' '\n'];
%    text = [text '-Metos3DSpinupCount                                 1' '\n'];
    text = [text '-Metos3DSpinupMonitor' '\n'];
    text = [text '-Metos3DSpinupMonitorModuloStep                     3000,1' '\n'];
%    text = [text '-Metos3DSpinupMonitorModuloStep                     1,1' '\n'];
    text = [text '-Metos3DSpinupMonitorFileFormatPrefix               ,$00004d-' '\n'];

    % write to file
    fid = fopen(filepath, 'w', 'ieee-be');
    err = fprintf(fid, text);
    err = fclose(fid);
end


