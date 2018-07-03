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
    print("Reading optpack configuration ...    " + conf_file_path)
    conf_optpack = parse_yaml_file(conf_file_path)

    print("Preparing model suite ...            " + model_name)

    if os.path.exists(model_name):
        print("Directory already exists ...")
        print("Exiting ...")
        sys.exit(1)

    print("Creating directory ...               {0}/".format(model_name))
    os.system("mkdir {0}".format(model_name))

    print("Creating directory ...               {0}/model/".format(model_name))
    os.system("mkdir {0}/model/".format(model_name))

    model_petsc_file_path = conf_optpack["model"]["petsc"]
    model_petsc_file = os.path.basename(model_petsc_file_path)
    print("Copying environment file ...         {0}/model/{1}".format(model_name, model_petsc_file))
    os.system("cd {0}/model/; cp {1} .".format(model_name, model_petsc_file_path))

    print("Compiling executable ...             {0}/model/metos3d-simpack-{0}.exe".format(model_name))
    os.system('''
cd {0}/model/;
source ./de.uni-kiel.rz.nesh-fe.petsc-3.3-p7.opt.sh

# links
ln -s ../../../../../../metos3d/data/data/
ln -s ../../../../../../metos3d/model/model/
ln -s ../../../../../../metos3d/simpack/
ln -s ../../../../../../metos3d/metos3d/Makefile

# compile
#make BGC=model/{0} clean &> /dev/null
#make BGC=model/{0} &> /dev/null
'''.format(model_name))

    print("Copying job template ...             {0}/{1}/template/template.job.sh".format(model_name, language_name))
    os.system("cp optpack/{1}/template/template.job.sh {0}/.".format(model_name, language_name))

#    print("Copying start template ...           {0}/{1}/template.start.py".format(model_name))
#    os.system("cp optpack/template/template.start.py {0}/.".format(model_name))



    print("Copying model configuration ...      {0}/conf.yaml".format(model_name))
    os.system("cp optpack/conf/{0}.conf.yaml {0}/conf.yaml".format(model_name))

    print("Copying experiment script ...        {0}/experiment.py".format(model_name))
    os.system("cp optpack/experiment.py {0}/.".format(model_name))



#    print("Preparing python scripts ...")
#    print("Creating directory ...               {0}/python/".format(model_name))
#    os.system("mkdir {0}/python/".format(model_name))
#
#    print("Copying python scripts ...")
#    os.system("cp optpack/python/*.py {0}/python/.".format(model_name))



