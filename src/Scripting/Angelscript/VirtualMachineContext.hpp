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
#include <Scripting/VirtualMachineContext.hpp>
//------------------------------------------------------------------//

class	asIScriptGeneric;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class VirtualMachineContext : public Scripting::VirtualMachineContext {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref VirtualMachineContext instance.
		VirtualMachineContext( asIScriptGeneric& scriptInterface );

		~VirtualMachineContext() = default;

	// ---------------------------------------------------

	public:
		void*	GetArgumentPointer( size_t argumentIndex ) const override sealed;

	// ---------------------------------------------------

	public:
		void*	GetResultAddress() const override sealed;

	// ---------------------------------------------------

	public:
		void	ThrowScriptException( const Eldritch2::Utf8Char* const messsage ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		asIScriptGeneric&	_scriptInterface;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2