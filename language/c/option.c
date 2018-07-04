#include "option.h"

// option
#undef  __FUNCT__
#define __FUNCT__ "option"
PetscErrorCode option(char *file_path, context *ctx) {
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
    strcat(text, line);
    
    // geometry
    sprintf(line, "%s%s", "-Metos3DGeometryType                                Profile", "\n");
    strcat(text, line);
    sprintf(line, "%s%s", "-Metos3DProfileInputDirectory                       data/TMM/2.8/Geometry/", "\n");
    strcat(text, line);
    sprintf(line, "%s%s", "-Metos3DProfileMaskFile                             landSeaMask.petsc", "\n");
    strcat(text, line);
    sprintf(line, "%s%s", "-Metos3DProfileVolumeFile                           volumes.petsc", "\n");
    strcat(text, line);

    // tracer
    sprintf(line, "%s%d%s", "-Metos3DTracerCount                                 ", ctx->ny, "\n");
    sprintf(line, "%s%s%s", "-Metos3DTracerInitValue                             ", ctx->y0, "\n");
    sprintf(line, "%s%s%s", "-Metos3DTracerOutputDirectory                       ", getenv("SCRATCH"), "\n");
    sprintf(line, "%s%s%s", "-Metos3DTracerOutputFile                            ", ctx->yout, "\n");

    // parameter
    sprintf(line, "%s%d%s", "-Metos3DParameterCount                              ", ctx->nu, "\n");
    sprintf(line, "%s%s%s", "-Metos3DParameterValue                              ", ustr, "\n");

    print(text);

    
//
//    // boundary
//    fprintf(f, "%s%s", "-Metos3DBoundaryConditionCount                      2", "\n");
//    fprintf(f, "%s%s", "-Metos3DBoundaryConditionInputDirectory             data/TMM/2.8/Forcing/BoundaryCondition/", "\n");
//    fprintf(f, "%s%s", "-Metos3DBoundaryConditionName                       Latitude,IceCover", "\n");
//    fprintf(f, "%s%s", "-Metos3DLatitudeCount                               1", "\n");
//    fprintf(f, "%s%s", "-Metos3DLatitudeFileFormat                          latitude.petsc", "\n");
//    fprintf(f, "%s%s", "-Metos3DIceCoverCount                               12", "\n");
//    fprintf(f, "%s%s", "-Metos3DIceCoverFileFormat                          fice_$02d.petsc", "\n");
//
//    // domain
//    fprintf(f, "%s%s", "-Metos3DDomainConditionCount                        2", "\n");
//    fprintf(f, "%s%s", "-Metos3DDomainConditionInputDirectory               data/TMM/2.8/Forcing/DomainCondition/", "\n");
//    fprintf(f, "%s%s", "-Metos3DDomainConditionName                         LayerDepth,LayerHeight", "\n");
//    fprintf(f, "%s%s", "-Metos3DLayerDepthCount                             1", "\n");
//    fprintf(f, "%s%s", "-Metos3DLayerDepthFileFormat                        z.petsc", "\n");
//    fprintf(f, "%s%s", "-Metos3DLayerHeightCount                            1", "\n");
//    fprintf(f, "%s%s", "-Metos3DLayerHeightFileFormat                       dz.petsc", "\n");
//
//    // transport
//    fprintf(f, "%s%s", "-Metos3DTransportType                               Matrix", "\n");
//    fprintf(f, "%s%s", "-Metos3DMatrixInputDirectory                        data/TMM/2.8/Transport/Matrix5_4/1dt/", "\n");
//    fprintf(f, "%s%s", "-Metos3DMatrixCount                                 12", "\n");
//    fprintf(f, "%s%s", "-Metos3DMatrixExplicitFileFormat                    Ae_$02d.petsc", "\n");
//    fprintf(f, "%s%s", "-Metos3DMatrixImplicitFileFormat                    Ai_$02d.petsc", "\n");
//
//    // time step
//    fprintf(f, "%s%s", "-Metos3DTimeStepStart                               0.0", "\n");
//    fprintf(f, "%s%s", "-Metos3DTimeStepCount                               2880", "\n");
//    fprintf(f, "%s%s", "-Metos3DTimeStep                                    0.0003472222222222", "\n");
//
//    // solver
//    fprintf(f, "%s%s", "-Metos3DSolverType                                  Spinup", "\n");
//    fprintf(f, "%s%s", "-Metos3DSpinupCount                                 3000", "\n");
////    fprintf(f, "%s%s", "-Metos3DSpinupCount                                 1", "\n");
//    fprintf(f, "%s%s", "-Metos3DSpinupMonitor", "\n");
//    fprintf(f, "%s%s", "-Metos3DSpinupMonitorModuloStep                     3000,1", "\n");
////    fprintf(f, "%s%s", "-Metos3DSpinupMonitorModuloStep                     1,1", "\n");
//    fprintf(f, "%s%s", "-Metos3DSpinupMonitorFileFormatPrefix               ,$00004d-", "\n");
//
    
//    FILE *f;
//    f = fopen(file_path, "w");
//    fprintf(f, text);
//    fclose(f);
    return(0);
}





