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

#endif /* !OPTCTX_H */

//# id
//expname = []        # name of experiment
//nexp    = []        # experiment number
//# model
//modname = []        # name of bgc model
//ny      = []        # number of tracers
//nx      = []        # vector length
//nt      = []        # number of time steps
//y0      = []        # initial concentration
//yout    = []        # output file name(s)
//# parameters
//nu      = []        # number of parameters
//u0      = []        # initial parameter set
//ud      = []        # reference parameter set
//lb      = []        # lower bounds
//ub      = []        # upper bounds
//# data
//ndata   = []        # number of data
//yd      = []        # data
//# optimization
//options = []        # algorithm options
//i       = []        # current iteration
//u       = []        # current parameter set
//y       = []        # current state
//J       = []        # current objective
//# log
//logfile = []        # name of log file

//% id
//expname = [];       % name of experiment
//nexp    = [];       % experiment number
//% model
//modname = [];       % name of bgc model
//ny      = [];       % number of tracers
//nx      = [];       % vector length
//nt      = [];       % number of time steps
//y0      = [];       % initial concentration
//yout    = [];       % output file name(s)
//% parameters
//nu      = [];       % number of parameters
//u0      = [];       % initial parameter set
//ud      = [];       % reference parameter set
//lb      = [];       % lower bounds
//ub      = [];       % upper bounds
//% data
//ndata   = [];       % number of data
//yd      = [];       % data
//% optimization
//options = [];       % algorithm options
//i       = [];       % current iteration
//u       = [];       % current parameter set
//y       = [];       % current state
//J       = [];       % current objective
//% log
//logfile = [];       % name of log file
