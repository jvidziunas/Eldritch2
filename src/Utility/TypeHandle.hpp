/*==================================================================*\
  TypeHandle.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class TypeHandle {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TypeHandle instance.
		TypeHandle( const std::type_info& typeInfo );
	//!	Constructs this @ref TypeHandle instance.
		TypeHandle( const TypeHandle& ) = default;
	//!	Constructs this @ref TypeHandle instance.
		TypeHandle();

		~TypeHandle() = default;

	// ---------------------------------------------------

	public:
		const char*	GetName() const;

	// ---------------------------------------------------

	public:
		friend ETPureFunctionHint size_t	GetHashCode( const TypeHandle& typeHandle, size_t seed );

		friend ETPureFunctionHint bool	operator==( const TypeHandle& left, const TypeHandle& right );

		friend ETPureFunctionHint bool	operator!=( const TypeHandle& left, const TypeHandle& right );

		friend ETPureFunctionHint bool	operator<( const TypeHandle& left, const TypeHandle& right );

		friend ETPureFunctionHint bool	operator>=( const TypeHandle& left, const TypeHandle& right );

		friend ETPureFunctionHint bool	operator>( const TypeHandle& left, const TypeHandle& right );

		friend ETPureFunctionHint bool	operator<=( const TypeHandle& left, const TypeHandle& right );

	// - DATA MEMBERS ------------------------------------

	private:
		const std::type_info*	_typeInfo;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/TypeHandle.inl>
//------------------------------------------------------------------//