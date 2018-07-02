import sys
import os

# main
if __name__ == "__main__":
    
    model_list = ["N", "N-DOP", "NP-DOP", "NPZ-DOP", "NPZD-DOP"]
    language_list = ["matlab", "python", "c"]
    
    if len(sys.argv[:]) < 3:
        print("usage: python {0} [model-name] [language]".format(sys.argv[0]))
        print("")
        print("options:")
        print("[model-name]     ", model_list)
        print("[language]       ", language_list)
        print("")
        print("example:")
        print("$> python {0} N matlab".format(sys.argv[0]))
        sys.exit(0)
    
    model_name = sys.argv[1]
    if not model_name in model_list:
        print("Model not known ... " + model_name)
        print("Choose from ... ", model_list)
        print("Exiting ...")
        sys.exit(1)
    
    language_name = sys.argv[2]
    if not language_name in language_list:
        print("Language not known ... " + language_name)
        print("Choose from ... ", language_list)
        print("Exiting ...")
        sys.exit(1)
    
    print("Preparing model suite ...            " + model_name)

    if os.path.exists(model_name):
        print("Directory already exists ...")
        print("Exiting ...")
        sys.exit(1)

    print("Creating directory ...               {0}/".format(model_name))
    os.system("mkdir {0}".format(model_name))

    print("Creating directory ...               {0}/model/".format(model_name))
    os.system("mkdir {0}/model/".format(model_name))

    print("Copying environment file ...         {0}/model/de.uni-kiel.rz.nesh-fe.petsc-3.3-p7.opt.sh".format(model_name))
    os.system("cd {0}/model/; cp ../../../../../../petsc/de.uni-kiel.rz.nesh-fe.petsc-3.3-p7.opt.sh .".format(model_name))

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




