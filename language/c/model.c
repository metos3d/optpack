#include "model.h"

#undef  __FUNCT__
#define __FUNCT__ "model"
Vec* model(Vec u, context *ctx) {
    int i, start;
    const PetscScalar* uarr;
    
    // info
    VecGetArrayRead(u, &uarr);
    for(i=0; i<ctx->nu; i++) PetscPrintf(ctx->comm, "# u:        %.16e\n", uarr[i]);
    VecRestoreArrayRead(u, &uarr);
    
    // store current parameter set
    VecCopy(u, ctx->u);
    
    // option file
    char optionfile[PETSC_MAX_PATH_LEN];
    char optionfilepath[PETSC_MAX_PATH_LEN];
    sprintf(optionfile, "%s.%s.%d.%03d%s", ctx->expname, ctx->modname, ctx->nexp, ctx->i, ".option.sh");
    sprintf(optionfilepath, "%s/option/%s", ctx->expname, optionfile);
    // write option file
    option(optionfilepath, ctx);
    
//    // run
//    int status;
//    char logfile[PETSC_MAX_PATH_LEN];
//    char runcmd[PETSC_MAX_PATH_LEN];
//    char work[PETSC_MAX_PATH_LEN];
//    sprintf(logfile, "%03d%s", ctx.i, ".log.txt");
//    strcpy(runcmd, "");
//    strcat(runcmd, "cd model; ");
//    strcat(runcmd, ". ../../../../../../petsc/de.uni-kiel.rz.nesh-fe.petsc-3.3-p7.opt.sh; ");
//    sprintf(work, "%s%s%s%s%s%s%s%s", "mpiexec ", getenv("NQSII_MPIOPTS"), " ./metos3d-simpack-", ctx.modname, ".exe option/", optionfile, " > work/", logfile);
//    strcat(runcmd, work);
//    PetscPrintf(ctx.comm, "# runcmd:   %s\n", runcmd);
//    status = system(runcmd);
//    PetscPrintf(ctx.comm, "# status:   %d\n", status);
    
//    // load
//    Vec *yw;
//    PetscViewer viewer;
//    char filepath[PETSC_MAX_PATH_LEN];
//    VecDuplicateVecs(ctx.yd[0], ctx.nt, &yw);
//    for (i=0; i<ctx.nt; i++) {
//        sprintf(filepath, "%s%04d%s", "/scratch/jpi/", i, "-N.petsc");
////        if (i%500==0) PetscPrintf(ctx.comm, "%s\n", filepath);
//        PetscViewerBinaryOpen(ctx.comm, filepath, FILE_MODE_READ, &viewer);
//        VecLoad(yw[i], viewer);
//        PetscViewerDestroy(&viewer);
//    }

//    // zero y
//    for (i=0; i<ctx.ndata; i++) VecZeroEntries(ctx.y[i]);
//
//    // annual
//    for (i=0; i<ctx.nt; i++) VecAXPY(ctx.y[0], 1.0/(double)ctx.nt, yw[i]);
//
//    // monthly
//    start = 0;    for (i=0; i<240; i++) VecAXPY(ctx.y[1],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 240;  for (i=0; i<240; i++) VecAXPY(ctx.y[2],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 480;  for (i=0; i<240; i++) VecAXPY(ctx.y[3],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 720;  for (i=0; i<240; i++) VecAXPY(ctx.y[4],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 960;  for (i=0; i<240; i++) VecAXPY(ctx.y[5],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 1200; for (i=0; i<240; i++) VecAXPY(ctx.y[6],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 1440; for (i=0; i<240; i++) VecAXPY(ctx.y[7],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 1680; for (i=0; i<240; i++) VecAXPY(ctx.y[8],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 1920; for (i=0; i<240; i++) VecAXPY(ctx.y[9],  12.0/(double)ctx.nt, yw[start+i]);
//    start = 2160; for (i=0; i<240; i++) VecAXPY(ctx.y[10], 12.0/(double)ctx.nt, yw[start+i]);
//    start = 2400; for (i=0; i<240; i++) VecAXPY(ctx.y[11], 12.0/(double)ctx.nt, yw[start+i]);
//    start = 2640; for (i=0; i<240; i++) VecAXPY(ctx.y[12], 12.0/(double)ctx.nt, yw[start+i]);
//
//    // seasonal
//    start = 0;    for (i=0; i<720; i++) VecAXPY(ctx.y[13], 4.0/(double)ctx.nt, yw[start+i]);
//    start = 720;  for (i=0; i<720; i++) VecAXPY(ctx.y[14], 4.0/(double)ctx.nt, yw[start+i]);
//    start = 1440; for (i=0; i<720; i++) VecAXPY(ctx.y[15], 4.0/(double)ctx.nt, yw[start+i]);
//    start = 2160; for (i=0; i<720; i++) VecAXPY(ctx.y[16], 4.0/(double)ctx.nt, yw[start+i]);

//    // clean up
//    VecDestroyVecs(ctx.nt, &yw);
    
    
    return ctx->y;
}


//
//# run
//logfile = ctx.expname + "." + ctx.modname + "." + "{:d}".format(ctx.nexp) + "." + "{:03d}".format(ctx.i) + ".log.txt"
//logfilepath = ctx.expname + "/log/" + logfile
//runcmd = ". model/petsc.env.sh; " + \
//"mpiexec " + os.environ["NQSII_MPIOPTS"] + " ./model/metos3d-simpack-" + ctx.modname + ".exe " + \
//optionfilepath + " > " + logfilepath
//#    print("# run:      " + runcmd, flush=True)
//status = os.system(runcmd)
//
//# read result from scratch
//readpath = os.environ["SCRATCH"]
//nx = ctx.nx
//nt = ctx.nt
//y = np.zeros((nt,nx))       # note, c order
//for i in range(nt):
//filepath = readpath + "{:04d}".format(i) + "-N.petsc"
//#        if i%500==0: print("# {}".format(filepath), flush=True)
//y[i,:] = read_petsc_vector(filepath);
//
//# clean scratch
//cleancmd = "rm " + readpath + "*"
//#    print("# clean:    " + cleancmd, flush=True)
//status = os.system(cleancmd)
//
//return y


//
//% run
//logfile = [ctx.expname '.' ctx.modname '.' num2str(ctx.nexp) '.' num2str(ctx.i, '%03d') '.log.txt'];
//logfilepath = [ctx.expname '/log/' logfile];
//runcmd = ['. model/petsc.env.sh; '...
//          'mpiexec ' getenv('NQSII_MPIOPTS') ' ./model/metos3d-simpack-' ctx.modname '.exe ' ...
//          optionfilepath ' > ' logfilepath];
//disp(['# run:      ' runcmd])
//[status, result] = system(runcmd);
//%    disp(['# status:   ' num2str(status)])
//
//% read result from scratch
//readpath = getenv('SCRATCH');
//nx = ctx.nx;
//nt = ctx.nt;
//y = zeros(nx, nt);
//for i = 1:nt;
//filepath = [readpath num2str(i-1, '%04d') '-N.petsc'];
//%        if mod(i,500)==0 disp(['# ' filepath]), end;
//y(:, i) = readPETScVector(filepath);
//end;
//
//% clean scratch
//cleancmd = ['rm ' readpath '*'];
//disp(['# clean:    ' cleancmd])
//[status, result] = system(cleancmd);
//%    disp(['# status:   ' num2str(status)])
//end

