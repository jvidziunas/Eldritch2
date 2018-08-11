/*==================================================================*\
  XAudio2WorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2WorldComponent.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Core;

	XAudio2WorldComponent::XAudio2WorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_allocator("XAudio2 World Service Root Allocator"),
		_log(FindService<World>().GetLog()),
		_scene(nullptr) {
	}

}}} // namespace Eldritch2::Audio::XAudio2
