// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban, Asher Elmquist, Evan Hoerl, Shuo He, Rainer Gericke
// =============================================================================
//
// Wrapper classes for modeling an entire MAN 5t vehicle assembly
// (including the vehicle itself, the powertrain, and the tires).
//
// The MAN Kat 1 truck family has been designed for offroad service.
// The development stems from the 60s, the begin of servce was ca. 1976
//
// The model data come from publicly available sources, fora of private Kat 1
// users and the book:
// P. Ocker: "MAN - Die Allrad-Alleskönner", Heel Verlag, 1999, ISBN 3-89365-705-3
//
// The 7t (load capacity) version has three driven rigid axles. The model is unloaded.
//
// =============================================================================

#ifndef MAN7T_H
#define MAN7T_H

#include <array>
#include <string>

#include "chrono_models/ChApiModels.h"
#include "chrono_models/vehicle/man/MAN_7t_Vehicle.h"
#include "chrono_models/vehicle/man/powertrain/MAN_7t_EngineSimpleMap.h"
#include "chrono_models/vehicle/man/powertrain/MAN_7t_AutomaticTransmissionSimpleMap.h"
#include "chrono_models/vehicle/man/powertrain/MAN_7t_EngineSimple.h"
#include "chrono_models/vehicle/man/powertrain/MAN_7t_AutomaticTransmissionSimple.h"
#include "chrono_models/vehicle/man/MAN_5t_TMeasyTire.h"
#include "chrono_models/vehicle/man/MAN_5t_TMsimpleTire.h"

namespace chrono {
namespace vehicle {
namespace man {

/// @addtogroup vehicle_models_man
/// @{

/// Wrapper class for modeling an entire MAN 7t vehicle assembly
/// (including the vehicle itself, the powertrain, and the tires).
class CH_MODELS_API MAN_7t {
  public:
    MAN_7t();
    MAN_7t(ChSystem* system);

    ~MAN_7t();

    void SetContactMethod(ChContactMethod val) { m_contactMethod = val; }
    void SetCollisionSystemType(ChCollisionSystem::Type collsys_type) { m_collsysType = collsys_type; }

    void SetChassisFixed(bool val) { m_fixed = val; }
    void SetChassisCollisionType(CollisionType val) { m_chassisCollisionType = val; }

    void SetDriveline6WD(bool val) { m_use_6WD_drivetrain = val; }
    void SetEngineType(EngineModelType val) { m_engineType = val; }
    void SetTransmissionType(TransmissionModelType val) { m_transmissionType = val; }
    void SetBrakeType(BrakeType brake_type) { m_brake_type = brake_type; }
    void SetTireType(TireModelType val) { m_tireType = val; }

    void SetInitPosition(const ChCoordsys<>& pos) { m_initPos = pos; }
    void SetInitFwdVel(double fwdVel) { m_initFwdVel = fwdVel; }
    void SetInitWheelAngVel(const std::vector<double>& omega) { m_initOmega = omega; }

    void SetTireStepSize(double step_size) { m_tire_step_size = step_size; }

    void EnableBrakeLocking(bool lock) { m_brake_locking = lock; }

    ChSystem* GetSystem() const { return m_vehicle->GetSystem(); }
    ChWheeledVehicle& GetVehicle() const { return *m_vehicle; }
    std::shared_ptr<ChChassis> GetChassis() const { return m_vehicle->GetChassis(); }
    std::shared_ptr<ChBodyAuxRef> GetChassisBody() const { return m_vehicle->GetChassisBody(); }

    void Initialize();

    void SetAerodynamicDrag(double Cd, double area, double air_density);

    void SetChassisVisualizationType(VisualizationType vis) { m_vehicle->SetChassisVisualizationType(vis); }
    void SetSuspensionVisualizationType(VisualizationType vis) { m_vehicle->SetSuspensionVisualizationType(vis); }
    void SetSteeringVisualizationType(VisualizationType vis) { m_vehicle->SetSteeringVisualizationType(vis); }
    void SetWheelVisualizationType(VisualizationType vis) { m_vehicle->SetWheelVisualizationType(vis); }
    void SetTireVisualizationType(VisualizationType vis) { m_vehicle->SetTireVisualizationType(vis); }

    void Synchronize(double time, const DriverInputs& driver_inputs, const ChTerrain& terrain);
    void Advance(double step);

    void LogHardpointLocations() { m_vehicle->LogHardpointLocations(); }
    void DebugLog(int what) { m_vehicle->DebugLog(what); }

  protected:
    ChContactMethod m_contactMethod;
    ChCollisionSystem::Type m_collsysType;
    CollisionType m_chassisCollisionType;
    bool m_fixed;
    bool m_brake_locking;

    EngineModelType m_engineType;
    TransmissionModelType m_transmissionType;
    BrakeType m_brake_type;
    TireModelType m_tireType;
    bool m_use_6WD_drivetrain;

    double m_tire_step_size;

    ChCoordsys<> m_initPos;
    double m_initFwdVel;
    std::vector<double> m_initOmega;

    bool m_apply_drag;
    double m_Cd;
    double m_area;
    double m_air_density;

    ChSystem* m_system;
    MAN_7t_Vehicle* m_vehicle;

    double m_tire_mass;
};

/// @} vehicle_models_man

}  // namespace man
}  // end namespace vehicle
}  // end namespace chrono

#endif
