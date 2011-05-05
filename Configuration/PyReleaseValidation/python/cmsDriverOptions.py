#! /usr/bin/env python

# A Pyrelval Wrapper

import optparse
import sys
import os
import re
import Configuration.PyReleaseValidation
from Configuration.PyReleaseValidation.ConfigBuilder import ConfigBuilder, defaultOptions
import traceback
# Prepare a parser to read the options
usage=\
"""%prog <TYPE> [options].
Example:

%prog reco -s RAW2DIGI,RECO --conditions STARTUP_V4::All --eventcontent RECOSIM
"""
parser = optparse.OptionParser(usage)

expertSettings = optparse.OptionGroup(parser, '===============\n  Expert Options', 'Caution: please use only if you know what you are doing.')
famosSettings = optparse.OptionGroup(parser, '===============\n  FastSimulation options', '')
parser.add_option_group(expertSettings)

parser.add_option("-s", "--step",
                   help="The desired step. The possible values are: "+\
                        "GEN,SIM,DIGI,L1,DIGI2RAW,HLT,RAW2DIGI,RECO,POSTRECO,DQM,ALCA,VALIDATION,HARVESTING, NONE or ALL.",
                   default="ALL",
                   dest="step")

parser.add_option("--conditions",
                  help="What conditions to use. This has to be specified",
                  default=None,
                  dest="conditions")

parser.add_option("--eventcontent",
                  help="What event content to write out. Default=FEVTDEBUG, or FEVT (for cosmics)",
                  default='RECOSIM',
                  dest="eventcontent")

parser.add_option("--filein",
                   help="The infile name.",
                   default="",#to be changed in the default form later
                   dest="filein")

parser.add_option("--fileout",
                   help="The outfile name. If absent a default value is assigned",
                   default="", #to be changed in the default form later
                   dest="fileout")

parser.add_option("--filetype",
                   help="The type of the infile (EDM, LHE or MCDB).",
                   default=defaultOptions.filetype,
                   dest="filetype",
                  choices=['EDM','DAT','LHE','MDCB'])

parser.add_option("-n", "--number",
                  help="The number of events. The default is 1.",
                  default="1",
                  dest="number")

parser.add_option("--mc",
                  help="Specify that simulation is to be processed (default = guess based on options",
                  action="store_true",
                  default=False,
                  dest="isMC")

parser.add_option("--data",
                  help="Specify that data is to be processed (default = guess based on options",
                  action="store_true",
                  default=False,
                  dest="isData")


parser.add_option("--no_exec",
                  help="Do not exec cmsRun. Just prepare the python config file.",
                  action="store_true",
                  default=False,
                  dest="no_exec_flag")   

# expert settings
expertSettings.add_option("--beamspot",
                          help="What beam spot to use (from Configuration/StandardSequences). Default depends on scenario",
                          default=defaultOptions.beamspot,
                          dest="beamspot")

expertSettings.add_option("--customise",
                          help="Specify the file where the code to modify the process object is stored.",
                          default="",
                          dest="customisation_file")
expertSettings.add_option("--customise_commands",
                          help="Specify a string of commands",
                          default="",
                          dest="customise_commands")

expertSettings.add_option("--inline_custom",
                          help="inline the customisation file",
                          default=False,
                          dest="inline_custom")

expertSettings.add_option("--datatier",
                          help="What data tier to use.",
                          default='',
                          dest="datatier")

expertSettings.add_option( "--dirin",
                          help="The infile directory.",
                          default="",
                          dest="dirin")                    

expertSettings.add_option( "--dirout",
                          help="The outfile directory.",
                          default="",
                          dest="dirout")                

expertSettings.add_option("--filtername",
                          help="What filter name to specify in output module",
                          default="",
                          dest="filtername")

expertSettings.add_option("--geometry",
                          help="What geometry to use (from Configuration/StandardSequences). Default=Ideal",
                          default=defaultOptions.geometry,
                          dest="geometry")

expertSettings.add_option("--magField",
                          help="What magnetic field to use (from Configuration/StandardSequences).",
                          default=defaultOptions.magField,
                          dest="magField")

expertSettings.add_option("--no_output",
                          help="Do not write anything to disk. This is for "+\
                          "benchmarking purposes.",
                          action="store_true",
                          default=False,
                          dest="no_output_flag")

expertSettings.add_option("--prefix",
                          help="Specify a prefix to the cmsRun command.",
                          default="",
                          dest="prefix")

expertSettings.add_option("--suffix",
                          help="Specify a suffix to the cmsRun command.",
                          default="",
                          dest="suffix")  

expertSettings.add_option("--relval",
                          help="Set total number of events and events per job.", #this does not get used but get parsed in the command by DatOps
                          default="",
                          dest="relval")

expertSettings.add_option("--dump_python",
                  help="Dump the config file in python "+\
                  "and do a full expansion of imports.",
                  action="store_true",
                  default=False,                  
                  dest="dump_python")

