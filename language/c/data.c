#include "data.h"
#include "average.h"

// data
#undef  __FUNCT__
#define __FUNCT__ "data"
PetscErrorCode data(Vec *yd, context *ctx) {
    int i;
    char file_path[PETSC_MAX_PATH_LEN];
    Vec *yt;
    PetscViewer viewer;

    // load trajectory
    VecDuplicateVecs(yd[0], ctx->nt, &yt);
    for (i=0; i<ctx->nt; i++) {
        sprintf(file_path, "%s%s%s%04d%s", "../../../../twin-data/", ctx->modname, "/work/", i, "-N.petsc");
        if (i%500==0) PetscPrintf(ctx->comm, "# %s\n", file_path);
        PetscViewerBinaryOpen(ctx->comm, file_path, FILE_MODE_READ, &viewer);
        VecLoad(yt[i], viewer);
        PetscViewerDestroy(&viewer);
    }
    average(yd, yt, ctx);    
    VecDestroyVecs(ctx->nt, &yt);
    
    return(0);
}


