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
} context;

#endif /* !CONTEXT_H */


