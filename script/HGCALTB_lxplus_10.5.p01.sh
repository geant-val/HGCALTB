#!/bin/sh

export G4GCC_VERSION="8"
export G4GCC_PLATFORM="x86_64-centos7-gcc8-opt"
export GEANT4_VERSION="10.5.p01"
export GEANT4_PLATFORM="x86_64-centos7-gcc8-opt-MT"
export GCC_VERSION="10"
export GCC_PLATFORM="x86_64-centos7-gcc10-opt"

source ./HGCALTB_cvmfs_setup.sh

export CC=$(command -v gcc)
export CXX=$(command -v g++)
cmake3 ../HGCALTB $@
make -j$(nproc)
