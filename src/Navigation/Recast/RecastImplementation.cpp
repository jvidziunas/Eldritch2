/*==================================================================*\
  RecastImplementation.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
ET_PUSH_MSVC_WARNING_STATE(disable : 6334 6385 6011)
#include <recastnavigation/DetourCrowd/Source/DetourObstacleAvoidance.cpp>
#include <recastnavigation/DetourCrowd/Source/DetourLocalBoundary.cpp>
#include <recastnavigation/DetourCrowd/Source/DetourProximityGrid.cpp>
#include <recastnavigation/DetourCrowd/Source/DetourPathCorridor.cpp>
#include <recastnavigation/DetourCrowd/Source/DetourPathQueue.cpp>
#include <recastnavigation/DetourCrowd/Source/DetourCrowd.cpp>
#include <recastnavigation/Detour/Source/DetourNavMeshBuilder.cpp>
#include <recastnavigation/Detour/Source/DetourNavMeshQuery.cpp>
#include <recastnavigation/Detour/Source/DetourNavMesh.cpp>
#include <recastnavigation/Detour/Source/DetourAssert.cpp>
#include <recastnavigation/Detour/Source/DetourCommon.cpp>
#include <recastnavigation/Detour/Source/DetourAlloc.cpp>
#include <recastnavigation/Detour/Source/DetourNode.cpp>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//
