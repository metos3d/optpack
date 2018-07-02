#include "objective.h"

void store(void);

#undef  __FUNCT__
#define __FUNCT__ "store"
void store(void) {

    // store u, J, use PETSc viewer for convience
    PetscViewer viewer;
    PetscViewerHDF5Open(ctx.comm, ctx.logfile, FILE_MODE_APPEND, &viewer);
    PetscViewerHDF5SetTimestep(viewer, ctx.i-1);
    VecView(ctx.u, viewer);
    VecView(ctx.J, viewer);
    PetscViewerDestroy(&viewer);

    // store y, use direct hdf5 access
    hid_t file;
    file = H5Fopen(ctx.logfile, H5F_ACC_RDWR, H5P_DEFAULT);
    
    if (ctx.i==1) {
        hsize_t dim = 3;
        hsize_t dims[3];
        hsize_t maxDims[3];
        hid_t   property;
        hid_t   dataspace;
        hid_t   filespace;
        hid_t   dataset;
        // create initial dataset
        dims[0] = 1;
        dims[1] = 1;
        dims[2] = ctx.nx;
        maxDims[0] = H5S_UNLIMITED;
        maxDims[1] = H5S_UNLIMITED;
        maxDims[2] = dims[2];

        property = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_deflate(property, 9);                        // compression on
        H5Pset_chunk(property, dim, dims);
        dataspace = H5Screate_simple(dim, dims, maxDims);
        dataset = H5Dcreate2(file, "/y", H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT, property, H5P_DEFAULT);

        H5Pclose(property);
        H5Sclose(dataspace);
        H5Dclose(dataset);
    }

    // set extent and write
    int     i;
    hsize_t dim = 3;
    hsize_t dims[3];
    hsize_t offset[3];
    hsize_t dimsext[3];
    hid_t   dataset;
    hid_t   filespace;
    hid_t   memspace;
    const PetscScalar *yarr;

    dims[0] = ctx.i;
    dims[1] = ctx.ndata;
    dims[2] = ctx.nx;
    dimsext[0] = 1;
    dimsext[1] = 1;
    dimsext[2] = ctx.nx;

    dataset = H5Dopen2(file, "/y", H5P_DEFAULT);
    H5Dset_extent(dataset, dims);
    filespace = H5Dget_space(dataset);
    memspace = H5Screate_simple(dim, dimsext, NULL);

    for (i=0; i<ctx.ndata; i++) {
        offset[0] = ctx.i-1;
        offset[1] = i;
        offset[2] = 0;
        H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset, NULL, dimsext, NULL);
        VecGetArrayRead(ctx.y[i], &yarr);
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, yarr);
        VecRestoreArrayRead(ctx.y[i], &yarr);
    }

    H5Dclose(dataset);
    H5Sclose(filespace);
    H5Sclose(memspace);
    
    H5Fclose(file);
}

#undef  __FUNCT__
#define __FUNCT__ "objective"
PetscErrorCode objective(Tao tao, Vec u, PetscReal* J, void* notused) {
    // info
    printf("# i:        %03d\n", ctx.i);
    
    // data type
    VecCopy(u, ctx.u);

    // evaluate model, compute misfit
    model(u);
    *J = misfit();
//    *J = 0.0;
    
    // store
    store();

    ctx.i = ctx.i+1;
    return(0);
}


