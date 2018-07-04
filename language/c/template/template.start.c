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

#include "objective.h"

#undef  __FUNCT__
#define __FUNCT__ "main"
int main(int argc, char **args) {
    // petsc
    PetscInitialize(&argc, &args, PETSC_NULL, PETSC_NULL);
    PetscPushErrorHandler(PetscAbortErrorHandler, NULL);
    
    // info
    system("echo '# date:    ' `date`");
    system("echo '# hostname:' `hostname`");
    system("echo '# PPID:    ' $PPID");

    // initialization
    OptCtx ctx;

    // optimization context
    ctx.comm     = PETSC_COMM_WORLD;
    ctx.expname  = "{experiment[name]}";
    ctx.nexp     = {nexp};

    //% algorithm
    //options     = [];
    //options     = optimset(options, 'algorithm', 'interior-point');
    //options     = optimset(options, 'display', 'iter');
    //options     = optimset(options, 'maxiter', {niter});
    //ctx.options = options;

    // model
    ctx.modname = "{model[name]}";
    ctx.ny      = {model[ny]};
    ctx.nx      = {model[nx]};
    ctx.nt      = {model[nt]};
    ctx.y0      = "{model[y0]}";
    ctx.yout    = "{model[yout]}";

    // parameter
    ctx.nu      = {parameter[nu]};
    PetscScalar U0[] = {parameter[u0]};
    PetscScalar UD[] = {parameter[ud]};     // not used, info only
    PetscScalar LB[] = {parameter[lb]};
    PetscScalar UB[] = {parameter[ub]};

//    ctx.u0      = {};
//    ctx.ud      = {};    % not used, info only
//    ctx.lb      = {};
//    ctx.ub      = {};
    
    // parameter
    Vec u, ud, lb, ub;
    VecCreate(ctx.comm, &u);
    VecCreate(ctx.comm, &ud);
    VecCreate(ctx.comm, &lb);
    VecCreate(ctx.comm, &ub);
    VecSetType(u, VECSTANDARD);
    VecSetType(ud, VECSTANDARD);
    VecSetType(lb, VECSTANDARD);
    VecSetType(ub, VECSTANDARD);
    VecSetSizes(u, PETSC_DECIDE, ctx.nu);
    VecSetSizes(ud, PETSC_DECIDE, ctx.nu);
    VecSetSizes(lb, PETSC_DECIDE, ctx.nu);
    VecSetSizes(ub, PETSC_DECIDE, ctx.nu);
    VecPlaceArray(u, u0);
    VecPlaceArray(ud, UD);
    VecPlaceArray(lb, LB);
    VecPlaceArray(ub, UB);
    PetscObjectSetName((PetscObject)ud, "ud");
    PetscObjectSetName((PetscObject)lb, "lb");
    PetscObjectSetName((PetscObject)ub, "ub");
    // for storage
    VecDuplicate(u, &ctx.u);
    PetscObjectSetName((PetscObject)ctx.u, "u");

//    ctx.i        = 1;
//    ctx.ndata    = 17;

//    // prepare y, yd
//    PetscErrorCode init(void) {
//        int i, start;
//        char file[PETSC_MAX_PATH_LEN];
//        Vec* yw;
//        PetscViewer viewer;
//
//        // data, yd
//        Vec ydw;
//        VecCreate(ctx.comm, &ydw);
//        VecSetType(ydw, VECSTANDARD);
//        VecSetSizes(ydw, PETSC_DECIDE, ctx.nx);
//        VecDuplicateVecs(ydw, ctx.ndata, &ctx.yd);
//        VecDestroy(&ydw);
//        // state, y
//        VecDuplicateVecs(ctx.yd[0], ctx.ndata, &ctx.y);
//        // misfit, J
//        VecCreate(ctx.comm, &ctx.J);
//        VecSetType(ctx.J, VECSTANDARD);
//        PetscObjectSetName((PetscObject)ctx.J, "J");
//        VecSetSizes(ctx.J, PETSC_DECIDE, 17);
//
//        // load trajectory in yw
//        VecDuplicateVecs(ctx.yd[0], ctx.nt, &yw);
//        for (i=0; i<ctx.nt; i++) {
//            sprintf(file, "%s%s%s%04d%s", "../../../../twin-data/", ctx.modname, "/work/", i, "-N.petsc");
//            //        if (i%500==0) PetscPrintf(ctx.comm, "%s\n", file);
//            PetscViewerBinaryOpen(ctx.comm, file, FILE_MODE_READ, &viewer);
//            VecLoad(yw[i], viewer);
//            PetscViewerDestroy(&viewer);
//        }
//
//        // yd
//        for (i=0; i<ctx.ndata; i++) VecZeroEntries(ctx.yd[i]);
//
//        // 00, annual
//        // 01 - 12, monthly
//        // 13 - 16, seasonal
//        // annual
//        for (i=0; i<ctx.nt; i++) VecAXPY(ctx.yd[0], 1.0/(double)ctx.nt, yw[i]);
//        // monthly
//        start = 0;    for (i=0; i<240; i++) VecAXPY(ctx.yd[1],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 240;  for (i=0; i<240; i++) VecAXPY(ctx.yd[2],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 480;  for (i=0; i<240; i++) VecAXPY(ctx.yd[3],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 720;  for (i=0; i<240; i++) VecAXPY(ctx.yd[4],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 960;  for (i=0; i<240; i++) VecAXPY(ctx.yd[5],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 1200; for (i=0; i<240; i++) VecAXPY(ctx.yd[6],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 1440; for (i=0; i<240; i++) VecAXPY(ctx.yd[7],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 1680; for (i=0; i<240; i++) VecAXPY(ctx.yd[8],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 1920; for (i=0; i<240; i++) VecAXPY(ctx.yd[9],  12.0/(double)ctx.nt, yw[start+i]);
//        start = 2160; for (i=0; i<240; i++) VecAXPY(ctx.yd[10], 12.0/(double)ctx.nt, yw[start+i]);
//        start = 2400; for (i=0; i<240; i++) VecAXPY(ctx.yd[11], 12.0/(double)ctx.nt, yw[start+i]);
//        start = 2640; for (i=0; i<240; i++) VecAXPY(ctx.yd[12], 12.0/(double)ctx.nt, yw[start+i]);
//        // seasonal
//        start = 0;    for (i=0; i<720; i++) VecAXPY(ctx.yd[13], 4.0/(double)ctx.nt, yw[start+i]);
//        start = 720;  for (i=0; i<720; i++) VecAXPY(ctx.yd[14], 4.0/(double)ctx.nt, yw[start+i]);
//        start = 1440; for (i=0; i<720; i++) VecAXPY(ctx.yd[15], 4.0/(double)ctx.nt, yw[start+i]);
//        start = 2160; for (i=0; i<720; i++) VecAXPY(ctx.yd[16], 4.0/(double)ctx.nt, yw[start+i]);
//
//        // clean up yw
//        VecDestroyVecs(ctx.nt, &yw);
//        return(0);
//    }


    // tao, optimizer
    Tao tao;
    TaoCreate(ctx.comm, &tao);
    TaoSetType(tao, TAOBLMVM);

    // parameter vector, bounds
    TaoSetInitialVector(tao, u);
    TaoSetVariableBounds(tao, lb, ub);

    // objective
    TaoSetObjectiveRoutine(tao, objective, PETSC_NULL);
    TaoSetGradientRoutine(tao, TaoDefaultComputeGradient, PETSC_NULL);

    // optimization
    TaoSetFromOptions(tao);
    TaoSolve(tao);
    TaoView(tao, PETSC_VIEWER_STDOUT_WORLD);

    // clean up
    TaoDestroy(&tao);
    VecDestroyVecs(ctx.ndata, &ctx.y);
    VecDestroyVecs(ctx.ndata, &ctx.yd);
    VecDestroy(&ctx.u);
    VecDestroy(&ctx.J);
    VecResetArray(u);
    VecResetArray(ud);
    VecResetArray(lb);
    VecResetArray(ub);
    VecDestroy(&u);
    VecDestroy(&ud);
    VecDestroy(&lb);
    VecDestroy(&ub);
    PetscPopErrorHandler();
    PetscFinalize();
    return 0;
}

