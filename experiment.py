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

import os
import sys
import yaml
import glob
import h5py

# ---------------------------------------------------------------------------------------------------------------------
# parse yaml file
# ---------------------------------------------------------------------------------------------------------------------
def parse_yaml_file(yaml_file_path):
    yaml_file = open(yaml_file_path, "r")
    conf_dict = yaml.load(yaml_file)
    yaml_file.close()
    return conf_dict

# ---------------------------------------------------------------------------------------------------------------------
# read template
# ---------------------------------------------------------------------------------------------------------------------
def read_template(template_file_path):
    template_file = open(template_file_path, "r")
    template_text = template_file.read()
    template_file.close()
    return template_text

# ---------------------------------------------------------------------------------------------------------------------
# format text
# ---------------------------------------------------------------------------------------------------------------------
def format_text(text_template, config_dict, nexp, niter):
    text = ""
    for line in text_template.splitlines(keepends=True):
        try:
            text = text + line.format(**config_dict, nexp=nexp, niter=niter)
        except KeyError:
            text = text + line
    return text

# ---------------------------------------------------------------------------------------------------------------------
# write text file
# ---------------------------------------------------------------------------------------------------------------------
def write_text_file(file_path, text):
    file = open(file_path, "w")
    file.write(text)
    file.close()

# ---------------------------------------------------------------------------------------------------------------------
# compile if c
# ---------------------------------------------------------------------------------------------------------------------
def compile_if_c(exp_config, expname, nexp):
    if exp_config["language"]["name"]=="c":
        modname = exp_config["model"]["name"]
        experiment_pattern = os.path.join(expname, expname + "." + modname + "." + nexp + ".start")
        print("Compiling executable ...             {0}.exe".format(experiment_pattern))
        os.system(". ./c/petsc.env.sh; PROGRAM={0}.exe EXPERIMENT={0}.o make -f c/Makefile clean &> /dev/null".format(experiment_pattern))
        os.system(". ./c/petsc.env.sh; PROGRAM={0}.exe EXPERIMENT={0}.o make -f c/Makefile &> /dev/null".format(experiment_pattern))

# ---------------------------------------------------------------------------------------------------------------------
# create job script
# ---------------------------------------------------------------------------------------------------------------------
def create_job_script(exp_config, expname, nexp, niter):
    
    job_template_text = read_template("template.job.sh")
    job_text = format_text(job_template_text, exp_config, nexp, niter)

    modname = exp_config["model"]["name"]
    job_text_file = os.path.join(expname, expname + "." + modname + "." + nexp + ".job.sh")
    print(job_text_file)
    
    write_text_file(job_text_file, job_text)

# ---------------------------------------------------------------------------------------------------------------------
# create start script
# ---------------------------------------------------------------------------------------------------------------------
def create_start_script(exp_config, expname, nexp, niter):
    
    # format parameter list
    # u0, ud, lb, ub
    # nu
    nu = exp_config["parameter"]["nu"]
    u0 = exp_config["parameter"]["u0"]
    ud = exp_config["parameter"]["ud"]
    lb = exp_config["parameter"]["lb"]
    ub = exp_config["parameter"]["ub"]
    exp_config["parameter"]["u0"] = ','.join(['{:.16e}']*nu).format(*u0)
    exp_config["parameter"]["ud"] = ','.join(['{:.16e}']*nu).format(*ud)
    exp_config["parameter"]["lb"] = ','.join(['{:.16e}']*nu).format(*lb)
    exp_config["parameter"]["ub"] = ','.join(['{:.16e}']*nu).format(*ub)

    extension_code = exp_config["language"]["code"]
    start_template_text = read_template("template.start." + extension_code)
    start_text = format_text(start_template_text, exp_config, nexp, niter)
    
    modname = exp_config["model"]["name"]
    start_text_file = os.path.join(expname, expname + "." + modname + "." + nexp + ".start." + extension_code)
    print(start_text_file)
    
    write_text_file(start_text_file, start_text)

## ---------------------------------------------------------------------------------------------------------------------
## prepare new experiment
## ---------------------------------------------------------------------------------------------------------------------
#def prepare_new_experiment(exp_config, expname, niter):
#    print("Preparing new experiment ...         " + expname)
#    
#    print("Creating experiment directory ...    {0}/".format(expname))
#    os.system("mkdir {0}".format(expname))
#    print("Creating option directory ...        {0}/option".format(expname))
#    os.system("mkdir {0}/option".format(expname))
#    print("Creating log directory ...           {0}/log".format(expname))
#    os.system("mkdir {0}/log".format(expname))
#    
#    nexp = "1"
#    print("Creating initial job script ...      ", end="")
#    create_job_script(exp_config, expname, nexp, niter)
#    print("Creating initial start script ...    ", end="")
#    create_start_script(exp_config, expname, nexp, niter)
#
#    compile_if_c(exp_config, expname, nexp)

## ---------------------------------------------------------------------------------------------------------------------
## continue experiment
## ---------------------------------------------------------------------------------------------------------------------
#def continue_experiment(exp_config, expname, niter):
#    print("Continuing experiment ...            " + expname)
#
#    extension_data = exp_config["language"]["data"]
#    modname = exp_config["model"]["name"]
#    log_file_pattern = os.path.join(expname, expname + "." + modname + ".*." + extension_data)
#    print("Checking pattern ...                 {0}".format(log_file_pattern))
#    log_file_list = glob.glob(log_file_pattern)
#    
#    nexp = len(log_file_list)
#    if nexp==0:
#        print("No previous runs found ... ")
#        print("Exiting ...")
#        sys.exit(1)
#    
#    for log_file in log_file_list:
#        print("Found log file ...                   {0}".format(log_file))
#
#    print("Reading last result ...")
#    uopt = h5py.File(log_file_list[-1])["uopt"][...].flatten()
#    exp_config["parameter"]["u0"] = uopt
#    
#    nexp = str(nexp + 1)
#    print("Continuing with optimization run ... {0}".format(nexp))
#
#    print("Creating job script ...              ", end="")
#    create_job_script(exp_config, expname, nexp, niter)
#    print("Creating start script ...            ", end="")
#    create_start_script(exp_config, expname, nexp, niter)
#
#    compile_if_c(exp_config, expname, nexp)

## ---------------------------------------------------------------------------------------------------------------------
## main
## ---------------------------------------------------------------------------------------------------------------------
#if __name__ == "__main__":
#    if len(sys.argv[:]) < 4:
#        print('''usage: python {0} [new|continue] [experiment-name] [iteration-count]
#example:
#$> python {0} new exp-01 10'''.format(sys.argv[0]))
#        sys.exit(0)
#
#    command = sys.argv[1]
#    expname = sys.argv[2]
#    niter = sys.argv[3]
#
#    exp_config = parse_yaml_file("experiment.conf.yaml")
#    # strip any trailing path separators
#    expname = os.path.normpath(expname)
#    exp_config["experiment"] = {"name": expname}
#
#    if command=="new":
#        if os.path.exists(expname):
#            print("Directory already exists ... " + expname)
#            print("Exiting ...")
#            sys.exit(1)
#
#        prepare_new_experiment(exp_config, expname, niter)
#
#    elif command=="continue":
#        if not os.path.exists(expname):
#            print("Experiment directory does not exists ... " + expname)
#            print("Exiting ...")
#            sys.exit(1)
#
#        continue_experiment(exp_config, expname, niter)
#
#    else:
#        print("Unknown command ... " + command)
#        print("Exiting ...")
#        sys.exit(1)



