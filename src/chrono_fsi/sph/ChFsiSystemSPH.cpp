// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2024 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Author: Radu Serban
// =============================================================================
//
// Implementation of FSI system using an SPH fluid solver.
//
// =============================================================================

////#define DEBUG_LOG

#include <iostream>

#include "chrono/utils/ChUtils.h"

#include "chrono_fsi/sph/ChFsiSystemSPH.h"
#include "chrono_fsi/sph/ChFsiInterfaceSPH.h"

namespace chrono {
namespace fsi {
namespace sph {

ChFsiSystemSPH::ChFsiSystemSPH(ChSystem& sysMBS, ChFsiFluidSystemSPH& sysSPH, bool use_generic_interface)
    : ChFsiSystem(sysMBS, sysSPH), m_sysSPH(sysSPH), m_generic_fsi_interface(use_generic_interface) {
    if (use_generic_interface) {
        std::cout << "Create an FSI system using a generic FSI interface" << std::endl;
        m_fsi_interface = chrono_types::make_shared<ChFsiInterfaceGeneric>(sysMBS, sysSPH);
    } else {
        std::cout << "Create an FSI system using a custom SPH FSI interface" << std::endl;
        m_fsi_interface = chrono_types::make_shared<ChFsiInterfaceSPH>(sysMBS, sysSPH);
    }

    // By default, use node directions for flexible meshes
    m_fsi_interface->EnableNodeDirections(true);
    ChDebugLog("default: use direction data");
}

ChFsiSystemSPH::~ChFsiSystemSPH() {}

ChFsiFluidSystemSPH& ChFsiSystemSPH::GetFluidSystemSPH() const {
    return m_sysSPH;
}

}  // end namespace sph
}  // end namespace fsi
}  // end namespace chrono
