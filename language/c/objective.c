#include "objective.h"

#undef  __FUNCT__
#define __FUNCT__ "objective"
PetscErrorCode objective(Tao tao, Vec u, PetscReal *J, void *objctx) {
    context *ctx = (context*)objctx;
    Vec *y;

    PetscPrintf(ctx->comm, "# i:        %03d\n", ctx->i);

    y = model(u, ctx);
    *J = misfit(y, ctx);
//    *J = 0.0;
    
    store(ctx);
    ctx->i = ctx->i+1;
    
    return(0);
}


