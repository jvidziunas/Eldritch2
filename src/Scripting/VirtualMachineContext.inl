/*==================================================================*\
  VirtualMachineContext.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	template <typename Argument>
	ETInlineHint Argument& VirtualMachineContext::GetArgument( size_t argumentIndex ) const {
		return *static_cast<Argument*>( GetArgumentPointer( argumentIndex ) );
	}

// ---------------------------------------------------

	template <typename Result, typename... Arguments>
	ETInlineHint void VirtualMachineContext::CreateResult( Arguments&&... arguments ) {
		new(GetResultAddress()) Result( eastl::forward<Arguments>(arguments)... );
	}

}	// namespace Scripting
}	// namespace Eldritch2