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
#define __FUNCT__ "objective"
PetscErrorCode objective(Tao tao, Vec u, PetscReal *J, void *objctx) {
    context *ctx = (context*)objctx;
    Vec *y;

    PetscPrintf(ctx->comm, "# i:        %03d\n", ctx->i);

    // create intermediate storage for state trajectory
    VecDuplicateVecs(ctx->y[0], ctx->nt, &y);
    
    model(y, u, ctx);
    misfit(J, y, ctx);
//    *J = 0.0;
    
    // free trajectory
    VecDestroyVecs(ctx->nt, &y);

    store(ctx);
    ctx->i = ctx->i+1;
    
    return(0);
}


