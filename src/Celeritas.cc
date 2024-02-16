#include "Celeritas.hh"

#include <memory>
#include <G4Threading.hh>
#include <accel/AlongStepFactory.hh>
#include <accel/LocalTransporter.hh>
#include <accel/SetupOptions.hh>
#include <accel/SetupOptionsMessenger.hh>
#include <accel/SharedParams.hh>
#include <celeritas/field/UniformFieldData.hh>
#include <celeritas/io/ImportData.hh>

using namespace celeritas;

// Global shared setup options
SetupOptions& CelerSetupOptions()
{
    static SetupOptions options = [] {
        // Construct setup options the first time CelerSetupOptions is invoked
        // Ideally, we should also set up SetupOptionsMessenger so these can be
        // adjusted
        SetupOptions so;

        // Set along-step factory (no field)
        so.make_along_step = celeritas::UniformAlongStepFactory();

        so.max_num_tracks = 1024 * 4;
        so.initializer_capacity = 1024 * 128 * 4;
        so.max_num_events = 10000;
        so.secondary_stack_factor = 2.0;
        so.ignore_processes = {"CoulombScat"};

        // Use Celeritas "hit processor" to call back to Geant4 SDs.
        so.sd.enabled = true;

        // Only call back for nonzero energy depositions: this is currently a
        // global option for all detectors, so if any SDs extract data from
        // tracks with no local energy deposition over the step, it must be set
        // to false.
        so.sd.ignore_zero_deposition = true;

        // Using the pre-step point, reconstruct the G4 touchable handle.
        so.sd.locate_touchable = true;
        // Reconstruct the track, needed for particle type
        so.sd.track = true;

        // Pre-step time and location are used
        so.sd.pre.position = true;
        so.sd.pre.global_time = true;
        return so;
    }();

    // Add hook to configure Celeritas from macro file
    static auto mess = std::make_unique<SetupOptionsMessenger>(&options);

    return options;
}

// Shared data and GPU setup
SharedParams& CelerSharedParams()
{
    static SharedParams sp;
    return sp;
}

// Thread-local transporter
LocalTransporter& CelerLocalTransporter()
{
    static G4ThreadLocal LocalTransporter lt;
    return lt;
}

// Thread-local offload interface
SimpleOffload& CelerSimpleOffload()
{
    static G4ThreadLocal SimpleOffload so;
    return so;
}
