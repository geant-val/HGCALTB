##**************************************************
## \file parser.py
## \brief: Implementation of HGCALTB parser
##         for geant-val usage
## \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
##          @lopezzot
## \start date: 16 January 2024
##**************************************************

#!/usr/bin/env python3

import os.path
import os
import subprocess

from gts.BaseParser import BaseParser, mergeROOT, mktemp, mktempdir
from gts.utils import getJSON
import ROOT

from collections import defaultdict

class Test(BaseParser):
    TEST = "HGCALTB"
    IGNOREKEYS = ["ENERGY"] # we nedd all energies in the analysis

    def parse(self,jobs):
        if jobs[0]['PARTICLE'] == "pi-":
            # PION ANALYSIS
            energies = ['20.','50.','80.','100.','120.','200.','250.','300.']
            pionjobs = [job for job in jobs if job['PARTICLE'] == "pi-"]
            physlist = pionjobs[0]['PHYSLIST']
            g4ver = pionjobs[0]['VERSION']
            print('start parsing ' + str(len(pionjobs)) + ' files from Geant4 ' + g4ver + ' using ' + physlist)
            dirName = 'analysis_'+g4ver+'_'+physlist
            os.mkdir(dirName)
            # prepare analysis_* folders and copy data and root macro
            # group jobs by energy
            energy_jobs_map = defaultdict(list)
            for job in pionjobs:
                energy_jobs_map[job['ENERGY']].append(job)

            for counter, energy in enumerate(energies):
                if energy in energy_jobs_map:
                    energy_jobs = energy_jobs_map[energy]

                    # merge all HGCALTBout_Run0.root from jobs with this energy
                    mergeroot = [os.path.join(job["path"], "HGCALTBout_Run0.root") for job in energy_jobs]
                    tmpdir = mktempdir()
                    merged_output = os.path.join(tmpdir, "HGCALTBout_Run0.root")
                    mergeROOT(mergeroot, merged_output)

                    # copy merged ROOT file to analysis directory
                    dest_root = os.path.join(dirName, f"HGCALTBout_Run{counter}.root")
                    os.system(f"cp {merged_output} {dest_root}")

                    # copy the macro from the first job with this energy
                    macro_path = os.path.join(energy_jobs[0]["path"], "energy.C")
                    os.system(f"cp {macro_path} {dirName}")


            cmnd = ['root', '-l', '-b', '-q', 'energy.C("")']
            proc = subprocess.Popen(cmnd, cwd=dirName, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
            output, error = proc.communicate()
            print("ROOT Output:", output)
            print("ROOT Error:", error)
            # get data from TGraphs
            tf = ROOT.TFile(os.path.join(dirName, 'HGCALTBpiene.root'), 'READ')
            cee_resp_pi = tf.Get('CEEresponse')
            che_resp_pi = tf.Get('CHEresponse')
            cee_resl_pi = tf.Get('CEEresolutioncorrected')
            che_resl_pi = tf.Get('CHEresolutioncorrected')
            # create json files
            # simulation: cee response
            yield getJSON(pionjobs[0], 'chart',
                mctool_name='GEANT4',
                mctool_model=physlist,
                targetName='CMS-HGCAL',
                beamParticle='pi-',
                beamEnergies=list(cee_resp_pi.GetX()),
                observableName='CEE pion response',
                xAxisName=cee_resp_pi.GetXaxis().GetTitle(),
                yAxisName=cee_resp_pi.GetYaxis().GetTitle(),
                xValues=list(cee_resp_pi.GetX()),
                yValues=list(cee_resp_pi.GetY()),
                yStatErrorsMinus=[0.]*len(list(cee_resp_pi.GetX())),
                yStatErrorsPlus=[0.]*len(list(cee_resp_pi.GetX())),
            )
            # simulation: che response
            yield getJSON(pionjobs[0], 'chart',
                mctool_name='GEANT4',
                mctool_model=physlist,
                targetName='CMS-HGCAL',
                beamParticle='pi-',
                beamEnergies=list(che_resp_pi.GetX()),
                observableName='CHE pion response',
                xAxisName=che_resp_pi.GetXaxis().GetTitle(),
                yAxisName=che_resp_pi.GetYaxis().GetTitle(),
                xValues=list(che_resp_pi.GetX()),
                yValues=list(che_resp_pi.GetY()),
                yStatErrorsMinus=[0.]*len(list(che_resp_pi.GetX())),
                yStatErrorsPlus=[0.]*len(list(che_resp_pi.GetX())),
            )
            # simulation: cee resolution
            yield getJSON(pionjobs[0], 'chart',
                mctool_name='GEANT4',
                mctool_model=physlist,
                targetName='CMS-HGCAL',
                beamParticle='pi-',
                beamEnergies=list(cee_resl_pi.GetX()),
                observableName='CEE pion resolution',
                xAxisName=cee_resl_pi.GetXaxis().GetTitle(),
                yAxisName=cee_resl_pi.GetYaxis().GetTitle(),
                xValues=list(cee_resl_pi.GetX()),
                yValues=list(cee_resl_pi.GetY()),
                yStatErrorsMinus=[0.]*len(list(cee_resl_pi.GetX())),
                yStatErrorsPlus=[0.]*len(list(cee_resl_pi.GetX())),
            )
            # simulation: che resolution
            yield getJSON(pionjobs[0], 'chart',
                mctool_name='GEANT4',
                mctool_model=physlist,
                targetName='CMS-HGCAL',
                beamParticle='pi-',
                beamEnergies=list(che_resl_pi.GetX()),
                observableName='CHE pion resolution',
                xAxisName=che_resl_pi.GetXaxis().GetTitle(),
                yAxisName=che_resl_pi.GetYaxis().GetTitle(),
                xValues=list(che_resl_pi.GetX()),
                yValues=list(che_resl_pi.GetY()),
                yStatErrorsMinus=[0.]*len(list(che_resl_pi.GetX())),
                yStatErrorsPlus=[0.]*len(list(che_resl_pi.GetX())),
            )
            # data: CMS HGCAL TB paper arxiv.org/abs/2211.04740
            # create JSON files for experimental comparison only
            # once.
            tbenergies = [20.,50.,80.,100.,120.,200.,250.,300.]
            tbEX = [0.]*len(tbenergies)
            tbEY = tbEX
            tbceeresp=[0.669,0.785,0.831,0.850,0.869,0.900,0.919,0.927]
            tbcheresp=[0.921,1.000,1.039,1.055,1.062,1.076,1.088,1.091]
            tbceeresl=[0.307,0.215,0.178,0.160,0.149,0.124,0.116,0.105]
            tbcheresl=[0.279,0.197,0.165,0.160,0.144,0.126,0.119,0.113]
            if physlist == "FTFP_BERT": # do only once
                print('Creating JSONs for experimental data')
                # cee response
                yield getJSON(pionjobs[0], 'chart',
                    mctool_name="experiment",
                    mctool_version="experiment",
                    mctool_model="experiment",
                    testName="experiment",
                    #inspireId = ,
                    targetName='CMS-HGCAL',
                    beamParticle='pi-',
                    beamEnergies=tbenergies,
                    observableName='CEE pion response',
                    xAxisName=cee_resp_pi.GetXaxis().GetTitle(),
                    yAxisName=cee_resp_pi.GetYaxis().GetTitle(),
                    xValues=tbenergies,
                    yValues=tbceeresp,
                    yStatErrorsMinus=tbEX,
                    yStatErrorsPlus=tbEY,
                )
                # che response
                yield getJSON(pionjobs[0], 'chart',
                    mctool_name="experiment",
                    mctool_version="experiment",
                    mctool_model="experiment",
                    testName="experiment",
                    #inspireId = ,
                    targetName='CMS-HGCAL',
                    beamParticle='pi-',
                    beamEnergies=tbenergies,
                    observableName='CHE pion response',
                    xAxisName=che_resp_pi.GetXaxis().GetTitle(),
                    yAxisName=che_resp_pi.GetYaxis().GetTitle(),
                    xValues=tbenergies,
                    yValues=tbcheresp,
                    yStatErrorsMinus=tbEX,
                    yStatErrorsPlus=tbEY,
                )
                # cee resolution
                yield getJSON(pionjobs[0], 'chart',
                    mctool_name="experiment",
                    mctool_version="experiment",
                    mctool_model="experiment",
                    testName="experiment",
                    #inspireId = ,
                    targetName='CMS-HGCAL',
                    beamParticle='pi-',
                    beamEnergies=tbenergies,
                    observableName='CEE pion resolution',
                    xAxisName=cee_resl_pi.GetXaxis().GetTitle(),
                    yAxisName=cee_resl_pi.GetYaxis().GetTitle(),
                    xValues=tbenergies,
                    yValues=tbceeresl,
                    yStatErrorsMinus=tbEX,
                    yStatErrorsPlus=tbEY,
                )
                # che resolution
                yield getJSON(pionjobs[0], 'chart',
                    mctool_name="experiment",
                    mctool_version="experiment",
                    mctool_model="experiment",
                    testName="experiment",
                    #inspireId = ,
                    targetName='CMS-HGCAL',
                    beamParticle='pi-',
                    beamEnergies=tbenergies,
                    observableName='CHE pion resolution',
                    xAxisName=che_resl_pi.GetXaxis().GetTitle(),
                    yAxisName=che_resl_pi.GetYaxis().GetTitle(),
                    xValues=tbenergies,
                    yValues=tbcheresl,
                    yStatErrorsMinus=tbEX,
                    yStatErrorsPlus=tbEY,
                )
        else:
            # POSITRON ANALYSIS
            emenergies = ["20.","100.","300."]
            emenergiesnum = [20,100,300]
            physlist = jobs[0]['PHYSLIST']
            g4ver = jobs[0]['VERSION']
            print('start parsing ' + str(len(jobs)) + ' files from Geant4 ' + g4ver + ' using ' + physlist)
            emdirName = 'emanalysis_'+g4ver+'_'+physlist
            os.mkdir(emdirName)
            # prepare analysis_* folders and copy data and root macro
            for counter, energy in enumerate(emenergies):
                energy_jobs = [job for job in jobs if job['ENERGY'] == energy]
                if not energy_jobs:
                    continue
                print(f"[INFO] Merging {len(energy_jobs)} jobs for e+ at {energy} GeV:")
                merged_file = os.path.join(emdirName, f'HGCALTBout_Run{counter}.root')
                input_files = [os.path.join(j["path"], "HGCALTBout_Run0.root") for j in energy_jobs]
                mergeROOT(input_files, merged_file)
                os.system('cp ' + energy_jobs[0]['path'] + '/emprofile.C ' + emdirName)
            # run the analysis inside each analysis_* folder
            for counter, e in enumerate(emenergiesnum):
                emcmnd = ['root', '-l', '-b', '-q', 'emprofile.C("HGCALTBout_Run'+str(counter)+'.root",'+'"'+str(e)+'"'+','+'"'+str(physlist)+'"'+')']
                emproc = subprocess.Popen(emcmnd, cwd=emdirName, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
                emoutput, emerror = emproc.communicate()
                print("ROOT Output:", emoutput)
                print("ROOT Error:", emerror)
            # get data from TGraphs and create json files
            for counter, e in enumerate(emenergiesnum):
                tf = ROOT.TFile(os.path.join(emdirName, 'EmProf'+str(e)+'_'+physlist+".root"), 'READ')
                fullemprof = tf.Get('FullEmProfile')
                yield getJSON(jobs[0], 'chart',
                    mctool_name='GEANT4',
                    mctool_model=physlist,
                    targetName='CMS-HGCAL',
                    beamParticle='e+',
                    beamEnergies=[e],
                    observableName='Longitudinal Shower Profile',
                    xAxisName=fullemprof.GetXaxis().GetTitle(),
                    yAxisName=fullemprof.GetYaxis().GetTitle(),
                    xValues=list(fullemprof.GetX()),
                    yValues=list(fullemprof.GetY()),
                    yStatErrorsMinus=[0.]*len(list(fullemprof.GetX())),
                    yStatErrorsPlus=[0.]*len(list(fullemprof.GetX())),
                )
        print("End of parsing")
