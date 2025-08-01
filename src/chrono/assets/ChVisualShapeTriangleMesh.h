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
// Authors: Alesandro Tasora, Radu Serban
// =============================================================================

#ifndef CH_VISUAL_SHAPE_TRIANGLE_MESH_H
#define CH_VISUAL_SHAPE_TRIANGLE_MESH_H

#include <vector>

#include "chrono/assets/ChVisualShape.h"
#include "chrono/geometry/ChTriangleMeshConnected.h"

namespace chrono {

/// @addtogroup chrono_assets
/// @{

/// Class for referencing a triangle mesh shape that can be visualized in some way.
/// A ChVisualShapeTriangleMesh can be attached to a physics object.
/// Provides various rendering options (e.g., drawing as wireframe, performing backface culling, etc.) which may not be
/// supported by a particular visualization system.
class ChApi ChVisualShapeTriangleMesh : public ChVisualShape {
  public:
    ChVisualShapeTriangleMesh();
    ChVisualShapeTriangleMesh(std::shared_ptr<ChTriangleMeshConnected> mesh, bool load_materials = true);
    ~ChVisualShapeTriangleMesh() {}

    std::shared_ptr<ChTriangleMeshConnected> GetMesh() { return trimesh; }

    /// Associate the mesh asset with a triangle mesh geometry.
    /// Optionally, if `load_materials` is set to `true` and if the provided trimesh was loaded from a Wavefront OBJ
    /// file, associated material files are searched for and visualization materials loaded.
    void SetMesh(std::shared_ptr<ChTriangleMeshConnected> mesh, bool load_materials = true);

    bool IsWireframe() const { return wireframe; }
    void SetWireframe(bool mw) { wireframe = mw; }

    bool IsBackfaceCull() const { return backface_cull; }
    void SetBackfaceCull(bool mbc) { backface_cull = mbc; }

    const std::string& GetName() const { return name; }
    void SetName(const std::string& mname) { name = mname; }

    const ChVector3d& GetScale() const { return scale; }
    void SetScale(const ChVector3d& mscale) { scale = mscale; }

    void SetFixedConnectivity() { fixed_connectivity = true; }
    bool FixedConnectivity() const { return fixed_connectivity; }
    void SetModifiedVertices(std::vector<int> vertices) { modified_vertices = vertices; }
    const std::vector<int>& GetModifiedVertices() const { return modified_vertices; }

    /// Get the shape bounding box.
    virtual ChAABB GetBoundingBox() const override { return trimesh->GetBoundingBox(); }

    /// Method to allow serialization of transient data to archives.
    virtual void ArchiveOut(ChArchiveOut& archive_out) override;

    /// Method to allow de-serialization of transient data from archives.
    virtual void ArchiveIn(ChArchiveIn& archive_in) override;

  private:
    std::shared_ptr<ChTriangleMeshConnected> trimesh;

    bool wireframe;
    bool backface_cull;

    std::string name;
    ChVector3d scale;

    bool fixed_connectivity;
    std::vector<int> modified_vertices;
};

/// @} chrono_assets

CH_CLASS_VERSION(ChVisualShapeTriangleMesh, 0)

}  // end namespace chrono

#endif
