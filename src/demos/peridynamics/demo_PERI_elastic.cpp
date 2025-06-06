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
// Authors: Alessandro Tasora
// =============================================================================
//
// Demo code explaining how to create a deformable solid using peridynamics. 
// Peridynamics allow an efficient handling of fracturing and cracks, working as
// a meshless framework: differently from conventional FEA it does not need 
// that the solid is preprocessed as a tetrahedral mesh. In the peridynamics 
// context, the solid is made with a node cloud, where nodes are connected
// each other (if withing a small distance called 'horizon') by microscopic bonds. 
//
// =============================================================================

#include "chrono/physics/ChSystemNSC.h"
#include "chrono/core/ChRealtimeStep.h"
#include "chrono/physics/ChBodyEasy.h"
#include "chrono/fea/ChLinkNodeFrame.h"

#include "chrono_peridynamics/ChMatterPeriSprings.h"
#include "chrono_peridynamics/ChMatterPeriBB.h"
#include "chrono_peridynamics/ChMatterPeriBBimplicit.h"
#include "chrono_peridynamics/ChMatterPeriLinearElastic.h"
#include "chrono_peridynamics/ChMatterPeriLiquid.h"
#include "chrono_peridynamics/ChPeridynamics.h"

#include "chrono_irrlicht/ChVisualSystemIrrlicht.h"
#include "chrono_irrlicht/ChIrrTools.h"

#include "chrono_postprocess/ChBlender.h"


// Use the namespaces of Chrono

using namespace chrono;
using namespace peridynamics;
using namespace postprocess;

// Use the main namespaces of Irrlicht
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace chrono::irrlicht;

