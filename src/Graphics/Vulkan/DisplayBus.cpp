/*==================================================================*\
  DisplayBus.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DisplayBus.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	DisplayBus::DisplayBus(
		DisplayMap<>& displayByName,
		Mutex&        mutex) :
		_displayByName(eastl::addressof(displayByName)),
		_displayMutex(eastl::addressof(mutex)) {
	}

	// ---------------------------------------------------

	bool DisplayBus::TryAcquireDisplay(const Utf8Char* name, DisplaySource& source) {
		Lock _(*_displayMutex);

		DisplayMap<>::Iterator candidate(_displayByName->Find(name, Hash<decltype(name)>(), EqualTo<decltype(name)>()));

		if (candidate == _displayByName->End()) {
			candidate = CreateDisplay(name);
		}

		return candidate->second.TryAcquire(source);
	}

	// ---------------------------------------------------

	void DisplayBus::ReleaseDisplay(const Utf8Char* const name, DisplaySource& source) {
		Lock _(*_displayMutex);

		DisplayMap<>::Iterator candidate(_displayByName->Find(name, Hash<decltype(name)>(), EqualTo<decltype(name)>()));

		if (candidate == _displayByName->End()) {
			return;
		}

		candidate->second.Release(source);
	}

	// ---------------------------------------------------

	DisplayMap<>::Iterator DisplayBus::CreateDisplay(const Utf8Char* name) {
		//  It is assumed that this function is called within @ref TryAcquireDisplay()/@ref ReleaseDisplay() and as such we do not acquire a lock here.

		return _displayByName->Emplace(String<>(name, MallocAllocator("Display Name Allocator"))).first;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
