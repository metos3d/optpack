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
# create job script
# ---------------------------------------------------------------------------------------------------------------------
def create_job_script(expname, exp_config, nexp, niter):
    
    job_template_text = read_template("template.job.sh")
    job_text = format_text(job_template_text, exp_config, nexp, niter)

    modname = exp_config["model"]["name"]
    job_text_file = os.path.join(expname, expname + "." + modname + "." + nexp + ".job.sh")
    print(job_text_file)
    
    write_text_file(job_text_file, job_text)

# ---------------------------------------------------------------------------------------------------------------------
# create start script
# ---------------------------------------------------------------------------------------------------------------------
def create_start_script(expname, exp_config, nexp, niter):
    
    start_template_text = read_template("template.start.py")
    start_text = format_text(start_template_text, exp_config, nexp, niter)
    
    modname = exp_config["model"]["name"]
    start_text_file = os.path.join(expname, expname + "." + modname + "." + nexp + ".start.py")
    print(start_text_file)
    
    write_text_file(start_text_file, start_text)

# ---------------------------------------------------------------------------------------------------------------------
# prepare new experiment
# ---------------------------------------------------------------------------------------------------------------------
def prepare_new_experiment(exp_config, expname, niter):
    print("Preparing new experiment ...         " + expname)
    
    print("Creating experiment directory ...    {0}/".format(expname))
    os.system("mkdir {0}".format(expname))
    print("Creating option directory ...        {0}/option".format(expname))
    os.system("mkdir {0}/option".format(expname))
    print("Creating log directory ...           {0}/log".format(expname))
    os.system("mkdir {0}/log".format(expname))
    
    print("Creating initial job script ...      ", end="")
    create_job_script(expname, exp_config, "1", niter)
    print("Creating initial start script ...    ", end="")
    create_start_script(expname, exp_config, "1", niter)

# ---------------------------------------------------------------------------------------------------------------------
# continue experiment
# ---------------------------------------------------------------------------------------------------------------------
def continue_experiment(exp_config, expname, niter):
    print("Continuing experiment ...            " + expname)

    modname = exp_config["model"]["name"]
    log_file_pattern = os.path.join(expname, expname + "." + modname + ".*.h5")
    print("Checking pattern ...                 {0}".format(log_file_pattern))
    log_file_list = glob.glob(log_file_pattern)
    
    nexp = len(log_file_list)
    if nexp==0:
        print("No previous runs found ... ")
        print("Exiting ...")
        sys.exit(1)
    
    for log_file in log_file_list:
        print("Found log file ...                   {0}".format(log_file))

    print("Reading last result ...")
    uopt = h5py.File(log_file_list[-1])["uopt"][0,:]

    nexp = nexp + 1
    print("Continuing with optimization run ... {0}".format(nexp))

    u0str = "["
    for ui in uopt:
        uistr = "%.16e" % ui
        print("Using parameters ...                 {0}".format(uistr))
        u0str = u0str + uistr + ","
    u0str = u0str[:-1] + "]"
    exp_config["parameter"]["u0"] = u0str

    print("Creating job script ...              ", end="")
    create_job_script(expname, exp_config, str(nexp), niter)
    print("Creating start script ...            ", end="")
    create_start_script(expname, exp_config, str(nexp), niter)

# ---------------------------------------------------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv[:]) < 4:
        print('''usage: python {0} [new|continue] [experiment-name] [iteration-count]
example:
$> python {0} new exp-01 10'''.format(sys.argv[0]))
        sys.exit(0)

    command = sys.argv[1]
    expname = sys.argv[2]
    niter = sys.argv[3]

    exp_config = parse_yaml_file("conf.yaml")
    # strip any trailing path separators
    expname = os.path.normpath(expname)
    exp_config["experiment"] = {"name": expname}

    if command=="new":
        if os.path.exists(expname):
            print("Directory already exists ... " + expname)
            print("Exiting ...")
            sys.exit(1)
        
        prepare_new_experiment(exp_config, expname, niter)

    elif command=="continue":
        if not os.path.exists(expname):
            print("Experiment directory does not exists ... " + expname)
            print("Exiting ...")
            sys.exit(1)
        
        continue_experiment(exp_config, expname, niter)

    else:
        print("Unknown command ... " + command)
        print("Exiting ...")
        sys.exit(1)