int main(int argc, char* argv[]) {
    std::cout << "Copyright (c) 2017 projectchrono.org\nChrono version: " << CHRONO_VERSION << std::endl;

    // Create a ChronoENGINE physical system
    ChSystemNSC mphysicalSystem;

    // Set small collision envelopes for objects that will be created from now on
    ChCollisionModel::SetDefaultSuggestedEnvelope(0.0002);
    ChCollisionModel::SetDefaultSuggestedMargin(0.0002);
    mphysicalSystem.SetCollisionSystemType(ChCollisionSystem::Type::BULLET);


    // CREATE A FLOOR
    auto mat = chrono_types::make_shared<ChContactMaterialNSC>();
    mat->SetFriction(0.2f);

    auto mfloorBody = chrono_types::make_shared<ChBodyEasyBox>(20,1,20,1000,true,true,mat);
    mphysicalSystem.Add(mfloorBody);
    mfloorBody->SetFixed(true);
    mfloorBody->SetPos(ChVector3d(0, -7.5, 0));

    mfloorBody->GetVisualShape(0)->SetColor(ChColor(0.2f, 0.2f, 0.2f));


    
    // CREATE A SPHERE PRESSING THE MATERIAL:
    auto msphere = chrono_types::make_shared<ChBodyEasySphere>(0.7, 7000, true,true, mat);
    mphysicalSystem.Add(msphere);
    msphere->SetPos(ChVector3d(0, -1.5, 0));
    msphere->SetPosDt(ChVector3d(0, -5.5, 0));
    
   

    // CREATE THE PERIDYNAMIC CONTINUUM

    // Create peridynamics material 
    // This is a very simple one: a linear bond-based elastic material, defined
    // via the bulk elasticity modulus. The Poisson ratio is fixed to 1/4. 
    auto my_perimaterial = chrono_types::make_shared<ChMatterPeriBB>();
    my_perimaterial->SetYoungModulus(45e6); // Young stiffness (unit N/m^2)
    my_perimaterial->damping = 0.0001;      // damping as Rayleigh beta

    /*
    // Alternatively, use the following, that supports user defined Poisson values (but 
    // at the cost of slower performance because it is a state-based material).
    auto my_perimaterial = chrono_types::make_shared<ChMatterPeriLinearElastic>();
    my_perimaterial->SetYoungModulusPoisson(30e6, 0.25);    // Young stiffness (unit N/m^2) and Poisson coeff.
    */

    // IMPORTANT!
    // This contains all the peridynamics particles and their materials. 
    auto my_peridynamics = chrono_types::make_shared<ChPeridynamics>();
    mphysicalSystem.Add(my_peridynamics);

    my_peridynamics->AddMatter(my_perimaterial);

    // Use the FillBox easy way to create the set of nodes in the Peridynamics matter
    my_peridynamics->FillBox(
        my_perimaterial,
        ChVector3d(3, 1.5, 3),                        // size of box
        4.0 / 15.0,                                   // resolution step
        1000,                                         // initial density
        ChCoordsys<>(ChVector3d(0, -3.4, 0), QUNIT),  // position & rotation of box
        1.7,                                          // set the horizon radius (as multiples of step) 
        0.4);                                         // set the collision radius (as multiples of step) for interface particles

    // Just for testing, fix some nodes
    
    for (const auto& node : my_peridynamics->GetNodes()) {
        if (node->GetPos().x() < -1.3)
            node->SetFixed(true);
    }
    

    // Attach visualization to peridynamics. The realtime visualization will show 
    // nodes and bonds with dots, lines etc. Suggestion: use the Blender importer add-on 
    // for rendering properties in falsecolor and other advanced features.
    
    auto mglyphs_nodes = chrono_types::make_shared<ChVisualPeriBB>(my_perimaterial);
    my_peridynamics->AddVisualShape(mglyphs_nodes);
    mglyphs_nodes->SetGlyphsSize(0.04);
    mglyphs_nodes->AttachVelocity(0, 20, "Vel"); // postprocessing tools can exploit this. Also suggest a min-max for falsecolor rendering.
    
    
    auto mglyphs_bonds = chrono_types::make_shared<ChVisualPeriBBBonds>(my_perimaterial);
    mglyphs_bonds->draw_unbroken = true;
    mglyphs_bonds->draw_broken = false;
    my_peridynamics->AddVisualShape(mglyphs_bonds);
    

    // -----Blender postprocess, optional

    // Create an exporter to Blender
    ChBlender blender_exporter = ChBlender(&mphysicalSystem);

    // Set the path where it will save all files, a directory will be created if not existing
    blender_exporter.SetBasePath(GetChronoOutputPath() + "BLENDER_PERI");

    // Export all existing visual shapes to POV-Ray
    blender_exporter.AddAll();

    blender_exporter.SetCamera(ChVector3d(3, 4, -5), ChVector3d(0, 0.5, 0), 50);  // pos, aim, angle

    blender_exporter.ExportScript();

    // --------------------
    

    // Create the Irrlicht visualization system
    auto vsys = chrono_types::make_shared<ChVisualSystemIrrlicht>();
    vsys->AttachSystem(&mphysicalSystem);
    vsys->SetWindowSize(1024, 768);
    vsys->SetWindowTitle("Peridynamics test");
    vsys->Initialize();
    vsys->AddLogo();
    vsys->AddSkyBox();
    vsys->AddCamera(ChVector3d(-3, -1, 1.3), ChVector3d(0, -4, 0));
    vsys->AddLight(ChVector3d(30, 30, 60), 120, ChColor(0.6f, 0.6f, 0.6f));
    vsys->AddLight(ChVector3d(40, 60, 30), 120, ChColor(0.6f, 0.6f, 0.6f));


    // Modify some setting of the physical system for the simulation, if you want
    if (mphysicalSystem.GetSolver()->IsIterative()) {
        mphysicalSystem.GetSolver()->AsIterative()->SetMaxIterations(25);
    }


    // IMPORTANT call this to generate bonds between nodes!
    ChPeridynamics::SetupInitialBonds(&mphysicalSystem, my_peridynamics);


    //
    // THE SOFT-REAL-TIME CYCLE
    //

    // Set timestep, that is smaller and smaller as stiffness of material increases 
    // and or mesh spacing decreases.
    double timestep = 0.0005;

    while (vsys->Run()) {
        vsys->BeginScene();

        vsys->Render();
        
        vsys->EndScene();
        
        mphysicalSystem.DoStepDynamics(timestep);
        
        if (mphysicalSystem.GetNumSteps() % 20 == 0)
            blender_exporter.ExportData();
    }


    return 0;
}
