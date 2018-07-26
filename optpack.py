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

import sys
import os
import yaml

#model_list = ["N", "N-DOP", "NP-DOP", "NPZ-DOP", "NPZD-DOP"]
#language_list = ["matlab", "python", "c"]
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
def format_text(text_template, config_dict, number_of_iterations):
    text = ""
    for line in text_template.splitlines(keepends=True):
        text = text + line.format(**config_dict, nexp=number_of_iterations)
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
#        print("File not found ...")
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
def create_job_script(experiment_conf, model_conf, opt_conf, experiment_name, experiment_number, number_of_iterations):

    job_text_file = os.path.join(experiment_name, experiment_name + ".job." + experiment_number + ".sh")
    print(job_text_file)

    job_conf = read_configuration(experiment_conf["experiment"]["job"])
    template_file = job_conf["job"]["template"]

    language = opt_conf["opt"]["language"]
    job_template_text = read_template("{0}/{1}/template/{2}".format(experiment_name, language, template_file))
    conf_dict = dict(experiment_conf, **model_conf, **job_conf)
    job_text = format_text(job_template_text, conf_dict, number_of_iterations)

    write_text_file(job_text_file, job_text)

# ---------------------------------------------------------------------------------------------------------------------
# create start script
# ---------------------------------------------------------------------------------------------------------------------
def create_start_script(experiment_conf, model_conf, opt_conf, experiment_name, experiment_number, number_of_iterations):

    language = opt_conf["opt"]["language"]
    extension_code = language_extensions[language]["code"]

    start_text_file = os.path.join(experiment_name, experiment_name + "." + experiment_number + ".start." + extension_code)
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
    job_conf = read_configuration(experiment_conf["experiment"]["job"])
    conf_dict = dict(experiment_conf, **model_conf, **job_conf)
    start_text = format_text(start_template_text, conf_dict, number_of_iterations)
    
    write_text_file(start_text_file, start_text)

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
#source ./petsc.env.sh &> /dev/null
source ./petsc.env.sh

# links
ln -s {1}/data/data
ln -s {1}/model/model
ln -s {1}/simpack
ln -s {1}/metos3d/Makefile

