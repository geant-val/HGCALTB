#!/bin/sh
# We cannot include TBHGCal181Oct.gdml in the "files" copied to every job, as it would significantly increase the total size used by all jobs.
cp %PATH2GDML%/TBHGCal181Oct.gdml .
HGCALTB -m HGCALTB.mac -p %PHYSLIST% 
rm TBHGCal181Oct.gdml
