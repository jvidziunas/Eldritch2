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
	class	Allocator;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	class Tool {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref Tool instance.
		/*!	@param[in] allocator @ref Allocator the new @ref Tool should use to perform internal allocations.
			*/
		Tool( ::Eldritch2::Allocator& allocator );

		//!	Destroys this @ref Tool instance.
		~Tool() = default;
	};

}	// namespace Detail

	template <class ImplementingTool>
	class ToolCRTPBase : public Detail::Tool {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ToolCRTPBase instance.
		/*!	@param[in] allocator @ref Allocator the new @ref Tool should use to perform internal allocations.
			*/
		ToolCRTPBase( ::Eldritch2::Allocator& allocator );

		//!	Destroys this @ref ToolCRTPBase instance.
		~ToolCRTPBase() = default;

	// ---------------------------------------------------

		int	Run( ::Eldritch2::Range<::Eldritch2::SystemChar**> arguments );
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/Tool.inl>
//------------------------------------------------------------------//