# compile
#make BGC=model/{2} clean &> /dev/null
#make BGC=model/{2} &> /dev/null
make BGC=model/{2} clean
make BGC=model/{2}
'''.format(experiment_name, model_metos3d_path, model_name))

    opt_conf = read_configuration(experiment_conf["experiment"]["opt"])
    language = opt_conf["opt"]["language"]
    print("Preparing language ..................... " + language)
    copy_from = optpack_path + "/language/{0}".format(language)
    copy_to = "{0}/{1}".format(experiment_name, language)
    print("Copying codes .................... from: {0}".format(copy_from))
    print("                                     to: {0}".format(copy_to))
    os.system("cp -r {0} {1}".format(copy_from, copy_to))

    experiment_number = "1"
    print("Creating initial job script ............ ", end="")
    create_job_script(experiment_conf, model_conf, opt_conf, experiment_name, experiment_number, number_of_iterations)

    print("Creating initial start script .......... ", end="")
    create_start_script(experiment_conf, model_conf, opt_conf, experiment_name, experiment_number, number_of_iterations)

#    compile_if_c(exp_config, expname, nexp)

# ---------------------------------------------------------------------------------------------------------------------
# continue experiment
# ---------------------------------------------------------------------------------------------------------------------
def continue_experiment(experiment_conf, experiment_name, number_of_iterations):
    print("Continuing experiment ...")

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
#print("Creating job script ...              ", end="")
#create_job_script(exp_config, expname, nexp, niter)
#print("Creating start script ...            ", end="")
#create_start_script(exp_config, expname, nexp, niter)
#
#compile_if_c(exp_config, expname, nexp)


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






#    copy_from = optpack_path + "/language/{0}/template/template.job.sh".format(language_name)
#    copy_to = "{0}/template.job.sh".format(model_name)
#    print("Copying job template ............. from: {0}".format(copy_from))
#    print("                                     to: {0}".format(copy_to))
#    os.system("cp {0} {1}".format(copy_from, copy_to))
#
#    extension_code = language_extensions[language_name]["code"]
#    copy_from = optpack_path + "/language/{0}/template/template.start.{1}".format(language_name, extension_code)
#    copy_to = "{0}/template.start.{1}".format(model_name, extension_code)
#    print("Copying start template ........... from: {0}".format(copy_from))
#    print("                                     to: {0}".format(copy_to))
#    os.system("cp {0} {1}".format(copy_from, copy_to))
#
#    extension_data = language_extensions[language_name]["data"]
#    copy_from = optpack_path + "/experiment/{0}.conf.yaml".format(model_name)
#    copy_to = "{0}/experiment.conf.yaml".format(model_name)
#    print("Preparing experiment configuration ..... " + copy_to)
#    print("Formatting template .................... " + copy_from)
#    experiment_conf_template = read_template(copy_from)
#    format_conf = {"language": {"name": language_name, "code": extension_code, "data": extension_data}}
#    experiment_conf = format_text(experiment_conf_template, format_conf)
#    write_text_file(copy_to, experiment_conf)
#
#    print("Preparing {0} codes ...".format(language_name))
#    dir_name = "{0}/{1}/".format(model_name, language_name)
#    print("Creating directory ..................... {0}".format(dir_name))
#    os.system("mkdir {0}".format(dir_name))
#
#    if language_name=="c":
#        copy_to = "{0}/{1}/.".format(model_name, language_name)
#        copy_from = optpack_path + "/language/{0}/*.c".format(language_name)
#        print("Copying codes .................... from: {0}".format(copy_from))
#        os.system("cp {0} {1}".format(copy_from, copy_to))
#        copy_from = optpack_path + "/language/{0}/*.h".format(language_name)
#        print("                                   from: {0}".format(copy_from))
#        os.system("cp {0} {1}".format(copy_from, copy_to))
#        copy_from = optpack_path + "/language/{0}/Makefile".format(language_name)
#        print("                                   from: {0}".format(copy_from))
#        print("                                     to: {0}".format(copy_to))
#        os.system("cp {0} {1}".format(copy_from, copy_to))
#        # petsc for tao
#        if conf_optpack.get("language"):
#            copy_from = conf_optpack["language"]["c"]["petsc"]
#            copy_to = "{0}/{1}/petsc.env.sh".format(model_name, language_name)
#            print("                                   from: {0}".format(copy_from))
#            print("                                     to: {0}".format(copy_to))
#            os.system("cp {0} {1}".format(copy_from, copy_to))
#    else:
#        copy_from = optpack_path + "/language/{0}/*.{1}".format(language_name, extension_code)
#        copy_to = "{0}/{1}/.".format(model_name, language_name)
#        print("Copying codes .................... from: {0}".format(copy_from))
#        print("                                     to: {0}".format(copy_to))
#        os.system("cp {0} {1}".format(copy_from, copy_to))




#
#   DUMP
#

#        print("")
#        print("options:")
#        print("[model-name]     ", ", ".join(model_list))
#        print("[language]       ", ", ".join(language_list))
#        print("")

#    model_name = sys.argv[1]
#    if not model_name in model_list:
#        print("Model not known ... " + model_name)
#        print("Choose from ... ", ", ".join(model_list))
#        print("Exiting ...")
#        sys.exit(1)
#
#    language_name = sys.argv[2]
#    if not language_name in language_list:
#        print("Language not known ... " + language_name)
#        print("Choose from ... ", ", ".join(language_list))
#        print("Exiting ...")
#        sys.exit(1)

#    optpack_path = os.path.abspath(os.path.dirname(__file__))
#    conf_file_path = os.path.join(optpack_path, "optpack.conf.yaml")
#    print("Reading optpack configuration .......... " + conf_file_path)
#    conf_optpack = parse_yaml_file(conf_file_path)

##    '{:_<10}'.format('test')
#
#optpack_path = os.path.abspath(os.path.dirname(__file__))
#    optpack_conf_path = os.path.join(optpack_path, "optpack.conf.yaml")
#    print("Reading optpack configuration .......... " + optpack_conf_path)


#    optpack_path = os.path.abspath(os.path.dirname(__file__))
#    optpack_conf_path = os.path.join(optpack_path, "optpack.conf.yaml")
#    print("Reading optpack configuration .......... " + optpack_conf_path)
#    optpack_conf = parse_yaml_file(optpack_conf_path)
#

#    print("Reading experiment configuration ....... " + experiment_conf_file)
#    experiment_conf = parse_yaml_file(experiment_conf_file)
#

#    model_conf_file = experiment_conf["experiment"]["model"]
#    model_conf_path = os.path.join(optpack_path, "conf/model/" + model_conf_file)
#    print("Reading model configuration ............ " + model_conf_path)
#    model_conf = parse_yaml_file(model_conf_path)
#
#    job_conf_file = experiment_conf["experiment"]["job"]
#    job_conf_path = os.path.join(optpack_path, "conf/job/" + job_conf_file)
#    print("Reading job configuration .............. " + job_conf_path)
#    job_conf = parse_yaml_file(job_conf_path)
#
#    opt_conf_file = experiment_conf["experiment"]["opt"]
#    opt_conf_path = os.path.join(optpack_path, "conf/opt/" + opt_conf_file)
#    print("Reading optimization configuration ..... " + opt_conf_path)
#    opt_conf = parse_yaml_file(opt_conf_path)

#    print("Creating experiment directory ...    {0}/".format(expname))
#    os.system("mkdir {0}".format(expname))

#
#    copy_from = optpack_path + "/experiment.py"
#    copy_to = "{0}/experiment.py".format(model_name)
#    print("Copying experiment script ........ from: {0}".format(copy_from))
#    print("                                     to: {0}".format(copy_to))
#    os.system("cp {0} {1}".format(copy_from, copy_to))
