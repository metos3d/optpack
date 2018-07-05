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
    Vec *yt;
    PetscViewer viewer;
    const PetscScalar* uarr;
    
    // info
    VecGetArrayRead(u, &uarr);
    for(i=0; i<ctx->nu; i++) PetscPrintf(ctx->comm, "# u:        %.16e\n", uarr[i]);
    VecRestoreArrayRead(u, &uarr);
    
    // store current parameter set
    VecCopy(u, ctx->u);
    
    // option file
    sprintf(optionfile, "%s.%s.%d.%03d%s", ctx->expname, ctx->modname, ctx->nexp, ctx->i, ".option.sh");
    sprintf(optionfilepath, "%s/option/%s", ctx->expname, optionfile);
    // write option file
    option(optionfilepath, ctx);

    // log file
    sprintf(logfile, "%s.%s.%d.%03d%s", ctx->expname, ctx->modname, ctx->nexp, ctx->i, ".log.txt");
    sprintf(logfilepath, "%s/log/%s", ctx->expname, logfile);

    // run
    strcpy(runcmd, ". model/petsc.env.sh; ");
    sprintf(runcmdpart, "%s%s%s%s%s%s%s%s", "mpiexec ", getenv("NQSII_MPIOPTS"), " ./model/metos3d-simpack-", ctx->modname, ".exe ", optionfilepath, " > ", logfilepath);
    strcat(runcmd, runcmdpart);
    PetscPrintf(ctx->comm, "# run:      %s\n", runcmd);
    status = system(runcmd);
    
    // read result from scratch
    strcpy(readpath, getenv("SCRATCH"));
    VecDuplicateVecs(ctx->y[0], ctx->nt, &yt);
    for (i=0; i<ctx->nt; i++) {
        sprintf(readfilepath, "%s%04d%s", readpath, i, "-N.petsc");
        if (i%500==0) PetscPrintf(ctx->comm, "# %s\n", readfilepath);
        PetscViewerBinaryOpen(ctx->comm, readfilepath, FILE_MODE_READ, &viewer);
        VecLoad(yt[i], viewer);
        PetscViewerDestroy(&viewer);
    }
    average(y, yt, ctx);
    VecDestroyVecs(ctx->nt, &yt);

    // clean scratch
    sprintf(cleancmd, "%s%s%s", "rm ", readpath, "*");
    PetscPrintf(ctx->comm, "# clean:    %s\n", cleancmd);
    status = system(cleancmd);

    return(0);
}


