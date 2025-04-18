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
// Authors: Alessandro Tasora, Radu Serban
// =============================================================================

#include "chrono/physics/ChShaft.h"
#include "chrono/physics/ChShaftsPlanetary.h"
#include "chrono/physics/ChSystem.h"

namespace chrono {

// Register into the object factory, to enable run-time dynamic creation and persistence
CH_FACTORY_REGISTER(ChShaftsPlanetary)

ChShaftsPlanetary::ChShaftsPlanetary()
    : r1(1),
      r2(1),
      r3(1),
      torque_react(0),
      shaft1(NULL),
      shaft2(NULL),
      shaft3(NULL),
      avoid_phase_drift(true),
      phase1(0),
      phase2(0),
      phase3(0),
      active(true) {}

ChShaftsPlanetary::ChShaftsPlanetary(const ChShaftsPlanetary& other) : ChPhysicsItem(other), active(true) {
    r1 = other.r1;
    r2 = other.r2;
    r3 = other.r3;

    torque_react = other.torque_react;
    shaft1 = NULL;
    shaft2 = NULL;
    shaft3 = NULL;

    avoid_phase_drift = other.avoid_phase_drift;
    phase1 = other.phase1;
    phase2 = other.phase2;
    phase3 = other.phase3;
}

bool ChShaftsPlanetary::Initialize(std::shared_ptr<ChShaft> shaft_1,  // first shaft to join (carrier wheel)
                                   std::shared_ptr<ChShaft> shaft_2,  // second shaft to join (wheel)
                                   std::shared_ptr<ChShaft> shaft_3   // third shaft to join (wheel)
) {
    shaft1 = shaft_1.get();
    shaft2 = shaft_2.get();
    shaft3 = shaft_3.get();

    assert(shaft1 && shaft2 && shaft3);
    assert(shaft1 != shaft2);
    assert(shaft1 != shaft3);
    assert(shaft3 != shaft2);
    assert((shaft1->GetSystem() == shaft2->GetSystem()) && (shaft1->GetSystem() == shaft3->GetSystem()));

    phase1 = shaft1->GetPos();
    phase2 = shaft2->GetPos();
    phase3 = shaft3->GetPos();

    constraint.SetVariables(&shaft_1->Variables(), &shaft_2->Variables(), &shaft_3->Variables());

    SetSystem(shaft1->GetSystem());

    return true;
}

void ChShaftsPlanetary::Update(double time, bool update_assets) {
    // Inherit time changes of parent class
    ChPhysicsItem::Update(time, update_assets);

    // update class data
    // ...
}

void ChShaftsPlanetary::IntLoadResidual_CqL(const unsigned int off_L,    // offset in L multipliers
                                            ChVectorDynamic<>& R,        // result: the R residual, R += c*Cq'*L
                                            const ChVectorDynamic<>& L,  // the L vector
                                            const double c               // a scaling factor
) {
    if (!active)
        return;

    constraint.AddJacobianTransposedTimesScalarInto(R, L(off_L) * c);
}

void ChShaftsPlanetary::IntLoadConstraint_C(const unsigned int off_L,  // offset in Qc residual
                                            ChVectorDynamic<>& Qc,     // result: the Qc residual, Qc += c*C
                                            const double c,            // a scaling factor
                                            bool do_clamp,             // apply clamping to c*C?
                                            double recovery_clamp      // value for min/max clamping of c*C
) {
    if (!active)
        return;

    double res = this->r1 * (this->shaft1->GetPos() - this->phase1) +
                 this->r2 * (this->shaft2->GetPos() - this->phase2) +
                 this->r3 * (this->shaft3->GetPos() - this->phase3);
    if (!this->avoid_phase_drift)
        res = 0;

    double cnstr_violation = c * res;

    if (do_clamp) {
        cnstr_violation = std::min(std::max(cnstr_violation, -recovery_clamp), recovery_clamp);
    }

    Qc(off_L) += cnstr_violation;
}

void ChShaftsPlanetary::IntToDescriptor(const unsigned int off_v,  // offset in v, R
                                        const ChStateDelta& v,
                                        const ChVectorDynamic<>& R,
                                        const unsigned int off_L,  // offset in L, Qc
                                        const ChVectorDynamic<>& L,
                                        const ChVectorDynamic<>& Qc) {
    if (!active)
        return;

    constraint.SetLagrangeMultiplier(L(off_L));
    constraint.SetRightHandSide(Qc(off_L));
}

void ChShaftsPlanetary::IntFromDescriptor(const unsigned int off_v,  // offset in v
                                          ChStateDelta& v,
                                          const unsigned int off_L,  // offset in L
                                          ChVectorDynamic<>& L) {
    if (!active)
        return;

    L(off_L) = constraint.GetLagrangeMultiplier();
}

void ChShaftsPlanetary::IntStateGatherReactions(const unsigned int off_L, ChVectorDynamic<>& L) {
    L(off_L) = torque_react;
}

void ChShaftsPlanetary::IntStateScatterReactions(const unsigned int off_L, const ChVectorDynamic<>& L) {
    torque_react = L(off_L);
}

void ChShaftsPlanetary::InjectConstraints(ChSystemDescriptor& descriptor) {
    if (!active)
        return;

    descriptor.InsertConstraint(&constraint);
}

void ChShaftsPlanetary::ConstraintsBiReset() {
    constraint.SetRightHandSide(0.);
}

void ChShaftsPlanetary::ConstraintsBiLoad_C(double factor, double recovery_clamp, bool do_clamp) {
    if (!active)
        return;

    double res = 0;  // no residual

    constraint.SetRightHandSide(constraint.GetRightHandSide() + factor * res);
}

void ChShaftsPlanetary::ConstraintsBiLoad_Ct(double factor) {
    // nothing
}

void ChShaftsPlanetary::LoadConstraintJacobians() {
    // compute jacobians
    constraint.Get_Cq_a()(0) = r1;
    constraint.Get_Cq_b()(0) = r2;
    constraint.Get_Cq_c()(0) = r3;
}

void ChShaftsPlanetary::ConstraintsFetch_react(double factor) {
    // From constraints to react vector:
    torque_react = constraint.GetLagrangeMultiplier() * factor;
}

void ChShaftsPlanetary::ArchiveOut(ChArchiveOut& archive_out) {
    // version number
    archive_out.VersionWrite<ChShaftsPlanetary>();

    // serialize parent class
    ChPhysicsItem::ArchiveOut(archive_out);

    // serialize all member data:
    archive_out << CHNVP(r1);
    archive_out << CHNVP(r2);
    archive_out << CHNVP(r3);
    archive_out << CHNVP(avoid_phase_drift);
    archive_out << CHNVP(phase1);
    archive_out << CHNVP(phase2);
    archive_out << CHNVP(phase3);
    archive_out << CHNVP(shaft1);  //// TODO  serialize with shared ptr
    archive_out << CHNVP(shaft2);  //// TODO  serialize with shared ptr
    archive_out << CHNVP(shaft3);  //// TODO  serialize with shared ptr
}

void ChShaftsPlanetary::ArchiveIn(ChArchiveIn& archive_in) {
    // version number
    /*int version =*/archive_in.VersionRead<ChShaftsPlanetary>();

    // deserialize parent class:
    ChPhysicsItem::ArchiveIn(archive_in);

    // deserialize all member data:
    archive_in >> CHNVP(r1);
    archive_in >> CHNVP(r2);
    archive_in >> CHNVP(r3);
    archive_in >> CHNVP(avoid_phase_drift);
    archive_in >> CHNVP(phase1);
    archive_in >> CHNVP(phase2);
    archive_in >> CHNVP(phase3);
    archive_in >> CHNVP(shaft1);  //// TODO  serialize with shared ptr
    archive_in >> CHNVP(shaft2);  //// TODO  serialize with shared ptr
    archive_in >> CHNVP(shaft3);  //// TODO  serialize with shared ptr

    constraint.SetVariables(&shaft1->Variables(), &shaft2->Variables(), &shaft3->Variables());
}

}  // end namespace chrono
