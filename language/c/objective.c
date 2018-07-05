#include "objective.h"

#undef  __FUNCT__
#define __FUNCT__ "objective"
PetscErrorCode objective(Tao tao, Vec u, PetscReal *J, void *objctx) {
    context *ctx = (context*)objctx;

    PetscPrintf(ctx->comm, "# i:        %03d\n", ctx->i);

    model(ctx->y, u, ctx);
    misfit(J, ctx->y, ctx);
//    *J = 0.0;
    
    store(ctx);
    ctx->i = ctx->i+1;
    
    return(0);
}


