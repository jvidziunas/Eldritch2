/*==================================================================*\
  VirtualMachineContext.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class ETPureAbstractHint VirtualMachineContext {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref VirtualMachineContext instance.
		VirtualMachineContext( const VirtualMachineContext& ) = default;
	//!	Constructs this @ref VirtualMachineContext instance.
		VirtualMachineContext() = default;

		virtual ~VirtualMachineContext() = default;

	// ---------------------------------------------------

	public:
		virtual void*	GetArgumentPointer( size_t argumentIndex ) const abstract;

		template <typename Argument>
		Argument&		GetArgument( size_t argumentIndex ) const;

	// ---------------------------------------------------

	public:
		virtual void*	GetResultAddress() const;

		template <typename Result, typename... Arguments>
		void			CreateResult( Arguments&&... arguments );

	// ---------------------------------------------------

	public:
		virtual void	ThrowScriptException( const Eldritch2::Utf8Char* const messsage ) abstract;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/VirtualMachineContext.inl>
//------------------------------------------------------------------//