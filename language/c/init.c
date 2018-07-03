#include "init.h"

// init
#undef  __FUNCT__
#define __FUNCT__ "init"
PetscErrorCode init(void) {
    int i, start;
    char file[PETSC_MAX_PATH_LEN];
    Vec* yw;
    PetscViewer viewer;

    // data, yd
    Vec ydw;
    VecCreate(ctx.comm, &ydw);
    VecSetType(ydw, VECSTANDARD);
    VecSetSizes(ydw, PETSC_DECIDE, ctx.nx);
    VecDuplicateVecs(ydw, ctx.ndata, &ctx.yd);
    VecDestroy(&ydw);
    // state, y
    VecDuplicateVecs(ctx.yd[0], ctx.ndata, &ctx.y);
    // misfit, J
    VecCreate(ctx.comm, &ctx.J);
    VecSetType(ctx.J, VECSTANDARD);
    PetscObjectSetName((PetscObject)ctx.J, "J");
    VecSetSizes(ctx.J, PETSC_DECIDE, 17);

    // load trajectory in yw
    VecDuplicateVecs(ctx.yd[0], ctx.nt, &yw);
    for (i=0; i<ctx.nt; i++) {
        sprintf(file, "%s%s%s%04d%s", "../../../../twin-data/", ctx.modname, "/work/", i, "-N.petsc");
//        if (i%500==0) PetscPrintf(ctx.comm, "%s\n", file);
        PetscViewerBinaryOpen(ctx.comm, file, FILE_MODE_READ, &viewer);
        VecLoad(yw[i], viewer);
        PetscViewerDestroy(&viewer);
    }

    // yd
    for (i=0; i<ctx.ndata; i++) VecZeroEntries(ctx.yd[i]);

    // 00, annual
    // 01 - 12, monthly
    // 13 - 16, seasonal
    // annual
    for (i=0; i<ctx.nt; i++) VecAXPY(ctx.yd[0], 1.0/(double)ctx.nt, yw[i]);
    // monthly
    start = 0;    for (i=0; i<240; i++) VecAXPY(ctx.yd[1],  12.0/(double)ctx.nt, yw[start+i]);
    start = 240;  for (i=0; i<240; i++) VecAXPY(ctx.yd[2],  12.0/(double)ctx.nt, yw[start+i]);
    start = 480;  for (i=0; i<240; i++) VecAXPY(ctx.yd[3],  12.0/(double)ctx.nt, yw[start+i]);
    start = 720;  for (i=0; i<240; i++) VecAXPY(ctx.yd[4],  12.0/(double)ctx.nt, yw[start+i]);
    start = 960;  for (i=0; i<240; i++) VecAXPY(ctx.yd[5],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1200; for (i=0; i<240; i++) VecAXPY(ctx.yd[6],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1440; for (i=0; i<240; i++) VecAXPY(ctx.yd[7],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1680; for (i=0; i<240; i++) VecAXPY(ctx.yd[8],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1920; for (i=0; i<240; i++) VecAXPY(ctx.yd[9],  12.0/(double)ctx.nt, yw[start+i]);
    start = 2160; for (i=0; i<240; i++) VecAXPY(ctx.yd[10], 12.0/(double)ctx.nt, yw[start+i]);
    start = 2400; for (i=0; i<240; i++) VecAXPY(ctx.yd[11], 12.0/(double)ctx.nt, yw[start+i]);
    start = 2640; for (i=0; i<240; i++) VecAXPY(ctx.yd[12], 12.0/(double)ctx.nt, yw[start+i]);
    // seasonal
    start = 0;    for (i=0; i<720; i++) VecAXPY(ctx.yd[13], 4.0/(double)ctx.nt, yw[start+i]);
    start = 720;  for (i=0; i<720; i++) VecAXPY(ctx.yd[14], 4.0/(double)ctx.nt, yw[start+i]);
    start = 1440; for (i=0; i<720; i++) VecAXPY(ctx.yd[15], 4.0/(double)ctx.nt, yw[start+i]);
    start = 2160; for (i=0; i<720; i++) VecAXPY(ctx.yd[16], 4.0/(double)ctx.nt, yw[start+i]);

    // clean up yw
    VecDestroyVecs(ctx.nt, &yw);
    return(0);
}





