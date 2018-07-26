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

#include "model.h"

#undef  __FUNCT__
#define __FUNCT__ "model"
PetscErrorCode model(Vec *y, Vec u, context *ctx) {
    int i, start, status;
    char optionfile     [PETSC_MAX_PATH_LEN];
    char optionfilepath [PETSC_MAX_PATH_LEN];
    char logfile        [PETSC_MAX_PATH_LEN];
    char logfilepath    [PETSC_MAX_PATH_LEN];
    char runcmd         [PETSC_MAX_PATH_LEN];
    char runcmdpart     [PETSC_MAX_PATH_LEN];
    char readpath       [PETSC_MAX_PATH_LEN];
    char readfilepath   [PETSC_MAX_PATH_LEN];
    char cleancmd       [PETSC_MAX_PATH_LEN];
    PetscViewer viewer;
    const PetscScalar* uarr;
    
    // info
    VecGetArrayRead(u, &uarr);
    for(i=0; i<ctx->nu; i++) PetscPrintf(ctx->comm, "# u:        %.16e\n", uarr[i]);
    VecRestoreArrayRead(u, &uarr);
    
    // store current parameter set
    VecCopy(u, ctx->u);
    
    // option file
    sprintf(optionfile, "%s.%d.%03d%s", ctx->expname, ctx->nexp, ctx->i, ".option.sh");
    sprintf(optionfilepath, "model/option/%s", optionfile);
    // write option file
    option(optionfilepath, ctx);

    // log file
    sprintf(logfile, "%s.%d.%03d%s", ctx->expname, ctx->nexp, ctx->i, ".log.txt");
    sprintf(logfilepath, "model/log/%s", logfile);

    // run
    strcpy(runcmd, ". model/petsc.env.sh; ");
    sprintf(runcmdpart, "%s%s%s%s%s%s%s", getenv("MPIRUN"), " ./model/metos3d-simpack-", ctx->modname, ".exe ", optionfilepath, " > ", logfilepath);
    strcat(runcmd, runcmdpart);
    PetscPrintf(ctx->comm, "# run:      %s\n", runcmd);
    status = system(runcmd);
    
    // read result from scratch
    strcpy(readpath, getenv("SCRATCH"));
    for (i=0; i<ctx->nt; i++) {
        sprintf(readfilepath, "%s%04d%s", readpath, i, "-N.petsc");
        if (i%500==0) PetscPrintf(ctx->comm, "# %s\n", readfilepath);
        PetscViewerBinaryOpen(ctx->comm, readfilepath, FILE_MODE_READ, &viewer);
        VecLoad(y[i], viewer);
        PetscViewerDestroy(&viewer);
    }

    // clean scratch
    sprintf(cleancmd, "%s%s%s", "rm ", readpath, "*");
    PetscPrintf(ctx->comm, "# clean:    %s\n", cleancmd);
    status = system(cleancmd);

    return(0);
}


