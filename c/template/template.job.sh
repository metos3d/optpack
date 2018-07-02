#!/bin/bash
#
# Metos3D: A Marine Ecosystem Toolkit for Optimization and Simulation in 3-D
# Copyright (C) 2018  Jaroslaw Piwonski, CAU, jpi@informatik.uni-kiel.de
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#PBS -T intmpi
#PBS -b 5
#PBS -l cpunum_job=32
#PBS -l elapstim_req=100:00:00
#PBS -l memsz_job=10gb
#PBS -N twin.c-petsc-tao.blmvm.NPZD-DOP
#PBS -o twin.c-petsc-tao.blmvm.NPZD-DOP.job.out.txt
#PBS -j o
#PBS -q cllong

# Note: We create a directory in '/scratch', because it is a local, fast storage located on the master node.
# All other directories are network mounted paths and, as such, significantly slower.
# On the NEC Linux cluster the ratio is approximately 1:6.
mkdir /scratch/jpi
cd $PBS_O_WORKDIR

. ../../petsc/de.uni-kiel.rz.nesh-fe.petsc-3.9.0.opt.sh
./c-petsc-tao/run.exe -tao_monitor &> twin.c-petsc-tao.blmvm.NPZD-DOP.out.txt

rm -fr /scratch/jpi
qstat -f ${PBS_JOBID/0:}

