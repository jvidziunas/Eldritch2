/*==================================================================*\
  Message.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class Message : public ::Eldritch2::IntrusiveForwardListBaseHook {
	public:
		ETInlineHint const ::Eldritch2::UTF8Char* const GetTypeName() const;

		ETInlineHint void*								GetPayload() const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref Message instance.
		Message( const ::Eldritch2::UTF8Char* const typeName );
		//!	Constructs this @ref Message instance.
		Message( const Message& ) = delete;

	public:
		//!	Destroys this @ref Message instance.
		~Message() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::UTF8Char* const	_typeName;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Message.inl>
//------------------------------------------------------------------//
