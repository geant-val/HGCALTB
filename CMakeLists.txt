##**************************************************
## \file CMakeLists.txt
## \brief: CMakeLists of HGCALTB project
## \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
##          @lopezzot
## \start date: 10 January 2024
##**************************************************

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.12...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(HGCALTB)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

option(WITH_CELERITAS "Build with Celeritas support" OFF)
if(WITH_CELERITAS)
  find_package(Celeritas 0.4.1 REQUIRED)
  if(NOT CELERITAS_USE_Geant4)
    message(SEND_ERROR "This example requires Geant4 support "
      "to be enabled in Celeritas")
  endif()
  include(CeleritasLibrary)
endif()

#----------------------------------------------------------------------------
# Output pedantic warnings
#
add_compile_options(-Wall -Wextra -Wpedantic)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
		    ${FLUKAInterface_INCLUDE_DIR})
set(sources
  src/HGCALTBAHCALSD.cc
  src/HGCALTBAHHit.cc
  src/HGCALTBActInitialization.cc
  src/HGCALTBCEEHit.cc
  src/HGCALTBCEESD.cc
  src/HGCALTBCHEHit.cc
  src/HGCALTBCHESD.cc
  src/HGCALTBDetConstruction.cc
  src/HGCALTBEventAction.cc
  src/HGCALTBPrimaryGenAction.cc
  src/HGCALTBRunAction.cc
  src/HGCALTBStepAction.cc
  src/HGCALTBTrackAction.cc
)

if(WITH_CELERITAS)
  list(APPEND sources src/Celeritas.cc)
endif()
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(HGCALTB HGCALTB.cc ${sources} ${headers})
target_link_libraries(HGCALTB ${Geant4_LIBRARIES} ${FLUKAInterface_LIBRARIES})
set_target_properties(HGCALTB PROPERTIES CXX_STANDARD 17)
if(WITH_CELERITAS)
  celeritas_target_link_libraries(HGCALTB Celeritas::accel)
  # TODO: no target_compile_definitions ?
  celeritas_target_compile_options(HGCALTB PRIVATE "-DUSE_CELERITAS")
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build HGCALTB
#
set(HGCALTB_SCRIPTS
    TBHGCal181Oct.gdml
    icons.mac
    gui.mac
    init_vis.mac
    vis.mac
    HGCALTBrun.mac
    HGCALTBfullrun.mac
  )

foreach(_script ${HGCALTB_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(G4HGCALTB DEPENDS HGCALTB)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS HGCALTB DESTINATION bin)

##**************************************************