expertSettings.add_option("--dump_DSetName",
                          help="Dump the primary datasetname.",
                          action="store_true",
                          default=False,
                          dest="dump_dsetname_flag")

expertSettings.add_option("--pileup",
                  help="What pileup config to use. Default="+defaultOptions.pileup,
                  default=defaultOptions.pileup,
                  dest="pileup")

expertSettings.add_option("--pileup_input",
                          help="define the pile up files to mix with",
                          default=None,
                          dest="pileup_input")

expertSettings.add_option("--datamix",
                  help="What datamix config to use. Default=DataOnSim.",
                  default=defaultOptions.datamix,
                  dest="datamix")

expertSettings.add_option("--gflash",
                  help="Run the FULL SIM using the GFlash parameterization.",
                  action="store_true",
                  default=defaultOptions.gflash,
                  dest="gflash")

expertSettings.add_option("--himix",
                 help="Run the Heavy Ions signal mixing.",
                 action="store_true",
                 default=defaultOptions.himix,
                 dest="himix")
                                                    
expertSettings.add_option("--python_filename",
                          help="Change the name of the created config file ",
                          default='',
                          dest="python_filename")

expertSettings.add_option("--secondfilein",
                          help="The secondary infile name."+\
                                "for the two-file solution. Default is no file",
                          default="",#to be changed in the default form later
                          dest="secondfilein")

expertSettings.add_option("--processName",
                          help="set process name explicitly",
                          default = None,
                          dest="name" 
                          )

expertSettings.add_option("--triggerResultsProcess",
                          help="for splitting jobs specify from which process to take edm::TriggerResults",
                          default = None,
                          dest="triggerResultsProcess"
                          )

expertSettings.add_option("--hltProcess",
                          help="modify the DQM sequence to look for HLT trigger results with the specified process name", 
                          default = None,
                          dest="hltProcess"
                          )

expertSettings.add_option("--scenario",
                          help="Select scenario overriding standard settings (available:"+str(defaultOptions.scenarioOptions)+")",
                          default='pp',
                          dest="scenario",
                          choices=defaultOptions.scenarioOptions)

expertSettings.add_option("--harvesting",
                          help="What harvesting to use (from Configuration/StandardSequences). Default=AtRunEnd",
                          default=defaultOptions.harvesting,
                          dest="harvesting")

expertSettings.add_option("--particle_table",
                          help="Which particle properties table is loaded. Default=pythia",
                          default=defaultOptions.particleTable,
                          dest="particleTable")

expertSettings.add_option("--dbsquery",
                          help="Allow to define the source.fileNames from the dbs search command",
                          default='',
                          dest="dbsquery")

expertSettings.add_option("--lazy_download",
                  help="Enable lazy downloading of input files",
                  action="store_true",
                  default=False,
                  dest="lazy_download")   

expertSettings.add_option("--repacked",
                          help="When the input file is a file with repacked raw data with label rawDataRepacker",
                          action="store_true",
                          default=False,
                          dest="isRepacked"
                          )

expertSettings.add_option("--custom_conditions",
                          help="Allow to give a few overriding tags for the GT",
                          default='',
                          dest='custom_conditions')

expertSettings.add_option("--inline_eventcontent",
                          help="expand event content definitions",
                          action="store_true",
                          default=False,
                          dest="inlineEventContent")


expertSettings.add_option("--inline_object",
                          help="expand explicitely the definition of a list of objects",
                          default='',
                          dest="inlineObjets")

expertSettings.add_option("--hideGen",
                          help="do not inline the generator information, just load it",
                          default=False,
                          action="store_true")
expertSettings.add_option("--output",
                          help="specify the list of output modules using dict",
                          default='',
                          dest="outputDefinition")

expertSettings.add_option("--inputCommands",
                          help="specify the input commands; i.e dropping products",
                          default=None,
                          dest="inputCommands")

expertSettings.add_option("--inputEventContent",
                          help="specify the input event content",
                          default=None,
                          dest="inputEventContent")

(options,args) = parser.parse_args() # by default the arg is sys.argv[1:]

if not options.conditions:
    raise Exception("the --conditions option is mandatory")


#################################
# Check parameters for validity #
#################################
# A simple check on the consistency of the arguments
if len(sys.argv)==1:
    raise "Event Type: ", "No event type specified!"

# check in case of ALCAOUTPUT case for alca splitting
if options.triggerResultsProcess == None and "ALCAOUTPUT" in options.step:
    print "ERROR: If ALCA splitting is requested, the name of the process in which the alca producers ran needs to be specified. E.g. via --triggerResultsProcess RECO"
    sys.exit(1)
            
            
options.evt_type=sys.argv[1]

# memorize the command line arguments 
options.arguments = reduce(lambda x, y: x+' '+y, sys.argv[1:])

# now adjust the given parameters before passing it to the ConfigBuilder

