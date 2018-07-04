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
    
    // initialization
    OptCtx ctx;

    // optimization context
    ctx.comm     = PETSC_COMM_WORLD;
    ctx.expname  = "twin.c-petsc-tao.blmvm";
    ctx.modname  = "NPZD-DOP";

    ctx.ny       = 5;
    PetscScalar y0[5] = {2.17e+0,1.e-4,1.e-4,1.e-4,1.e-4};
    ctx.y0       = y0;
    
    ctx.yout     = "N.petsc,P.petsc,Z.petsc,D.petsc,DOP.petsc";
    ctx.i        = 1;
    ctx.nu       = 18;
    ctx.nx       = 52749;
    ctx.nt       = 2880;
    ctx.ndata    = 17;

    // info
    system("echo '# date:    ' `date`");
    system("echo '# hostname:' `hostname`");
    system("echo '# PPID:    ' $PPID");
    
    // parameter
    PetscScalar UD[] = {0.02, 0.48, 2.0, 2.0, 0.5,  0.088, 30.0, 0.75, 0.67, 0.04, 0.03,  3.2, 0.01,  0.01,  0.05,  0.5,  0.058, 0.0};     // not used, info only
    PetscScalar u0[] = {0.04, 0.60, 3.5, 3.5, 0.3,  0.100, 25.0, 0.5,  0.4,  0.07, 0.02,  2.0, 0.007, 0.007, 0.075, 0.8,  0.04,  0.0};
    PetscScalar LB[] = {0.01, 0.24, 1.0, 1.0, 0.25, 0.044, 15.0, 0.05, 0.05, 0.02, 0.015, 1.6, 0.005, 0.005, 0.025, 0.25, 0.029, 0.0};
    PetscScalar UB[] = {0.05, 0.72, 4.0, 4.0, 1.0,  0.176, 60.0, 0.95, 0.95, 0.08, 0.045, 4.8, 0.015, 0.015, 0.1,   1.0,  0.087, 0.0};
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

    // prepare y, yd
    init();

    // initial log file creation
    PetscViewer viewer;
    sprintf(ctx.logfile, "%s.%s.h5", ctx.expname, ctx.modname);
    PetscViewerHDF5Open(ctx.comm, ctx.logfile, FILE_MODE_WRITE, &viewer);
    // store ud, lb, ub
    VecView(lb, viewer);
    VecView(ub, viewer);
    VecView(ud, viewer);
    // store yd
    int i;
    for (i=0; i<ctx.ndata; i++) {
        PetscViewerHDF5SetTimestep(viewer, i);
        PetscObjectSetName((PetscObject)ctx.yd[i], "yd");
        VecView(ctx.yd[i], viewer);
    }
    PetscViewerDestroy(&viewer);

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

//% id
//ctx.expname = '{experiment[name]}';
//ctx.nexp    = {nexp};
//
//% algorithm
//options     = [];
//options     = optimset(options, 'algorithm', 'interior-point');
//options     = optimset(options, 'display', 'iter');
//options     = optimset(options, 'maxiter', {niter});
//ctx.options = options;
//
//% model
//ctx.modname = '{model[name]}';
//ctx.ny      = {model[ny]};
//ctx.nx      = {model[nx]};
//ctx.nt      = {model[nt]};
//ctx.y0      = '{model[y0]}';
//ctx.yout    = '{model[yout]}';
//
//% parameters
//ctx.nu      = {parameter[nu]};
//if ctx.nexp == 1;
//% inital experiment, use hard-coded parameter set
//ctx.u0 = {parameter[u0]};
//else;
//% consecutive experiment, load optimal parameter set from previous optimization
//logfile = [ctx.expname filesep ctx.expname '.' ctx.modname '.' num2str(ctx.nexp-1) '.mat'];
//load(logfile, 'uopt');
//ctx.u0 = uopt';   % transposed
//end;
//ctx.ud      = {parameter[ud]};    % not used, info only
//ctx.lb      = {parameter[lb]};
//ctx.ub      = {parameter[ub]};
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


