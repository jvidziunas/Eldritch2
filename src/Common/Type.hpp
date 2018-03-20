/*==================================================================*\
  Type.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class Type {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Type instance.
		Type( const std::type_info& type );
	//!	Constructs this @ref Type instance.
		Type( const Type& ) = default;
	//!	Constructs this @ref Type instance.
		Type();

		~Type() = default;

	// ---------------------------------------------------

	public:
		const char*	GetName() const;

	// - DATA MEMBERS ------------------------------------

	private:
		const std::type_info*	_type;

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint size_t	GetHashCode( const Type& type, size_t seed );

		friend ETPureFunctionHint bool		operator==( const Type& left, const Type& right );

		friend ETPureFunctionHint bool		operator!=( const Type& left, const Type& right );

		friend ETPureFunctionHint bool		operator<( const Type& left, const Type& right );

		friend ETPureFunctionHint bool		operator>=( const Type& left, const Type& right );

		friend ETPureFunctionHint bool		operator>( const Type& left, const Type& right );

		friend ETPureFunctionHint bool		operator<=( const Type& left, const Type& right );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Type.inl>
//------------------------------------------------------------------//