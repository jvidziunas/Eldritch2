/*==================================================================*\
  DisplayBus.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/Display.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class DisplayBus {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DisplayBus instance.
		DisplayBus(DisplayMap<>& displayByName, Mutex& mutex);
		//!	Disable copy construction.
		DisplayBus(const DisplayBus&) = delete;

		~DisplayBus() = default;

		// ---------------------------------------------------

	public:
		bool TryAcquireDisplay(const Utf8Char* const name, DisplaySource& source);

		void ReleaseDisplay(const Utf8Char* const name, DisplaySource& source);

		// ---------------------------------------------------

	private:
		DisplayMap<>::Iterator CreateDisplay(const Utf8Char* name);

		// - DATA MEMBERS ------------------------------------

	private:
		DisplayMap<>* _displayByName;
		Mutex*        _displayMutex;
	};

}}} // namespace Eldritch2::Graphics::Vulkan
