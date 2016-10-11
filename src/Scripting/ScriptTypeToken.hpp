/*==================================================================*\
  ScriptTypeToken.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Detail {

	class ScriptTypeToken {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using RawToken = Eldritch2::uintptr;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref ScriptTypeToken instance.
		ScriptTypeToken( RawToken value );
	//!	Constructs this @ref ScriptTypeToken instance.
		ScriptTypeToken( const ScriptTypeToken& ) = default;

		~ScriptTypeToken() = default;

	// ---------------------------------------------------

	public:
		operator	RawToken() const;

	// - DATA MEMBERS ------------------------------------

	private:
		RawToken	_value;
	};

}	// namespace Detail

	using RawTypeToken	= Detail::ScriptTypeToken::RawToken;

// ---

	template <typename Native>
	class ScriptTypeToken : public Detail::ScriptTypeToken {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ScriptTypeToken instance.
		ScriptTypeToken( RawToken token );
	//!	Constructs this @ref ScriptTypeToken instance.
		ScriptTypeToken( const ScriptTypeToken& ) = default;

		~ScriptTypeToken() = default;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ScriptTypeToken.inl>
//------------------------------------------------------------------//