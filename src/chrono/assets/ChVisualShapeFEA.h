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

#ifndef CH_VISUAL_SHAPE_FEA_H
#define CH_VISUAL_SHAPE_FEA_H

#include "chrono/assets/ChColor.h"
#include "chrono/assets/ChColormap.h"
#include "chrono/assets/ChGlyphs.h"
#include "chrono/assets/ChVisualShapeTriangleMesh.h"

namespace chrono {

/// @addtogroup chrono_assets
/// @{

// Forward declarations
namespace fea {
class ChMesh;
class ChMeshSurface;
class ChContactSurface;
class ChNodeFEAxyz;
class ChNodeFEAxyzP;
class ChElementBase;
}  // namespace fea

/// FEA mesh visualization.
/// Adds to the containing visual model a trimesh and a glyphs visual shapes.
class ChApi ChVisualShapeFEA {
  public:
    /// Visualization mesh data type.
    enum class DataType {
        NONE,
        SURFACE,
        CONTACTSURFACES,
        LOADSURFACES,
        NODE_DISP_NORM,
        NODE_DISP_X,
        NODE_DISP_Y,
        NODE_DISP_Z,
        NODE_SPEED_NORM,
        NODE_SPEED_X,
        NODE_SPEED_Y,
        NODE_SPEED_Z,
        NODE_ACCEL_NORM,
        NODE_ACCEL_X,
        NODE_ACCEL_Y,
        NODE_ACCEL_Z,
        ELEM_STRAIN_VONMISES,
        ELEM_STRESS_VONMISES,
        ELEM_STRAIN_HYDROSTATIC,
        ELEM_STRESS_HYDROSTATIC,
        ELEM_BEAM_MX,
        ELEM_BEAM_MY,
        ELEM_BEAM_MZ,
        ELEM_BEAM_TX,
        ELEM_BEAM_TY,
        ELEM_BEAM_TZ,
        NODE_FIELD_VALUE,
        ANCF_BEAM_AX,
        ANCF_BEAM_BD
    };

    /// Visualization glyph data type.
    enum class GlyphType {
        NONE,
        NODE_DOT_POS,
        NODE_CSYS,
        NODE_VECT_SPEED,
        NODE_VECT_ACCEL,
        ELEM_TENS_STRAIN,
        ELEM_TENS_STRESS,
        ELEM_VECT_DP  // gradient field for Poisson problems
    };

  public:
    ChVisualShapeFEA();
    ~ChVisualShapeFEA() {}

    /// Set the current data type to be plotted (speeds, forces, etc.).
    void SetFEMdataType(DataType type) { fem_data_type = type; }

    /// Set the current data type to be drawn with glyphs.
    void SetFEMglyphType(GlyphType type) { fem_glyph = type; }

    /// Set color for SURFACE mode (also for wireframe lines).
    void SetDefaultMeshColor(ChColor mcolor) { meshcolor = mcolor; }

    /// Set color for GlyphType::NONE mode or for wireframe lines.
    void SetDefaultSymbolsColor(ChColor mcolor) { symbolscolor = mcolor; }

    /// Set min and max values for indexing in the colormap.
    void SetColormapRange(double min, double max);

    /// Set min and max values for indexing in the colormap.
    void SetColormapRange(const ChVector2d& range);

    /// Set the colormap type for false coloring of the SCM mesh.
    /// The default colormap is JET (a divergent blue-red map).
    void SetColormap(ChColormap::Type type);

    /// Set the scale for drawing of symbols (e.g., vectors).
    void SetSymbolsScale(double scale) { symbols_scale = scale; }

    /// Set the thickness for drawing of symbols (e.g., vectors).
    void SetSymbolsThickness(double thickness) { symbols_thickness = thickness; }

    /// Set the resolution of beam triangulated drawing, along the beam direction (default: 8).
    void SetBeamResolution(int resolution) { beam_resolution = resolution; }

    /// Set the resolution of beam triangulated drawing, along the section (default: 10).
    /// For example, in case of a circular section, this is the number of points along the circle.
    void SetBeamResolutionSection(int resolution) { beam_resolution_section = resolution; }

    /// Set the resolution of shell triangulated drawing (default: 2).
    /// This value represents the number of visualization mesh vertices on each FEM element edge.
    /// The default value of 2 results in a visualization mesh constructed using only the FEM nodes.
    void SetShellResolution(int resolution) { shell_resolution = resolution; }

    /// Set shrinkage of elements during drawing.
    void SetShrinkElements(bool shrink, double factor);

    /// Set backface cull speedup (default false).
    /// Must be set true for shells and in general where already double-sided twin triangles are used.
    void SetBackfaceCull(bool cull) { backface_cull = cull; }

    /// Set the Z buffer enable/disable, for visualization systems that support it (default: true).
    /// If hide = false, symbols will appear even if hidden by meshes/geometries.
    void SetZbufferHide(bool hide) { zbuffer_hide = hide; }

    /// Activate Gourad or Phong smoothing for faces of non-straight elements.
    /// Note: experimental feature.
    void SetSmoothFaces(bool smooth) { smooth_faces = smooth; }

    /// Draw the mesh in its underformed (reference) configuration.
    void SetDrawInUndeformedReference(bool undeformed) { undeformed_reference = undeformed; }

