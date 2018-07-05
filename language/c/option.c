//
// Metos3D: A Marine Ecosystem Toolkit for Optimization and Simulation in 3-D
// Copyright (C) 2018  Jaroslaw Piwonski, CAU, jpi@informatik.uni-kiel.de
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "option.h"

// option
#undef  __FUNCT__
#define __FUNCT__ "option"
PetscErrorCode option(char *filepath, context *ctx) {
    char ustr[PETSC_MAX_PATH_LEN];
    char uwrk[PETSC_MAX_PATH_LEN];
    char line[PETSC_MAX_PATH_LEN];
    char text[1<<16];               // 64K, should be sufficient
    const PetscScalar *uarr;
    int i;

    // u
    VecGetArrayRead(ctx->u, &uarr);
    strcpy(ustr, "");
    for(i=0; i<ctx->nu-1; i++) {
        sprintf(uwrk, "%.16e,", uarr[i]);
        strcat(ustr, uwrk);
    }
    i = ctx->nu-1;
    sprintf(uwrk, "%.16e", uarr[i]);
    strcat(ustr, uwrk);
    VecRestoreArrayRead(ctx->u, &uarr);

    strcpy(text, "");

    // debug
    sprintf(line, "%s%s", "-Metos3DDebugLevel                                  1", "\n");
    
    // geometry
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DGeometryType                                Profile", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DProfileInputDirectory                       data/TMM/2.8/Geometry/", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DProfileMaskFile                             landSeaMask.petsc", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DProfileVolumeFile                           volumes.petsc", "\n");

    // tracer
    strcat(text, line); sprintf(line, "%s%d%s", "-Metos3DTracerCount                                 ", ctx->ny, "\n");
    strcat(text, line); sprintf(line, "%s%s%s", "-Metos3DTracerInitValue                             ", ctx->y0, "\n");
    strcat(text, line); sprintf(line, "%s%s%s", "-Metos3DTracerOutputDirectory                       ", getenv("SCRATCH"), "\n");
    strcat(text, line); sprintf(line, "%s%s%s", "-Metos3DTracerOutputFile                            ", ctx->yout, "\n");

    // parameter
    strcat(text, line); sprintf(line, "%s%d%s", "-Metos3DParameterCount                              ", ctx->nu, "\n");
    strcat(text, line); sprintf(line, "%s%s%s", "-Metos3DParameterValue                              ", ustr, "\n");

    // boundary
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DBoundaryConditionCount                      2", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DBoundaryConditionInputDirectory             data/TMM/2.8/Forcing/BoundaryCondition/", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DBoundaryConditionName                       Latitude,IceCover", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DLatitudeCount                               1", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DLatitudeFileFormat                          latitude.petsc", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DIceCoverCount                               12", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DIceCoverFileFormat                          fice_$02d.petsc", "\n");

    // domain
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DDomainConditionCount                        2", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DDomainConditionInputDirectory               data/TMM/2.8/Forcing/DomainCondition/", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DDomainConditionName                         LayerDepth,LayerHeight", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DLayerDepthCount                             1", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DLayerDepthFileFormat                        z.petsc", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DLayerHeightCount                            1", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DLayerHeightFileFormat                       dz.petsc", "\n");

    // transport
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DTransportType                               Matrix", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DMatrixInputDirectory                        data/TMM/2.8/Transport/Matrix5_4/1dt/", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DMatrixCount                                 12", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DMatrixExplicitFileFormat                    Ae_$02d.petsc", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DMatrixImplicitFileFormat                    Ai_$02d.petsc", "\n");

    // time step
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DTimeStepStart                               0.0", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DTimeStepCount                               2880", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DTimeStep                                    0.0003472222222222", "\n");

    // solver
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DSolverType                                  Spinup", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DSpinupCount                                 3000", "\n");
//    strcat(text, line); sprintf(line, "%s%s", "-Metos3DSpinupCount                                 1", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DSpinupMonitor", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DSpinupMonitorModuloStep                     3000,1", "\n");
//    strcat(text, line); sprintf(line, "%s%s", "-Metos3DSpinupMonitorModuloStep                     1,1", "\n");
    strcat(text, line); sprintf(line, "%s%s", "-Metos3DSpinupMonitorFileFormatPrefix               ,$00004d-", "\n");
    strcat(text, line);
    
    FILE *f;
    f = fopen(filepath, "w");
    fprintf(f, text);
    fclose(f);
    
    return(0);
}


