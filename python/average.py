import numpy as np

def average(y, ctx):
    # annual
    nt = ctx.nt
    yan = 1./nt * np.sum(y[0:nt,:], axis=0)
    
    # monthly
    start = 0;    yjan = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 240;  yfeb = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 480;  ymar = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 720;  yapr = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 960;  ymay = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1200; yjun = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1440; yjul = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1680; yaug = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 1920; ysep = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 2160; yoct = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 2400; ynov = 12./nt * np.sum(y[start:start+240,:], axis=0)
    start = 2640; ydec = 12./nt * np.sum(y[start:start+240,:], axis=0)
    
    # seasonal
    start = 0;    ywin = 4./nt * np.sum(y[start:start+720,:], axis=0)
    start = 720;  yspr = 4./nt * np.sum(y[start:start+720,:], axis=0)
    start = 1440; ysum = 4./nt * np.sum(y[start:start+720,:], axis=0)
    start = 2160; yaut = 4./nt * np.sum(y[start:start+720,:], axis=0)

    # store, same order as woa data, caution: matlab uses 1 indexed arrays
    # 00, annual
    # 01 - 12, monthly
    # 13 - 16, seasonal
    yavg = np.array([yan,yjan,yfeb,ymar,yapr,ymay,yjun,yjul,yaug,ysep,yoct,ynov,ydec,ywin,yspr,ysum,yaut])

    return yavg


