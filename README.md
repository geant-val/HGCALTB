# HGCALTB
A Geant4 simulation of the 2018 CMS HGCAL test-beam for geant-val.

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#project-description">Project description</a></li>
    <li><a href="#authors-and-contacts">Authors and contacts</a></li>
    <li>
      <a href="#results">Results</a>
      <ul>
        <li><a href="#selected-reports-and-presentations">Selected reports and presentations</a></li>
        <li><a href="#available-datasets-and-analyses">Available datasets and analyses</a></li>
      </ul>
    </li>
    <li>
      <a href="#how-to">How to</a>
      <ul>
        <li><a href="#build-compile-and-execute-on-maclinux">Build, compile and execute on Mac/Linux</a></li>
        <li><a href="#build-compile-and-execute-on-lxplus">Build, compile and execute on lxplus</a></li>
        <li><a href="#submit-a-job-with-htcondor-on-lxplus">Submit a job with HTCondor on lxplus</a></li>
      </ul>
    </li>
    <li><a href="#geant-val-integration">Geant Val integration</a></li>
    <li><a href="#selected-cms-hgcal-test-beam-references">Selected CMS HGCAL test beam references</a></li>
  </ol>
</details>

<!--Project desription-->
## Project description
The project targets a standalone Geant4 simulation of the [2018 CMS HGCAL test beam](https://arxiv.org/abs/2211.04740) to perform Geant4 regression testing, physics lists comparison and validation against test-beam data with geant-val.
- ⏰ Start date: 11 January 2024
- 📌 Status: development

<!--Authors and contacts-->
## Authors and contacts
- 👨‍🔬 Lorenzo Pezzotti (CERN EP-SFT) - lorenzo.pezzotti@cern.ch 
- 👨‍🔬 Supervisor: Alberto Ribon (CERN EP-SFT)

<!--Results-->
## Results

### Selected reports and presentations

### Available datasets and analyses

<!--How to-->
## How to

### Build, compile and execute on Mac/Linux
1.  git clone the repo
    ```sh
    git clone https://github.com/geant-val/HGCALTB.git
    ```
2.  source Geant4 env
    ```sh
    source /relative_path_to/geant4.11.2-install/bin/geant4.sh
    ```
3.  cmake build directory and make
    ```sh
    mkdir HGCALTB-build && cd HGCALTB-build/
    cmake -DGeant4_DIR=/absolute_path_to/geant4.11.2-install/lib/Geant4-11.2/ relative_path_to/HGCALTB/
    make
    ```
4.  execute (example with HGCALTBrun.mac macro card, 2 threads and FTFP_BERT physics list)
    ```sh
    ./HGCALTB -m HGCALTBrun.mac -t 2 -p FTFP_BERT
    ```

### Build, compile and execute on lxplus
1. git clone the repo
   ```sh
   git clone https://github.com/geant-val/HGCALTB.git
   ```
2. cmake build directory and make (using geant4-11.2, check for gcc and cmake dependencies for other versions)
   ```sh
   mkdir HGCALTB-build && cd HGCALTB-build/
   cp ../HGCALTB/scripts/HGCALTB_cvmfs_setup.sh ../HGCALTB/scripts/HGCALTB_lxplus_11.2.sh .
   source ./HGCALTB_lxplus_11.2.sh
   ```
3. execute (example with HGCALTBrun.mac macro card, 4 threads and FTFP_BERT physics list)
   ```sh
   ./HGCALTB -m HGCALTBrun.mac -t 4 -p FTFP_BERT
   ```

### Submit a job with HTCondor on lxplus
1. [First follow the build instructions on lxplus](#build-compile-and-execute-on-lxplus)
2. prepare for HTCondor submission (example with Geant4.11.2, HGCALTBrun.mac, 4 threads, FTFP_BERT physics list)
    ```sh
    mkdir -p error log output
    cp ../HGCALTB/scripts/HGCALTB_HTCondor.sub ../HGCALTB/scripts/HGCALTB_HTCondor_11.2.sh .
    sed -i "2 i cd $(pwd)" HGCALTB_HTCondor_11.2.sh
    echo ./HGCALTB -m HGCALTBrun.mac -t 4 -p FTFP_BERT >> HGCALTB_HTCondor_11.2.sh
    sed -i "1 i executable = HGCALTB_HTCondor_11.2.sh" HGCALTB_HTCondor.sub
    ```
3. submit a job
   ```sh
   condor_submit HGCALTB_HTCondor.sub 
   ```
4. monitor the job
   ```sh
   condor_q
   ```
   or (for persistency)
   ```sh
   condor_wait -status log/*.log
   ```
5. additional info from HTCondor (optional) \
   rm all your jobs
    ```sh
   condor_rm username
   ```
   inspect your accounting group
   ```sh
   condor_q owner $LOGNAME -long | grep '^AccountingGroup' | sort | uniq -c
   ```
   display all accounting groups
   ```sh
   haggis group list
   ```
   display your accounting groups
    ```sh
   haggis rights
   ```
   check what accounting group a job has
   ```sh
   condor_q jobid.0 -af AccountingGroup
   ```
   specify the accounting group for yout job, to be added in the .sub file
   ```sh
   +AccountingGroup = "group_u_*"
   ```
   check job CPU usage
   ```sh
   condor_q -l jobid.0 | grep CPUsUsage
   ```
   ssh to machine where job is running
   ```sh
   condor_ssh_to_job jobid.0
   ```
