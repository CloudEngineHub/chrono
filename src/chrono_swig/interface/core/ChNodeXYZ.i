#ifdef SWIGCSHARP  // --------------------------------------------------------------------- CSHARP

// MULTIPLE INHERITANCE WORKAROUND

// (B) Methods of a base class that SWIG discards that *are* overriden in ChNodeXYZ

// Ensure that these functions are not marked as 'overrides' in the generated C# code.

%csmethodmodifiers chrono::ChNodeXYZ::Variables "public"
%csmethodmodifiers chrono::ChNodeXYZ::GetLoadableNumCoordsPosLevel "public"
%csmethodmodifiers chrono::ChNodeXYZ::GetLoadableNumCoordsVelLevel "public"
%csmethodmodifiers chrono::ChNodeXYZ::LoadableGetVariables "public"
%csmethodmodifiers chrono::ChNodeXYZ::LoadableStateIncrement "public"
%csmethodmodifiers chrono::ChNodeXYZ::LoadableGetStateBlockPosLevel "public"
%csmethodmodifiers chrono::ChNodeXYZ::LoadableGetStateBlockVelLevel "public"
%csmethodmodifiers chrono::ChNodeXYZ::ComputeNF "public"
%csmethodmodifiers chrono::ChNodeXYZ::GetDensity "public"
%csmethodmodifiers chrono::ChNodeXYZ::GetNumFieldCoords "public"
%csmethodmodifiers chrono::ChNodeXYZ::GetSubBlockOffset "public"
%csmethodmodifiers chrono::ChNodeXYZ::GetSubBlockSize "public"
%csmethodmodifiers chrono::ChNodeXYZ::GetNumSubBlocks "public"
%csmethodmodifiers chrono::ChNodeXYZ::IsSubBlockActive "public"

//// RADU:  Do we actually want to wrap methods of ChLoadable?
////        If not, we should probably just use %ignore

#endif             // --------------------------------------------------------------------- CSHARP

// Include the C++ header(s)
%{
#include "chrono/physics/ChNodeXYZ.h"
%}
 
%shared_ptr(chrono::ChNodeXYZ)

// Parse the header file to generate wrappers
%include "../../../chrono/physics/ChNodeXYZ.h"  
