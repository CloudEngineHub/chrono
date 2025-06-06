// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// Semi-trailing arm suspension constructed with data from file.
//
// =============================================================================

#ifndef SEMITRAILINGARM_H
#define SEMITRAILINGARM_H

#include "chrono_vehicle/ChApiVehicle.h"
#include "chrono_vehicle/wheeled_vehicle/suspension/ChSemiTrailingArm.h"

#include "chrono_thirdparty/rapidjson/document.h"

namespace chrono {
namespace vehicle {

/// @addtogroup vehicle_wheeled_suspension
/// @{

/// Semi-trailing arm suspension constructed with data from file.
class CH_VEHICLE_API SemiTrailingArm : public ChSemiTrailingArm {
  public:
    SemiTrailingArm(const std::string& filename);
    SemiTrailingArm(const rapidjson::Document& d);
    ~SemiTrailingArm();

    virtual double getCamberAngle() const override { return m_camber_angle; }
    virtual double getToeAngle() const override { return m_toe_angle; }

    virtual double getSpindleMass() const override { return m_spindleMass; }
    virtual double getArmMass() const override { return m_armMass; }

    virtual double getSpindleRadius() const override { return m_spindleRadius; }
    virtual double getSpindleWidth() const override { return m_spindleWidth; }
    virtual double getArmRadius() const override { return m_armRadius; }

    virtual const ChVector3d& getSpindleInertia() const override { return m_spindleInertia; }
    virtual const ChVector3d& getArmInertia() const override { return m_armInertia; }

    virtual double getAxleInertia() const override { return m_axleInertia; }

    virtual double getSpringRestLength() const override { return m_springRestLength; }
    virtual double getShockRestLength() const override { return m_shockRestLength; }
    virtual std::shared_ptr<ChLinkTSDA::ForceFunctor> getSpringForceFunctor() const override { return m_springForceCB; }
    virtual std::shared_ptr<ChLinkTSDA::ForceFunctor> getShockForceFunctor() const override { return m_shockForceCB; }

    virtual std::shared_ptr<ChJoint::BushingData> getCABushingData() const override { return m_armBushingData; }

  private:
    virtual const ChVector3d getLocation(PointId which) override { return m_points[which]; }

    virtual void Create(const rapidjson::Document& d) override;

    std::shared_ptr<ChLinkTSDA::ForceFunctor> m_springForceCB;
    std::shared_ptr<ChLinkTSDA::ForceFunctor> m_shockForceCB;

    ChVector3d m_points[NUM_POINTS];

    double m_camber_angle;
    double m_toe_angle;

    double m_spindleMass;
    double m_armMass;

    double m_spindleRadius;
    double m_spindleWidth;
    double m_armRadius;

    ChVector3d m_spindleInertia;
    ChVector3d m_armInertia;

    double m_axleInertia;

    double m_springRestLength;
    double m_shockRestLength;

    std::shared_ptr<ChJoint::BushingData> m_armBushingData;
};

/// @} vehicle_wheeled_suspension

}  // end namespace vehicle
}  // end namespace chrono

#endif
