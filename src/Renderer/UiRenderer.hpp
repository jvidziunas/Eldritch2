/*==================================================================*\
  UiRenderer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ArraySet.hpp>
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	class ETPureAbstractHint UiRenderer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UiRenderer instance.
		UiRenderer( const UiRenderer& ) = default;
	//!	Constructs this @ref UiRenderer instance.
		UiRenderer() = default;

		~UiRenderer() = default;

	// ---------------------------------------------------

	private:
		// Eldritch2::ArraySet<>
	};

}	// namespace Renderer
}	// namespace Eldritch2