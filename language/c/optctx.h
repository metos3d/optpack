#ifndef CONTEXT_H
#define CONTEXT_H 1

#include <petsctao.h>
#include "petscviewerhdf5.h"

// context data type
typedef struct {
    // petsc specific
    MPI_Comm        comm;
    // id
    char            *expname;       // name of experiment
    PetscInt        nexp;           // experiment number
    // model
    char            *modname;       // name of bgc model
    PetscInt        ny;             // number of tracers
    PetscInt        nx;             // vector length
    PetscInt        nt;             // number of time steps
    char            *y0;            // initial concentration
    char            *yout;          // output file name(s)
    // parameters
    PetscInt        nu;             // number of parameters
    Vec             u0;             // initial parameter set
    Vec             ud;             // reference parameter set
    Vec             lb;             // lower bounds
    Vec             ub;             // upper bounds
    // data
    PetscInt        ndata;          // number of data
    Vec             *yd;            // data
    // optimization
//    void            *options;       // algorithm options
    PetscInt        i;              // current iteration
    Vec             u;              // current parameter set
    Vec             *y;             // current state
    Vec             J;              // current objective
    // log
    char            logfile[PETSC_MAX_PATH_LEN];    // name of log file
} Context;

#endif /* !CONTEXT_H */


