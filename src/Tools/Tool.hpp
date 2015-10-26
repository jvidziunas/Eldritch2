/*==================================================================*\
  Tool.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	class Tool {

	};

	template <typename Tool, class Allocator>
	int	ExecuteTool( Allocator&& allocator, int argc, SystemChar** argv );

}	// namespace Tools
}	// namespace Eldritch2

#include <Tools/Tool.inl>