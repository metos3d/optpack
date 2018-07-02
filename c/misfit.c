#include "misfit.h"

// misfit
#undef  __FUNCT__
#define __FUNCT__ "misfit"
PetscScalar misfit(void) {
    PetscScalar Jan;
    PetscScalar Jjan, Jfeb, Jmar, Japr, Jmay, Jjun, Jjul, Jaug, Jsep, Joct, Jnov, Jdec;
    PetscScalar Jwin, Jspr, Jsum, Jaut;
    Vec         ydiff;
    PetscScalar norm;
    
    // ydiff
    VecDuplicate(ctx.y[0], &ydiff);
    
    // annual
    VecWAXPY(ydiff, -1.0, ctx.y[0], ctx.yd[0]); VecNorm(ydiff, NORM_2, &norm); Jan = 0.5 * norm*norm;
    
    // monthly
    VecWAXPY(ydiff, -1.0, ctx.y[1], ctx.yd[1]);   VecNorm(ydiff, NORM_2, &norm); Jjan = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[2], ctx.yd[2]);   VecNorm(ydiff, NORM_2, &norm); Jfeb = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[3], ctx.yd[3]);   VecNorm(ydiff, NORM_2, &norm); Jmar = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[4], ctx.yd[4]);   VecNorm(ydiff, NORM_2, &norm); Japr = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[5], ctx.yd[5]);   VecNorm(ydiff, NORM_2, &norm); Jmay = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[6], ctx.yd[6]);   VecNorm(ydiff, NORM_2, &norm); Jjun = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[7], ctx.yd[7]);   VecNorm(ydiff, NORM_2, &norm); Jjul = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[8], ctx.yd[8]);   VecNorm(ydiff, NORM_2, &norm); Jaug = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[9], ctx.yd[9]);   VecNorm(ydiff, NORM_2, &norm); Jsep = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[10], ctx.yd[10]); VecNorm(ydiff, NORM_2, &norm); Joct = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[11], ctx.yd[11]); VecNorm(ydiff, NORM_2, &norm); Jnov = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[12], ctx.yd[12]); VecNorm(ydiff, NORM_2, &norm); Jdec = 0.5 * norm*norm;
    
    // seasonal
    VecWAXPY(ydiff, -1.0, ctx.y[13], ctx.yd[13]); VecNorm(ydiff, NORM_2, &norm); Jwin = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[14], ctx.yd[14]); VecNorm(ydiff, NORM_2, &norm); Jspr = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[15], ctx.yd[15]); VecNorm(ydiff, NORM_2, &norm); Jsum = 0.5 * norm*norm;
    VecWAXPY(ydiff, -1.0, ctx.y[16], ctx.yd[16]); VecNorm(ydiff, NORM_2, &norm); Jaut = 0.5 * norm*norm;
    
    // clean up
    VecDestroy(&ydiff);

    // store in J
    VecSetValue(ctx.J, 0, Jan, INSERT_VALUES);
    VecSetValue(ctx.J, 1, Jjan, INSERT_VALUES);
    VecSetValue(ctx.J, 2, Jfeb, INSERT_VALUES);
    VecSetValue(ctx.J, 3, Jmar, INSERT_VALUES);
    VecSetValue(ctx.J, 4, Japr, INSERT_VALUES);
    VecSetValue(ctx.J, 5, Jmay, INSERT_VALUES);
    VecSetValue(ctx.J, 6, Jjun, INSERT_VALUES);
    VecSetValue(ctx.J, 7, Jjul, INSERT_VALUES);
    VecSetValue(ctx.J, 8, Jaug, INSERT_VALUES);
    VecSetValue(ctx.J, 9, Jsep, INSERT_VALUES);
    VecSetValue(ctx.J, 10, Joct, INSERT_VALUES);
    VecSetValue(ctx.J, 11, Jnov, INSERT_VALUES);
    VecSetValue(ctx.J, 12, Jdec, INSERT_VALUES);
    VecSetValue(ctx.J, 13, Jwin, INSERT_VALUES);
    VecSetValue(ctx.J, 14, Jspr, INSERT_VALUES);
    VecSetValue(ctx.J, 15, Jsum, INSERT_VALUES);
    VecSetValue(ctx.J, 16, Jaut, INSERT_VALUES);

    // info
    printf("# J:        %.16e\n", Jan);
    return Jan;
}




