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
#PBS -b {job[nodes]}
#PBS -l cpunum_job={job[cores]}
#PBS -l elapstim_req={job[walltime]}
#PBS -l memsz_job=10gb
#PBS -N {experiment[name]}.{model[name]}.{nexp}
#PBS -o {experiment[name]}/{experiment[name]}.{model[name]}.{nexp}.job.out.txt
#PBS -j o
#PBS -q {job[queue]}

# Note: We use a directory in '/scratch', because it is a local, fast storage located on the master node.
# All other directories are network mounted paths and, as such, significantly slower.
# On the NEC Linux cluster the ratio is usually better than 1:10.
cd $PBS_O_WORKDIR

export SCRATCH="/scratch/${PBS_JOBID/0:}/"
python {experiment[name]}/{experiment[name]}.{model[name]}.{nexp}.start.py &> {experiment[name]}/{experiment[name]}.{model[name]}.{nexp}.out.txt

qstat -f ${PBS_JOBID/0:}


