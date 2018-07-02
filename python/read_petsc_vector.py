import numpy as np

def read_petsc_vector(file):
    f = open(file, "rb")
    np.fromfile(f, dtype=">i4", count=1)
    nv = np.fromfile(f, dtype=">i4", count=1)[0]
    v = np.fromfile(f, dtype=">f8", count=nv)
    f.close()
    return v


