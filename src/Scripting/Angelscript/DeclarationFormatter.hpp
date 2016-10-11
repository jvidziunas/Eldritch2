/*==================================================================*\
  DeclarationFormatter.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/TypeHandle.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	TypeDescriptor;
		class	Callable;
		class	Argument;
		class	Function;
		class	Method;
		class	Field;
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class DeclarationFormatter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DeclarationFormatter instance.
		DeclarationFormatter( Eldritch2::Allocator& allocator );
	//!	Constructs this @ref DeclarationFormatter instance.
		DeclarationFormatter( const DeclarationFormatter& ) = default;

		~DeclarationFormatter() = default;

	// ---------------------------------------------------

	public:
		template <typename StringAllocator>
		void	RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::TypeDescriptor& descriptor );

		template <typename StringAllocator>
		void	RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Callable& descriptor );

		template <typename StringAllocator>
		void	RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Argument& descriptor );

		template <typename StringAllocator>
		void	RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Function& descriptor );

		template <typename StringAllocator>
		void	RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Method& descriptor );

		template <typename StringAllocator>
		void	RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Field& descriptor );

	// ---------------------------------------------------

	public:
	//!	Returns the name of a type.
	/*!	@param[in] type Runtime handle of the type to get the name of.
		@returns A null-terminated UTF-8-encoded string containing the type name.
		@see OverrideName */
		Eldritch2::Utf8Literal	GetName( Eldritch2::TypeHandle type ) const;

	//!	Adds an entry to the name override table in this @ref DeclarationFormatter.
	/*!	@param[in] type Runtime handle of the type to insert an override for.
		@param[in] name @parblock Null-terminated UTF-8-encoded C string containing the type name to use. The string must persist
		until after the @ref DeclarationFormatter is destroyed. @endparblock */
		void					OverrideName( Eldritch2::TypeHandle type, Eldritch2::Utf8Literal name );
	//!	Adds an entry to the name override table in this @ref DeclarationFormatter.
	/*!	@param[in] name @parblock Null-terminated UTF-8-encoded C string containing the type name to use. The string must persist
		until after the @ref DeclarationFormatter is destroyed. @endparblock */
		template <typename T>
		void					OverrideName( Eldritch2::Utf8Literal name );

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Collection of custom type name overrides. Used when the C++ type name cannot match the Angelscript name, as is the case for primitive types.
		Eldritch2::HashMap<Eldritch2::TypeHandle, Eldritch2::Utf8Literal>	_nameOverrides;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/AngelScript/DeclarationFormatter.inl>
//------------------------------------------------------------------//