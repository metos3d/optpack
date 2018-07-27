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

model_list = ["N", "N-DOP", "NP-DOP", "NPZ-DOP", "NPZD-DOP"]
language_list = ["matlab", "python", "c"]
language_extensions = {"matlab": {"code": "m", "data": "mat"}, "python": {"code": "py", "data": "h5"}, "c": {"code": "c", "data": "h5"}}

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
def format_text(text_template, config_dict, experiment_number, number_of_iterations):
    text = ""
    for line in text_template.splitlines(keepends=True):
        text = text + line.format(**config_dict, nexp=experiment_number, niter=number_of_iterations)
    return text

# ---------------------------------------------------------------------------------------------------------------------
# write text file
# ---------------------------------------------------------------------------------------------------------------------
def write_text_file(file_path, text):
    file = open(file_path, "w")
    file.write(text)
    file.close()

# ---------------------------------------------------------------------------------------------------------------------
# read configuration
# ---------------------------------------------------------------------------------------------------------------------
def read_configuration(file):
    print("Reading configuration .................. " + file)
    if not os.path.exists(file):
        optpack_path = os.path.abspath(os.path.dirname(__file__))
        file = os.path.join(optpack_path, file)
        print("Reading configuration .................. " + file)
        if not os.path.exists(file):
            print("File not found ...")
            print("Exiting ...")
            sys.exit(1)
    return parse_yaml_file(file)

# ---------------------------------------------------------------------------------------------------------------------
# create job script
# ---------------------------------------------------------------------------------------------------------------------
def create_job_script(experiment_conf, model_conf, job_conf, opt_conf, experiment_name, experiment_number, number_of_iterations):

    job_text_file = os.path.join(experiment_name, experiment_name + ".job." + experiment_number + ".sh")
    print(job_text_file)

    template_file = job_conf["job"]["template"]
    language = opt_conf["opt"]["language"]
    job_template_text = read_template("{0}/{1}/template/{2}".format(experiment_name, language, template_file))
    conf_dict = dict(experiment_conf, **model_conf, **job_conf, **opt_conf)
    job_text = format_text(job_template_text, conf_dict, experiment_number, number_of_iterations)

    write_text_file(job_text_file, job_text)

# ---------------------------------------------------------------------------------------------------------------------
# create start script
# ---------------------------------------------------------------------------------------------------------------------
def create_start_script(experiment_conf, model_conf, job_conf, opt_conf, experiment_name, experiment_number, number_of_iterations):

    language = opt_conf["opt"]["language"]
    extension_code = language_extensions[language]["code"]

    start_text_file = os.path.join(experiment_name, experiment_name + ".start." + experiment_number + "." + extension_code)
    print(start_text_file)
    
    # format parameter list
    # u0, ud, lb, ub
    # nu
    nu = model_conf["parameter"]["nu"]
    u0 = model_conf["parameter"]["u0"]
    ud = model_conf["parameter"]["ud"]
    lb = model_conf["parameter"]["lb"]
    ub = model_conf["parameter"]["ub"]
    model_conf["parameter"]["u0"] = ','.join(['{:.16e}']*nu).format(*u0)
    model_conf["parameter"]["ud"] = ','.join(['{:.16e}']*nu).format(*ud)
    model_conf["parameter"]["lb"] = ','.join(['{:.16e}']*nu).format(*lb)
    model_conf["parameter"]["ub"] = ','.join(['{:.16e}']*nu).format(*ub)
    
    start_template_text = read_template("{0}/{1}/template/template.start.{2}".format(experiment_name, language, extension_code))
    conf_dict = dict(experiment_conf, **model_conf, **job_conf, **opt_conf)
    start_text = format_text(start_template_text, conf_dict, experiment_number, number_of_iterations)
    
    write_text_file(start_text_file, start_text)

# ---------------------------------------------------------------------------------------------------------------------
# compile if c
# ---------------------------------------------------------------------------------------------------------------------
def compile_if_c(opt_conf, experiment_name, experiment_number):
    if opt_conf["opt"]["language"]=="c":
        executable_base = experiment_name + ".start." + experiment_number
        print("Compiling executable ................... {0}.exe".format(executable_base))
        os.system('''
cd {0}/;
source petsc.env.sh &> /dev/null;
#source model/petsc.env.sh;
PROGRAM={1}.exe OBJECT_FILE={1}.o make -f c/Makefile clean &> /dev/null
#PROGRAM={1}.exe OBJECT_FILE={1}.o make -f c/Makefile clean
'''.format(experiment_name, executable_base))
        os.system('''
cd {0}/;
source petsc.env.sh &> /dev/null;
#source model/petsc.env.sh;
PROGRAM={1}.exe OBJECT_FILE={1}.o make -f c/Makefile &> /dev/null
#PROGRAM={1}.exe OBJECT_FILE={1}.o make -f c/Makefile
'''.format(experiment_name, executable_base))

