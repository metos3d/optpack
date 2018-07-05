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

#include "store.h"

#undef  __FUNCT__
#define __FUNCT__ "store"
PetscErrorCode store(context* ctx) {
    PetscViewer viewer;
    hid_t       file;
    int         i;
    hsize_t     dim = 3;
    hsize_t     dims[3];
    hsize_t     offset[3];
    hsize_t     dimsext[3];
    hid_t       dataset;
    hid_t       filespace;
    hid_t       memspace;
    const       PetscScalar *yarr;

    // initial log file creation
    if (ctx->i==1) {
        PetscViewerHDF5Open(ctx->comm, ctx->logfile, FILE_MODE_WRITE, &viewer);
        // store ud, lb, ub
        VecView(ctx->lb, viewer);
        VecView(ctx->ub, viewer);
        VecView(ctx->ud, viewer);
        // store yd
        for (i=0; i<ctx->ndata; i++) {
            PetscViewerHDF5SetTimestep(viewer, i);
            PetscObjectSetName((PetscObject)ctx->yd[i], "yd");
            VecView(ctx->yd[i], viewer);
        }
        PetscViewerDestroy(&viewer);
    }

    // store u, J, use PETSc viewer for convience
    PetscViewerHDF5Open(ctx->comm, ctx->logfile, FILE_MODE_APPEND, &viewer);
    PetscViewerHDF5SetTimestep(viewer, ctx->i-1);
    VecView(ctx->u, viewer);
    VecView(ctx->J, viewer);
    PetscViewerDestroy(&viewer);

    // store y, use direct hdf5 access
    file = H5Fopen(ctx->logfile, H5F_ACC_RDWR, H5P_DEFAULT);

    if (ctx->i==1) {
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
        dims[2] = ctx->nx;
        maxDims[0] = H5S_UNLIMITED;
        maxDims[1] = H5S_UNLIMITED;
        maxDims[2] = dims[2];

        property = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_deflate(property, 4);                        // compression on
        H5Pset_chunk(property, dim, dims);
        dataspace = H5Screate_simple(dim, dims, maxDims);
        dataset = H5Dcreate2(file, "/y", H5T_NATIVE_DOUBLE, dataspace, H5P_DEFAULT, property, H5P_DEFAULT);

        H5Pclose(property);
        H5Sclose(dataspace);
        H5Dclose(dataset);
    }

    // set extent and write
    dims[0] = ctx->i;
    dims[1] = ctx->ndata;
    dims[2] = ctx->nx;
    dimsext[0] = 1;
    dimsext[1] = 1;
    dimsext[2] = ctx->nx;

    dataset = H5Dopen2(file, "/y", H5P_DEFAULT);
    H5Dset_extent(dataset, dims);
    filespace = H5Dget_space(dataset);
    memspace = H5Screate_simple(dim, dimsext, NULL);

    for (i=0; i<ctx->ndata; i++) {
        offset[0] = ctx->i-1;
        offset[1] = i;
        offset[2] = 0;
        H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset, NULL, dimsext, NULL);
        VecGetArrayRead(ctx->y[i], &yarr);
        H5Dwrite(dataset, H5T_NATIVE_DOUBLE, memspace, filespace, H5P_DEFAULT, yarr);
        VecRestoreArrayRead(ctx->y[i], &yarr);
    }

    H5Dclose(dataset);
    H5Sclose(filespace);
    H5Sclose(memspace);

    H5Fclose(file);
    
    return(0);
}