//
//
//% data
//ctx.ndata   = 17;
//ctx.yd      = data(ctx);
//
//% optimization
//ctx.i       = 1;
//
//% log
//ctx.logfile = [ctx.expname filesep ctx.expname '.' ctx.modname '.' num2str(ctx.nexp) '.mat'];
//
//% optimization
//[uopt,Jopt,exitflag,output,lambda,grad,hessian] = fmincon(@(u) objective(u,ctx),ctx.u0,[],[],[],[],ctx.lb,ctx.ub,[],ctx.options)
//
//% store optimal parameter set and corresponding objective value
//uopt = uopt';   % transposed
//save(ctx.logfile, 'uopt', 'Jopt', '-append')


//
// store
//


//    // initial log file creation
//    PetscViewer viewer;
//    sprintf(ctx.logfile, "%s.%s.h5", ctx.expname, ctx.modname);
//    PetscViewerHDF5Open(ctx.comm, ctx.logfile, FILE_MODE_WRITE, &viewer);
//    // store ud, lb, ub
//    VecView(lb, viewer);
//    VecView(ub, viewer);
//    VecView(ud, viewer);
//    // store yd
//    int i;
//    for (i=0; i<ctx.ndata; i++) {
//        PetscViewerHDF5SetTimestep(viewer, i);
//        PetscObjectSetName((PetscObject)ctx.yd[i], "yd");
//        VecView(ctx.yd[i], viewer);
//    }
//    PetscViewerDestroy(&viewer);
