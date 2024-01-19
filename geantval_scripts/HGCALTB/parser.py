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

from gts.BaseParser import BaseParser, mergeROOT, mktemp
from gts.utils import getJSON
import ROOT

class Test(BaseParser):
    TEST = "HGCALTB"
    IGNOREKEYS = ["ENERGY"] # we nedd all energies in the analysis
    def parse(self,jobs):
        energies = ['20.','50.','80.','100.','120.','200.','250.','300.']
        physlist = jobs[0]['PHYSLIST']
        g4ver = jobs[0]['VERSION']
        print('start parsing ' + str(len(jobs)) + ' files from Geant4 ' + g4ver + ' using ' + physlist)
        dirName = 'analysis_'+g4ver+'_'+physlist
        os.mkdir(dirName)
        # prepare analysis_* folders and copy data and root macro
        for job in jobs:
            for counter, energy in enumerate(energies):
                if job['ENERGY'] == energies[counter] :
                    os.system('cp ' + job['path'] + '/HGCALTBout_Run0.root ' + dirName + '/HGCALTBout_Run' + str(counter) + '.root')
                    os.system('cp ' + job['path'] + '/energy.C ' + dirName)
        # run the analysis inside each analysis_* folder
        cmnd = ['root', '-l', '-b', '-q', 'energy.C("")']
        proc = subprocess.Popen(cmnd, cwd=dirName, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        output, error = proc.communicate()
        print("Output:", output)
        print("Error:", error)
        # get data from TGraphs
        tf = ROOT.TFile(os.path.join(dirName, 'HGCALTBpiene.root'), 'READ')
        cee_resp_pi = tf.Get('CEEresponse')
        che_resp_pi = tf.Get('CHEresponse')
        cee_resl_pi = tf.Get('CEEresolutioncorrected')
        che_resl_pi = tf.Get('CHEresolutioncorrected')
        # create json files
        # simulation: cee response
        yield getJSON(jobs[0], 'chart',
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
        yield getJSON(jobs[0], 'chart',
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
        yield getJSON(jobs[0], 'chart',
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
        yield getJSON(jobs[0], 'chart',
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