    /// Set as wireframe visualization.
    void SetWireframe(bool use_wireframe) { wireframe = use_wireframe; }

    /// Return the current data type to be plotted (speeds, forces, etc.).
    DataType GetFEMdataType() { return fem_data_type; }

    /// Returns the current data type to be drawn with glyphs.
    GlyphType GetFEMglyphType() { return fem_glyph; }

    /// Get the type of the colormap currently in use.
    ChColormap::Type GetColormapType() const;

    /// Get the colormap object in current use.
    const ChColormap& GetColormap() const;

    /// Return the scale for drawing of symbols.
    double GetSymbolsScale() const { return symbols_scale; }

    /// Return the thickness for drawing of symbols.
    double GetSymbolsThickness() const { return symbols_thickness; }

    /// Return the resolution of beam triangulated drawing, along the beam direction.
    int GetBeamResolution() const { return beam_resolution; }

    /// Return the resolution of beam triangulated drawing, along the section.
    int GetBeamResolutionSection() const { return beam_resolution_section; }

    /// Return the resolution of shell triangulated drawing.
    int GetShellResolution() const { return shell_resolution; }

    /// Update the triangle visualization mesh so that it matches with the FEM mesh.
    void Update(ChObj* updater, const ChFrame<>& frame);

  private:
    double ComputeScalarOutput(std::shared_ptr<fea::ChNodeFEAxyz> node,
                               std::shared_ptr<fea::ChElementBase> element) const;
    double ComputeScalarOutput(std::shared_ptr<fea::ChNodeFEAxyzP> node,
                               std::shared_ptr<fea::ChElementBase> element) const;
    ChColor ComputeFalseColor(double value) const;

    // Helper functions for updating buffers of specific element types

    void UpdateBuffers_Tetrahedron(std::shared_ptr<fea::ChElementBase> element,
                                   ChTriangleMeshConnected& trianglemesh,
                                   unsigned int& i_verts,
                                   unsigned int& i_vnorms,
                                   unsigned int& i_vcols,
                                   unsigned int& i_triindex,
                                   bool& need_automatic_smoothing);
    void UpdateBuffers_Tetra_4_P(std::shared_ptr<fea::ChElementBase> element,
                                 ChTriangleMeshConnected& trianglemesh,
                                 unsigned int& i_verts,
                                 unsigned int& i_vnorms,
                                 unsigned int& i_vcols,
                                 unsigned int& i_triindex,
                                 bool& need_automatic_smoothing);
    void UpdateBuffers_Hex(std::shared_ptr<fea::ChElementBase> element,
                           ChTriangleMeshConnected& trianglemesh,
                           unsigned int& i_verts,
                           unsigned int& i_vnorms,
                           unsigned int& i_vcols,
                           unsigned int& i_triindex,
                           bool& need_automatic_smoothing);
    void UpdateBuffers_Beam(std::shared_ptr<fea::ChElementBase> element,
                            ChTriangleMeshConnected& trianglemesh,
                            unsigned int& i_verts,
                            unsigned int& i_vnorms,
                            unsigned int& i_vcols,
                            unsigned int& i_triindex,
                            bool& need_automatic_smoothing);
    void UpdateBuffers_Shell(std::shared_ptr<fea::ChElementBase> element,
                             ChTriangleMeshConnected& trianglemesh,
                             unsigned int& i_verts,
                             unsigned int& i_vnorms,
                             unsigned int& i_vcols,
                             unsigned int& i_triindex,
                             bool& need_automatic_smoothing);
    void UpdateBuffers_LoadSurface(std::shared_ptr<fea::ChMeshSurface> surface,
                                   ChTriangleMeshConnected& trianglemesh,
                                   unsigned int& i_verts,
                                   unsigned int& i_vnorms,
                                   unsigned int& i_vcols,
                                   unsigned int& i_triindex,
                                   bool& need_automatic_smoothing);
    void UpdateBuffers_ContactSurfaceMesh(std::shared_ptr<fea::ChContactSurface> surface,
                                          ChTriangleMeshConnected& trianglemesh,
                                          unsigned int& i_verts,
                                          unsigned int& i_vnorms,
                                          unsigned int& i_vcols,
                                          unsigned int& i_triindex,
                                          bool& need_automatic_smoothing);

    ChObj* obj;

    DataType fem_data_type;
    GlyphType fem_glyph;

    double colorscale_min;
    double colorscale_max;

    double symbols_scale;
    double symbols_thickness;

    bool shrink_elements;
    double shrink_factor;

    bool wireframe;
    bool backface_cull;

    bool zbuffer_hide;

    bool smooth_faces;

    bool undeformed_reference;

    int beam_resolution;
    int beam_resolution_section;
    int shell_resolution;

    ChColor meshcolor;
    ChColor symbolscolor;

    std::shared_ptr<ChVisualShapeTriangleMesh> m_trimesh_shape;
    std::shared_ptr<ChGlyphs> m_glyphs_shape;

    std::vector<int> normal_accumulators;

    std::unique_ptr<ChColormap> m_colormap;  ///< colormap for mesh false coloring
    ChColormap::Type m_colormap_type;        ///< colormap type

    friend class ChVisualModel;
    friend class ChObj;
};

/// @} chrono_assets

}  // end namespace chrono

#endif
