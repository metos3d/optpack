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
def format_text(text_template, config_dict):
    text = ""
    for line in text_template.splitlines(keepends=True):
        try:
            text = text + line.format(**config_dict)
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
# main
# ---------------------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    
    model_list = ["N", "N-DOP", "NP-DOP", "NPZ-DOP", "NPZD-DOP"]
    language_list = ["matlab", "python", "c"]
    language_extensions = {"matlab": {"code": "m", "data": "mat"}, "python": {"code": "py", "data": "h5"}, "c": {"code": "c", "data": "h5"}}
    
    if len(sys.argv[:]) < 3:
        print("usage: python {0} [model-name] [language]".format(sys.argv[0]))
        print("")
        print("options:")
        print("[model-name]     ", ", ".join(model_list))
        print("[language]       ", ", ".join(language_list))
        print("")
        print("example:")
        print("$> python {0} N matlab".format(sys.argv[0]))
        sys.exit(0)
    
    model_name = sys.argv[1]
    if not model_name in model_list:
        print("Model not known ... " + model_name)
        print("Choose from ... ", ", ".join(model_list))
        print("Exiting ...")
        sys.exit(1)
    
    language_name = sys.argv[2]
    if not language_name in language_list:
        print("Language not known ... " + language_name)
        print("Choose from ... ", ", ".join(language_list))
        print("Exiting ...")
        sys.exit(1)

    conf_file_path = os.path.join(os.path.dirname(__file__), "optpack.conf.yaml")
    print("Reading optpack configuration ...... " + conf_file_path)
    conf_optpack = parse_yaml_file(conf_file_path)

    print("Preparing model suite .................. " + model_name)

    if os.path.exists(model_name):
        print("Directory already exists ...")
        print("Exiting ...")
        sys.exit(1)
        
    print("Creating directory ..................... {0}/".format(model_name))
    os.system("mkdir {0}".format(model_name))
    
    print("Creating directory ..................... {0}/model/".format(model_name))
    os.system("mkdir {0}/model/".format(model_name))
    
    copy_from = conf_optpack["model"]["petsc"]
    copy_to = "{0}/model/petsc.env.sh".format(model_name)
    print("Copying environment file ......... from: {0}".format(copy_from))
    print("                                     to: {0}".format(copy_to))
    os.system("cp {0} {1}".format(copy_from, copy_to))
    
    model_metos3d_path = conf_optpack["model"]["metos3d"]
    print("Compiling executable ................... {0}/model/metos3d-simpack-{0}.exe".format(model_name))
    os.system('''
cd {0}/model/;
source ./petsc.env.sh

# links
ln -s {1}/data/data/
ln -s {1}/model/model/
ln -s {1}/simpack/
ln -s {1}/metos3d/Makefile

# compile
#make BGC=model/{0} clean &> /dev/null
#make BGC=model/{0} &> /dev/null
'''.format(model_name, model_metos3d_path))
    
    copy_from = "optpack/language/{0}/template/template.job.sh".format(language_name)
    copy_to = "{0}/template.job.sh".format(model_name)
    print("Copying job template ............. from: {0}".format(copy_from))
    print("                                     to: {0}".format(copy_to))
    os.system("cp {0} {1}".format(copy_from, copy_to))

    extension_code = language_extensions[language_name]["code"]
    copy_from = "optpack/language/{0}/template/template.start.{1}".format(language_name, extension_code)
    copy_to = "{0}/template.start.{1}".format(model_name, extension_code)
    print("Copying start template ........... from: {0}".format(copy_from))
    print("                                     to: {0}".format(copy_to))
    os.system("cp {0} {1}".format(copy_from, copy_to))

    copy_from = "optpack/experiment/{0}.conf.yaml".format(model_name)
    copy_to = "{0}/experiment.conf.yaml".format(model_name)
    print("Preparing experiment configuration ..... " + copy_to)
    print("Formatting template .................... " + copy_from)
    experiment_conf_template = read_template(copy_from)
    format_conf = {"language": {"name": language_name}}
    experiment_conf = format_text(experiment_conf_template, format_conf)
    write_text_file(copy_to, experiment_conf)

#    print("Copying experiment script ...        {0}/experiment.py".format(model_name))
#    os.system("cp optpack/experiment.py {0}/.".format(model_name))

    print("Preparing {0} codes ...".format(language_name))
    dir_name = "{0}/{1}/".format(model_name, language_name)
    print("Creating directory ..................... {0}".format(dir_name))
    os.system("mkdir {0}".format(dir_name))

    print("Copying {0} codes ...".format(language_name))
    if language_name=="c":
        copy_to = "{0}/{1}/.".format(model_name, language_name)
        copy_from = "optpack/language/{0}/*.c".format(language_name)
        print("                                   from: {0}".format(copy_from))
        os.system("cp {0} {1}".format(copy_from, copy_to))
        copy_from = "optpack/language/{0}/*.h".format(language_name)
        print("                                   from: {0}".format(copy_from))
        os.system("cp {0} {1}".format(copy_from, copy_to))
        copy_from = "optpack/language/{0}/Makefile")
        print("                                   from: {0}".format(copy_from))
        os.system("cp {0} {1}".format(copy_from, copy_to))
        print("                                     to: {0}".format(copy_to))
    else:
        copy_from = "optpack/language/{0}/*.{1}".format(language_name, extension_code)
        copy_to = "{0}/{1}/.".format(model_name, language_name)
        print("                                   from: {0}".format(copy_from))
        print("                                     to: {0}".format(copy_to))
        os.system("cp {0} {1}".format(copy_from, copy_to))




