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
        sprintf(file_path, "%s%s%s%04d%s", "../../twin-data/", ctx->modname, "/work/", i, "-N.petsc");
//        if (i%500==0) PetscPrintf(ctx->comm, "# %s\n", file_path);
        PetscViewerBinaryOpen(ctx->comm, file_path, FILE_MODE_READ, &viewer);
        VecLoad(yt[i], viewer);
        PetscViewerDestroy(&viewer);
    }
    average(yd, yt, ctx);    
    VecDestroyVecs(ctx->nt, &yt);
    
    return(0);
}


