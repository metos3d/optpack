#ifndef OPTCTX_H
#define OPTCTX_H 1

#include <petsctao.h>
#include "petscviewerhdf5.h"

// context data type
typedef struct {
    MPI_Comm        comm;
    char            *expname;
    char            *modname;
    PetscScalar     *y0;
    char            *yout;
    PetscInt        i;
    PetscInt        nu;
    Vec             u;
    PetscInt        ny;
    PetscInt        nx;
    PetscInt        nt;
    Vec             *y;
    PetscInt        ndata;
    Vec             *yd;
    Vec             J;
    // log
    char            logfile[PETSC_MAX_PATH_LEN];
} OptCtx;

// global context
OptCtx ctx;

#endif /* !OPTCTX_H */
