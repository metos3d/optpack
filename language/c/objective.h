#ifndef OBJECTIVE_H
#define OBJECTIVE_H 1

#include "model.h"
#include "misfit.h"

PetscErrorCode objective(Tao, Vec, PetscReal*, void*);

#endif /* !OBJECTIVE_H */
