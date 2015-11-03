/*==================================================================*\
  Tool.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	template <class ImplementingTool>
	ToolCRTPBase<ImplementingTool>::ToolCRTPBase( ::Eldritch2::Allocator& allocator ) : Detail::Tool( allocator ) {}

// ---------------------------------------------------

	template <class ImplementingTool>
	int	ToolCRTPBase<ImplementingTool>::Run( ::Eldritch2::Range<::Eldritch2::SystemChar**> /*arguments*/ ) {
		return static_cast<ImplementingTool*>(this)->ProcessInputFiles( ::Eldritch2::Range<const ::Eldritch2::SystemChar**>( _inputFiles.Begin(), _inputFiles.End() ) );
	}

}	// namespace Tools
}	// namespace Eldritch2

