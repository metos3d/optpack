#include "average.h"

// average
#undef  __FUNCT__
#define __FUNCT__ "average"
Vec* average(Vec* y, context* ctx) {
    int i, start;
    // zero yd
    for (i=0; i<ctx->ndata; i++) VecZeroEntries(ctx->yd[i]);
    // 00, annual
    // 01 - 12, monthly
    // 13 - 16, seasonal
    // annual
    for (i=0; i<ctx->nt; i++) VecAXPY(ctx->yd[0], 1.0/(double)ctx->nt, y[i]);
    // monthly
    start = 0;    for (i=0; i<240; i++) VecAXPY(ctx->yd[1],  12.0/(double)ctx->nt, y[start+i]);
    start = 240;  for (i=0; i<240; i++) VecAXPY(ctx->yd[2],  12.0/(double)ctx->nt, y[start+i]);
    start = 480;  for (i=0; i<240; i++) VecAXPY(ctx->yd[3],  12.0/(double)ctx->nt, y[start+i]);
    start = 720;  for (i=0; i<240; i++) VecAXPY(ctx->yd[4],  12.0/(double)ctx->nt, y[start+i]);
    start = 960;  for (i=0; i<240; i++) VecAXPY(ctx->yd[5],  12.0/(double)ctx->nt, y[start+i]);
    start = 1200; for (i=0; i<240; i++) VecAXPY(ctx->yd[6],  12.0/(double)ctx->nt, y[start+i]);
    start = 1440; for (i=0; i<240; i++) VecAXPY(ctx->yd[7],  12.0/(double)ctx->nt, y[start+i]);
    start = 1680; for (i=0; i<240; i++) VecAXPY(ctx->yd[8],  12.0/(double)ctx->nt, y[start+i]);
    start = 1920; for (i=0; i<240; i++) VecAXPY(ctx->yd[9],  12.0/(double)ctx->nt, y[start+i]);
    start = 2160; for (i=0; i<240; i++) VecAXPY(ctx->yd[10], 12.0/(double)ctx->nt, y[start+i]);
    start = 2400; for (i=0; i<240; i++) VecAXPY(ctx->yd[11], 12.0/(double)ctx->nt, y[start+i]);
    start = 2640; for (i=0; i<240; i++) VecAXPY(ctx->yd[12], 12.0/(double)ctx->nt, y[start+i]);
    // seasonal
    start = 0;    for (i=0; i<720; i++) VecAXPY(ctx->yd[13], 4.0/(double)ctx->nt, y[start+i]);
    start = 720;  for (i=0; i<720; i++) VecAXPY(ctx->yd[14], 4.0/(double)ctx->nt, y[start+i]);
    start = 1440; for (i=0; i<720; i++) VecAXPY(ctx->yd[15], 4.0/(double)ctx->nt, y[start+i]);
    start = 2160; for (i=0; i<720; i++) VecAXPY(ctx->yd[16], 4.0/(double)ctx->nt, y[start+i]);

    return ctx->yd;
}


