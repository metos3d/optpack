#include "data.h"
#include "average.h"

// data
#undef  __FUNCT__
#define __FUNCT__ "data"
Vec* data(context* ctx) {
    int i;
    char file_path[PETSC_MAX_PATH_LEN];
    Vec ytmp, *yt, *yd;
    PetscViewer viewer;
    
    // create y, template
    VecCreate(ctx->comm, &ytmp);
    VecSetType(ytmp, VECSTANDARD);
    VecSetSizes(ytmp, PETSC_DECIDE, ctx->nx);
    
    // create yt, trajectory
    VecDuplicateVecs(ytmp, ctx->nt, &yt);
    
    // load trajectory
    for (i=0; i<ctx->nt; i++) {
        sprintf(file_path, "%s%s%s%04d%s", "../../../../twin-data/", ctx->modname, "/work/", i, "-N.petsc");
        if (i%500==0) PetscPrintf(ctx->comm, "%s\n", file);
        PetscViewerBinaryOpen(ctx->comm, file_path, FILE_MODE_READ, &viewer);
        VecLoad(yt[i], viewer);
        PetscViewerDestroy(&viewer);
    }
    yd = average(y, ctx);
    
    // free yt, ytmp
    VecDestroyVecs(ctx->nt, &yt);
    VecDestroy(&ytmp);
    
    return yd;
}


