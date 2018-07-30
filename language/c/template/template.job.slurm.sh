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

#SBATCH --job-name={experiment[name]}.{nexp}
#SBATCH --output={experiment[name]}/{experiment[name]}}.job.{nexp}.out.txt
#SBATCH --error={experiment[name]}/{experiment[name]}.job.{nexp}.out.txt
#SBATCH --nodes={job[nodes]}
#SBATCH --tasks-per-node={job[cores]}
#SBATCH --cpus-per-task=1
#SBATCH --mem=10G
#SBATCH --time={job[walltime]}
#SBATCH --partition={job[queue]}
#SBATCH --qos={job[queue]}

export SCRATCH="${{TMPDIR}}/"
export MPIRUN="{job[mpirun]}"

cd {experiment[name]}/
./{experiment[name]}.start.{nexp}.exe {opt[display]} &> {experiment[name]}.out.{nexp}.txt

scontrol show job $SLURM_JOBID