#trail a "/" to dirin and dirout
if options.dirin!='' and (not options.dirin.endswith('/')):    options.dirin+='/'
if options.dirout!='' and (not options.dirout.endswith('/')):  options.dirout+='/'

# Build the IO files if necessary.
# The default form of the files is:
# <type>_<energy>_<step>.root
prec_step = {"NONE":"",
             "ALL":"",
             "GEN":"",
             "SIM":"GEN",
             "DIGI":"SIM",
             "HLT":"RAW",
             "RECO":"DIGI",
             "ALCA":"RECO",
             "ANA":"RECO",
             "SKIM":"RECO",
             "DIGI2RAW":"DIGI",
             "RAW2DIGI":"DIGI2RAW",
             "RAW2RECO":"DIGI2RAW",
             "DATAMIX":"DIGI",
             "DIGI2RAW":"DATAMIX",
             "HARVESTING":"RECO",
             "ALCAHARVEST":"RECO"}

trimmedEvtType=options.evt_type.split('/')[-1]

#get the list of steps, without their options
options.trimmedStep=[]
for s in options.step.split(','):
    step=s.split(':')[0]
    options.trimmedStep.append(step)
first_step=options.trimmedStep[0]

#replace step aliases
# this does not affect options.trimmedStep which still contains 'NONE'
stepsAliases={
    'NONE':'',
    'ALL':'GEN,SIM,DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,RECO,POSTRECO,VALIDATION,DQM',
    'DATA_CHAIN':'RAW2DIGI,RECO,POSTRECO,DQM'
    }
if options.step in stepsAliases:
    options.step=stepsAliases[options.step]

options.step = options.step.replace("SIM_CHAIN","GEN,SIM,DIGI,L1,DIGI2RAW")



#determine the type of file on input
if options.filetype==defaultOptions.filetype:
    if options.filein.lower().endswith(".lhe") or options.filein.lower().endswith(".lhef"):
        options.filetype="LHE"
    elif options.filein.startswith("mcdb:"):
        options.filetype="MCDB"
    else:
        options.filetype="EDM"

filesuffix = {"LHE": "lhe", "EDM": "root", "MCDB": ""}[options.filetype]

if options.filein=="" and not (first_step in ("ALL","GEN","SIM_CHAIN")):
    options.dirin="file:"+options.dirin.replace('file:','')
    options.filein=trimmedEvtType+"_"+prec_step[first_step]+"."+filesuffix


# Prepare the canonical file name for output / config file etc
#   (EventType_STEP1_STEP2_..._PU)
standardFileName = ""
standardFileName = trimmedEvtType+"_"+"_".join(options.trimmedStep)
standardFileName = standardFileName.replace(",","_").replace(".","_")
if options.pileup != "NoPileUp":
    standardFileName += "_PU"


# if no output file name given, set it to default
if options.fileout=="" and not first_step in ("HARVESTING", "ALCAHARVEST"):
    options.fileout = standardFileName+".root"

# Prepare the name of the config file
python_config_filename = standardFileName+'.py'

# add on the end of job sequence...
addEndJob = True
if ("FASTSIM" in options.step and not "VALIDATION" in options.step) or "HARVESTING" in options.step or "ALCAHARVEST" in options.step or "ALCAOUTPUT" in options.step or options.step == "": 
    addEndJob = False
if ("SKIM" in options.step and not "RECO" in options.step):
    addEndJob = False
if ("ENDJOB" in options.step):
    addEndJob = False
if addEndJob:    
    options.step=options.step+',ENDJOB'
print options.step


# Setting name of process
# if not set explicitly it needs some thinking
if not options.name:
    if 'HLT' in options.trimmedStep:    
        options.name = 'HLT'
    elif 'RECO' in options.trimmedStep:
        options.name = 'RECO'
    elif options.trimmedStep == ['NONE'] and options.filetype in ('LHE', 'MCDB'):
        options.name = 'LHE'
    elif len(options.trimmedStep)==0:
        options.name = 'PROCESS'
    else:
        options.name = options.trimmedStep[-1]

# check to be sure that people run the harvesting as a separate step
isHarvesting = False
isOther = False

if "HARVESTING" in options.trimmedStep and len(options.trimmedStep) > 1:
    raise Exception("The Harvesting step must be run alone")

# if not specified by user try to guess whether MC or DATA
if not options.isData and not options.isMC:
    if 'SIM' in options.trimmedStep:
        options.isMC=True
    if 'CFWRITER' in options.trimmedStep:
        options.isMC=True
    if 'DIGI' in options.trimmedStep:
        options.isMC=True
    if (not (options.eventcontent == None)) and 'SIM' in options.eventcontent:
        options.isMC=True
    if 'SIM' in options.datatier:
        options.isMC=True
    if options.isMC:
        print 'We have determined that this is simulation (if not, rerun cmsDriver.py with --data)'
    else:
        print 'We have determined that this is real data (if not, rerun cmsDriver.py with --mc)'
    