# ---------------------------------------------------------------------------------------------------------------------
# prepare new experiment
# ---------------------------------------------------------------------------------------------------------------------
def prepare_new_experiment(experiment_conf, experiment_name, number_of_iterations):
    print("Preparing new experiment ...")
    
    print("Creating directory ..................... {0}/".format(experiment_name))
    os.system("mkdir {0}".format(experiment_name))
    print("Creating directory ..................... {0}/model/".format(experiment_name))
    os.system("mkdir {0}/model/".format(experiment_name))
    print("Creating directory ..................... {0}/model/option".format(experiment_name))
    os.system("mkdir {0}/model/option".format(experiment_name))
    print("Creating directory ..................... {0}/model/log".format(experiment_name))
    os.system("mkdir {0}/model/log".format(experiment_name))
    
    optpack_conf = read_configuration("optpack.conf.yaml")
    optpack_path = os.path.abspath(os.path.dirname(__file__))
    copy_from = os.path.normpath(os.path.join(optpack_path, optpack_conf["model"]["petsc"]))
    copy_to = "{0}/model/petsc.env.sh".format(experiment_name)
    print("Copying environment file ......... from: {0}".format(copy_from))
    print("                                     to: {0}".format(copy_to))
    os.system("cp {0} {1}".format(copy_from, copy_to))

    model_conf = read_configuration(experiment_conf["experiment"]["model"])
    model_name = model_conf["model"]["name"]
    model_metos3d_path = os.path.normpath(os.path.join(optpack_path, optpack_conf["model"]["metos3d"]))
    print("Compiling executable ................... {0}/model/metos3d-simpack-{1}.exe".format(experiment_name, model_name))
    os.system('''
cd {0}/model/;
source petsc.env.sh &> /dev/null
#source petsc.env.sh

# links
ln -s {1}/data/data
ln -s {1}/model/model
ln -s {1}/simpack
ln -s {1}/metos3d/Makefile

# compile
make BGC=model/{2} clean &> /dev/null
make BGC=model/{2} &> /dev/null
#make BGC=model/{2} clean
#make BGC=model/{2}
'''.format(experiment_name, model_metos3d_path, model_name))

    opt_conf = read_configuration(experiment_conf["experiment"]["opt"])
    language = opt_conf["opt"]["language"]
    print("Preparing language ..................... " + language)
    copy_from = optpack_path + "/language/{0}".format(language)
    copy_to = "{0}/{1}".format(experiment_name, language)
    print("Copying codes .................... from: {0}".format(copy_from))
    print("                                     to: {0}".format(copy_to))
    os.system("cp -r {0} {1}".format(copy_from, copy_to))

    job_conf = read_configuration(experiment_conf["experiment"]["job"])
    experiment_number = "1"
    print("Creating initial job script ............ ", end="")
    create_job_script(experiment_conf, model_conf, job_conf, opt_conf, experiment_name, experiment_number, number_of_iterations)
    print("Creating initial start script .......... ", end="")
    create_start_script(experiment_conf, model_conf, job_conf, opt_conf, experiment_name, experiment_number, number_of_iterations)

    compile_if_c(opt_conf, experiment_name, experiment_number)

# ---------------------------------------------------------------------------------------------------------------------
# continue experiment
# ---------------------------------------------------------------------------------------------------------------------
def continue_experiment(experiment_conf, experiment_name, number_of_iterations):
    print("Continuing experiment ...")

    optpack_conf = read_configuration("optpack.conf.yaml")

    opt_conf = read_configuration(experiment_conf["experiment"]["opt"])
    language = opt_conf["opt"]["language"]

    extension_data = language_extensions[language]["data"]
    log_file_pattern = os.path.join(experiment_name, experiment_name + ".*." + extension_data)
    print("Checking pattern ....................... {0}".format(log_file_pattern))
    log_file_list = glob.glob(log_file_pattern)

    experiment_number = len(log_file_list)
    if experiment_number==0:
        print("No previous runs found ... ")
        print("Exiting ...")
        sys.exit(1)

    for log_file in log_file_list:
        print("Found log file ......................... {0}".format(log_file))

    print("Reading last result ...")
    uopt = h5py.File(log_file_list[-1])["uopt"][...].flatten()
    model_conf = read_configuration(experiment_conf["experiment"]["model"])
    model_conf["parameter"]["u0"] = uopt

    experiment_number = str(experiment_number + 1)
    print("Continuing with optimization run ....... {0}".format(experiment_number))

    job_conf = read_configuration(experiment_conf["experiment"]["job"])
    print("Creating initial job script ............ ", end="")
    create_job_script(experiment_conf, model_conf, job_conf, opt_conf, experiment_name, experiment_number, number_of_iterations)
    print("Creating initial start script .......... ", end="")
    create_start_script(experiment_conf, model_conf, job_conf, opt_conf, experiment_name, experiment_number, number_of_iterations)

    compile_if_c(opt_conf, experiment_name, experiment_number)

# ---------------------------------------------------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------------------------------------------------
if __name__ == "__main__":

    if len(sys.argv[:]) < 3:
        print("usage: python {0} [experiment-conf-file] [number-of-iterations]".format(sys.argv[0]))
        print("example:")
        print("$> python {0} TW.MAT-IP.N.conf.yaml 10".format(sys.argv[0]))
        sys.exit(0)

    experiment_conf_file = sys.argv[1]
    number_of_iterations = sys.argv[2]

    experiment_conf = read_configuration(experiment_conf_file)
    experiment_name = experiment_conf["experiment"]["name"]

    print("Processing experiment .................. " + experiment_name)
    if os.path.exists(experiment_name):
        continue_experiment(experiment_conf, experiment_name, number_of_iterations)
    else:
        prepare_new_experiment(experiment_conf, experiment_name, number_of_iterations)


