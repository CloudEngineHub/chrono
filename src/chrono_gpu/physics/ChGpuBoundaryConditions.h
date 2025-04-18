// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2019 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Conlain Kelly, Nic Olsen, Dan Negrut
// =============================================================================

#pragma once

#include "chrono_gpu/ChApiGpu.h"
#include "chrono_gpu/ChGpuDefines.h"

namespace chrono {
namespace gpu {

/// Sphere
template <typename T, typename T3>
struct Sphere_BC_params_t {
    T3 sphere_center;
    float3 sphere_velo;
    float3 sphere_angularVelo;
    float3 reaction_torques;
    float radius;
    T normal_sign;
    float mass;
};

/// Z-aligned cone pointed downward
template <typename T, typename T3>
struct Z_Cone_BC_params_t {
    T3 cone_tip;
    float slope;
    T hmax;
    T hmin;
    T normal_sign;
};

/// Infinite Plane defined by point in plane and normal
template <typename T3>
struct Plane_BC_params_t {
    float3 normal;
    T3 position;
    T3 rotation_center;
    float3 angular_acc;
};

/// Customized finite Plate defined by center of the plate, normal and y dim
template <typename T3>
struct Plate_BC_params_t {
    float3 normal;
    T3 plate_center;
    float h_dim_y;
};

/// Infinite Z-aligned cylinder
template <typename T, typename T3>
struct Z_Cylinder_BC_params_t {
    T3 center;
    T radius;
    T normal_sign;
};

/// big enum to handle all possible boundary conditions
template <typename T, typename T3>
struct BC_params_t {
    /// Is this boundary condition active?
    bool active;
    /// Is the boundary condition fixed in space?
    bool fixed;
    /// Whether or not to track reaction forces
    bool track_forces;
    float3 reaction_forces;
    /// velocity in SU if the motion is being prescribed
    float3 vel_SU;
    union {
        Sphere_BC_params_t<T, T3> sphere_params;
        Z_Cone_BC_params_t<T, T3> cone_params;
        Plate_BC_params_t<T3> plate_params;
        Plane_BC_params_t<T3> plane_params;        // plane only needs one template arg
        Z_Cylinder_BC_params_t<T, T3> cyl_params;  // plane only needs one arg
    };
};

enum BC_type { SPHERE, CONE, PLANE, PLATE, CYLINDER };
}  // namespace gpu
}  // namespace chrono
