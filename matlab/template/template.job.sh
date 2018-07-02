#!/bin/bash
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
module load matlab2017a
matlab -nodisplay -logfile {experiment[name]}/{experiment[name]}.{model[name]}.{nexp}.out.txt < {experiment[name]}/{experiment[name]}.{model[name]}.{nexp}.start.m > /dev/null

qstat -f ${PBS_JOBID/0:}


