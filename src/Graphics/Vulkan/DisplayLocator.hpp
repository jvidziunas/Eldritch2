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

	class DisplayLocator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DisplayBus instance.
		DisplayLocator(DisplayMap<>& displayByName, Mutex& mutex);
		//!	Disable copy construction.
		DisplayLocator(const DisplayLocator&) = delete;

		~DisplayLocator() = default;

		// ---------------------------------------------------

	public:
		bool TryAcquireDisplay(StringView<Utf8Char> name, DisplaySource& source);

		void ReleaseDisplay(StringView<Utf8Char> name, DisplaySource& source);

		// ---------------------------------------------------

	private:
		DisplayMap<>::Iterator CreateDisplay(StringView<Utf8Char> name);

		// - DATA MEMBERS ------------------------------------

	private:
		DisplayMap<>* _displayByName;
		Mutex*        _displayMutex;
	};

}}} // namespace Eldritch2::Graphics::Vulkan
