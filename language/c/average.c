#include "average.h"

// average
#undef  __FUNCT__
#define __FUNCT__ "average"
PetscErrorCode average(Vec *yavg, Vec *y, context *ctx) {
    int i, start;
    
    // zero yd
    for (i=0; i<ctx->ndata; i++) VecZeroEntries(yavg[i]);
    
    // 00, annual
    // 01 - 12, monthly
    // 13 - 16, seasonal
    // annual
    for (i=0; i<ctx->nt; i++) VecAXPY(yavg[0], 1.0/(double)ctx->nt, y[i]);
    
    // monthly
    start = 0;    for (i=0; i<240; i++) VecAXPY(yavg[1],  12.0/(double)ctx->nt, y[start+i]);
    start = 240;  for (i=0; i<240; i++) VecAXPY(yavg[2],  12.0/(double)ctx->nt, y[start+i]);
    start = 480;  for (i=0; i<240; i++) VecAXPY(yavg[3],  12.0/(double)ctx->nt, y[start+i]);
    start = 720;  for (i=0; i<240; i++) VecAXPY(yavg[4],  12.0/(double)ctx->nt, y[start+i]);
    start = 960;  for (i=0; i<240; i++) VecAXPY(yavg[5],  12.0/(double)ctx->nt, y[start+i]);
    start = 1200; for (i=0; i<240; i++) VecAXPY(yavg[6],  12.0/(double)ctx->nt, y[start+i]);
    start = 1440; for (i=0; i<240; i++) VecAXPY(yavg[7],  12.0/(double)ctx->nt, y[start+i]);
    start = 1680; for (i=0; i<240; i++) VecAXPY(yavg[8],  12.0/(double)ctx->nt, y[start+i]);
    start = 1920; for (i=0; i<240; i++) VecAXPY(yavg[9],  12.0/(double)ctx->nt, y[start+i]);
    start = 2160; for (i=0; i<240; i++) VecAXPY(yavg[10], 12.0/(double)ctx->nt, y[start+i]);
    start = 2400; for (i=0; i<240; i++) VecAXPY(yavg[11], 12.0/(double)ctx->nt, y[start+i]);
    start = 2640; for (i=0; i<240; i++) VecAXPY(yavg[12], 12.0/(double)ctx->nt, y[start+i]);
    
    // seasonal
    start = 0;    for (i=0; i<720; i++) VecAXPY(yavg[13], 4.0/(double)ctx->nt, y[start+i]);
    start = 720;  for (i=0; i<720; i++) VecAXPY(yavg[14], 4.0/(double)ctx->nt, y[start+i]);
    start = 1440; for (i=0; i<720; i++) VecAXPY(yavg[15], 4.0/(double)ctx->nt, y[start+i]);
    start = 2160; for (i=0; i<720; i++) VecAXPY(yavg[16], 4.0/(double)ctx->nt, y[start+i]);

    return(0);
}


