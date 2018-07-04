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

#include "data.h"
#include "objective.h"

#undef  __FUNCT__
#define __FUNCT__ "main"
int main(int argc, char **args) {{
    // petsc
    PetscInitialize(&argc, &args, PETSC_NULL, PETSC_NULL);
    PetscPushErrorHandler(PetscAbortErrorHandler, NULL);
    
    // info
    system("echo '# date:    ' `date`");
    system("echo '# hostname:' `hostname`");
    system("echo '# PPID:    ' $PPID");

    // initialization
    context ctxobj;
    context *ctx = &ctxobj;

    // optimization context
    ctx->comm       = PETSC_COMM_WORLD;
    ctx->expname    = "{experiment[name]}";
    ctx->nexp       = {nexp};

    // tao, optimizer, algorithm
    Tao tao;
    TaoCreate(ctx->comm, &tao);
    TaoSetType(tao, TAOBLMVM);
    TaoSetMaximumIterations(tao, {niter});
    TaoSetMonitor(tao, TaoMonitorDefault, NULL, NULL);

    // model
    ctx->modname    = "{model[name]}";
    ctx->ny         = {model[ny]};
    ctx->nx         = {model[nx]};
    ctx->nt         = {model[nt]};
    ctx->y0         = "{model[y0]}";
    ctx->yout       = "{model[yout]}";

    // parameter
    Vec u;
    ctx->nu = {parameter[nu]};
    PetscScalar U0[] = {{{parameter[u0]}}};
    PetscScalar UD[] = {{{parameter[ud]}}};     // not used, info only
    PetscScalar LB[] = {{{parameter[lb]}}};
    PetscScalar UB[] = {{{parameter[ub]}}};
    VecCreate(ctx->comm, &u);
    VecCreate(ctx->comm, &ctx->ud);
    VecCreate(ctx->comm, &ctx->lb);
    VecCreate(ctx->comm, &ctx->ub);
    VecSetType(u, VECSTANDARD);
    VecSetType(ctx->ud, VECSTANDARD);
    VecSetType(ctx->lb, VECSTANDARD);
    VecSetType(ctx->ub, VECSTANDARD);
    VecSetSizes(u, PETSC_DECIDE, ctx->nu);
    VecSetSizes(ctx->ud, PETSC_DECIDE, ctx->nu);
    VecSetSizes(ctx->lb, PETSC_DECIDE, ctx->nu);
    VecSetSizes(ctx->ub, PETSC_DECIDE, ctx->nu);
    VecPlaceArray(u, U0);
    VecPlaceArray(ctx->ud, UD);
    VecPlaceArray(ctx->lb, LB);
    VecPlaceArray(ctx->ub, UB);
    PetscObjectSetName((PetscObject)u, "u");
    PetscObjectSetName((PetscObject)ctx->ud, "ud");
    PetscObjectSetName((PetscObject)ctx->lb, "lb");
    PetscObjectSetName((PetscObject)ctx->ub, "ub");
    VecDuplicate(u, &ctx->u);
    PetscObjectSetName((PetscObject)ctx->u, "u");

    // data
    ctx->ndata  = 17;
    ctx->yd     = data(ctx);

    // optimization
    ctx->i      = 1;
    // state, y
    VecDuplicateVecs(ctx->yd[0], ctx->ndata, &ctx->y);
    // misfit, J
    VecCreate(ctx->comm, &ctx->J);
    VecSetType(ctx->J, VECSTANDARD);
    PetscObjectSetName((PetscObject)ctx->J, "J");
    VecSetSizes(ctx->J, PETSC_DECIDE, ctx->ndata);

    // log
    sprintf(ctx->logfile, "%s/%s.%s.%d.h5", ctx->expname, ctx->expname, ctx->modname, ctx->nexp);

    // parameter vector, bounds
    TaoSetInitialVector(tao, u);
    TaoSetVariableBounds(tao, ctx->lb, ctx->ub);

    // objective
    TaoSetObjectiveRoutine(tao, objective, (void*)ctx);
    TaoSetGradientRoutine(tao, TaoDefaultComputeGradient, PETSC_NULL);

    // optimization
    TaoSetFromOptions(tao);
    TaoSolve(tao);
    TaoView(tao, PETSC_VIEWER_STDOUT_WORLD);

    // clean up
    TaoDestroy(&tao);
    VecDestroyVecs(ctx->ndata, &ctx->y);
    VecDestroyVecs(ctx->ndata, &ctx->yd);
    VecDestroy(&ctx->J);
    VecResetArray(ctx->u);
    VecResetArray(ctx->ud);
    VecResetArray(ctx->lb);
    VecResetArray(ctx->ub);
    VecDestroy(&ctx->u);
    VecDestroy(&ctx->ud);
    VecDestroy(&ctx->lb);
    VecDestroy(&ctx->ub);
    VecDestroy(&u);
    // petsc
    PetscPopErrorHandler();
    PetscFinalize();
    return 0;
}}


//% store optimal parameter set and corresponding objective value
//uopt = uopt';   % transposed
//save(ctx.logfile, 'uopt', 'Jopt', '-append')

//PetscErrorCode TaoGetSolutionVector(Tao tao, Vec *X)
//PetscErrorCode TaoGetSolutionStatus(Tao tao, PetscInt *its, PetscReal *f, PetscReal *gnorm, PetscReal *cnorm, PetscReal *xdiff, TaoConvergedReason *reason)
//


