#include "model.h"

#undef  __FUNCT__
#define __FUNCT__ "model"
PetscErrorCode model(Vec u) {
    int i, start;
    const PetscScalar* uarr;
    
    // info
    VecGetArrayRead(u, &uarr);
    for(i=0; i<ctx.nu; i++) PetscPrintf(ctx.comm, "# u:        %.16e\n", uarr[i]);
    VecRestoreArrayRead(u, &uarr);
    
    // option file
    char optionfile[PETSC_MAX_PATH_LEN];
    char optionpath[PETSC_MAX_PATH_LEN];
    sprintf(optionfile, "%03d%s", ctx.i, ".option.sh");
    sprintf(optionpath, "%s%s", "model/option/", optionfile);
    PetscPrintf(ctx.comm, "# option:   %s\n", optionpath);
    // write option file
    option(u, optionpath);
    
    // run
    int status;
    char logfile[PETSC_MAX_PATH_LEN];
    char runcmd[PETSC_MAX_PATH_LEN];
    char work[PETSC_MAX_PATH_LEN];
    sprintf(logfile, "%03d%s", ctx.i, ".log.txt");
    strcpy(runcmd, "");
    strcat(runcmd, "cd model; ");
    strcat(runcmd, ". ../../../../../../petsc/de.uni-kiel.rz.nesh-fe.petsc-3.3-p7.opt.sh; ");
    sprintf(work, "%s%s%s%s%s%s%s%s", "mpiexec ", getenv("NQSII_MPIOPTS"), " ./metos3d-simpack-", ctx.modname, ".exe option/", optionfile, " > work/", logfile);
    strcat(runcmd, work);
    PetscPrintf(ctx.comm, "# runcmd:   %s\n", runcmd);
    status = system(runcmd);
    PetscPrintf(ctx.comm, "# status:   %d\n", status);
    
    // load
    Vec *yw;
    PetscViewer viewer;
    char filepath[PETSC_MAX_PATH_LEN];
    VecDuplicateVecs(ctx.yd[0], ctx.nt, &yw);
    for (i=0; i<ctx.nt; i++) {
        sprintf(filepath, "%s%04d%s", "/scratch/jpi/", i, "-N.petsc");
//        if (i%500==0) PetscPrintf(ctx.comm, "%s\n", filepath);
        PetscViewerBinaryOpen(ctx.comm, filepath, FILE_MODE_READ, &viewer);
        VecLoad(yw[i], viewer);
        PetscViewerDestroy(&viewer);
    }

    // zero y
    for (i=0; i<ctx.ndata; i++) VecZeroEntries(ctx.y[i]);
    
    // annual
    for (i=0; i<ctx.nt; i++) VecAXPY(ctx.y[0], 1.0/(double)ctx.nt, yw[i]);
    
    // monthly
    start = 0;    for (i=0; i<240; i++) VecAXPY(ctx.y[1],  12.0/(double)ctx.nt, yw[start+i]);
    start = 240;  for (i=0; i<240; i++) VecAXPY(ctx.y[2],  12.0/(double)ctx.nt, yw[start+i]);
    start = 480;  for (i=0; i<240; i++) VecAXPY(ctx.y[3],  12.0/(double)ctx.nt, yw[start+i]);
    start = 720;  for (i=0; i<240; i++) VecAXPY(ctx.y[4],  12.0/(double)ctx.nt, yw[start+i]);
    start = 960;  for (i=0; i<240; i++) VecAXPY(ctx.y[5],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1200; for (i=0; i<240; i++) VecAXPY(ctx.y[6],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1440; for (i=0; i<240; i++) VecAXPY(ctx.y[7],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1680; for (i=0; i<240; i++) VecAXPY(ctx.y[8],  12.0/(double)ctx.nt, yw[start+i]);
    start = 1920; for (i=0; i<240; i++) VecAXPY(ctx.y[9],  12.0/(double)ctx.nt, yw[start+i]);
    start = 2160; for (i=0; i<240; i++) VecAXPY(ctx.y[10], 12.0/(double)ctx.nt, yw[start+i]);
    start = 2400; for (i=0; i<240; i++) VecAXPY(ctx.y[11], 12.0/(double)ctx.nt, yw[start+i]);
    start = 2640; for (i=0; i<240; i++) VecAXPY(ctx.y[12], 12.0/(double)ctx.nt, yw[start+i]);
    
    // seasonal
    start = 0;    for (i=0; i<720; i++) VecAXPY(ctx.y[13], 4.0/(double)ctx.nt, yw[start+i]);
    start = 720;  for (i=0; i<720; i++) VecAXPY(ctx.y[14], 4.0/(double)ctx.nt, yw[start+i]);
    start = 1440; for (i=0; i<720; i++) VecAXPY(ctx.y[15], 4.0/(double)ctx.nt, yw[start+i]);
    start = 2160; for (i=0; i<720; i++) VecAXPY(ctx.y[16], 4.0/(double)ctx.nt, yw[start+i]);

    // clean up
    VecDestroyVecs(ctx.nt, &yw);
    return(0);
}




