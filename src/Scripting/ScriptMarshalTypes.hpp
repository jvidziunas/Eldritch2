/*==================================================================*\
  ScriptMarshalTypes.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/UTF8String.hpp>
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scripting {

	class StringMarshal : public ::Eldritch2::UTF8String<> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		StringMarshal( const EmptyStringSemantics, ::Eldritch2::Allocator& allocator );
		// Constructs this StringMarshal instance.
		StringMarshal( const ::Eldritch2::UTF8Char* const string, const size_t stringLengthInOctets, ::Eldritch2::Allocator& allocator );
		// Constructs this StringMarshal instance.
		StringMarshal( const Scripting::StringMarshal& string, ::Eldritch2::Allocator& allocator );

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

		static void	AttachWorldAllocator( ::Eldritch2::Allocator& allocator );

	// - DATA MEMBERS ------------------------------------

		static const char* const	scriptTypeName;
	};

// ---

	class Float4Marshal : public ::Eldritch2::Float4 {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Float4Marshal instance.
		Float4Marshal( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		// Constructs this Float4Marshal instance.
		Float4Marshal( const Scripting::Float4Marshal& marshal );
		// Constructs this Float4Marshal instance.
		Float4Marshal( const ::Eldritch2::Float4 initializer );

		// Destroys this Float4Marshal instance.
		~Float4Marshal();

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// - DATA MEMBERS ------------------------------------

		static const char* const	scriptTypeName;
	};

// ---

	class OrientationMarshal : public ::Eldritch2::Quaternion {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this OrientationMarshal instance.
		OrientationMarshal( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		// Constructs this OrientationMarshal instance.
		OrientationMarshal( const Scripting::OrientationMarshal& initializer );
		// Constructs this OrientationMarshal instance.
		OrientationMarshal( const ::Eldritch2::Quaternion initializer );

		// Destroys this OrientationMarshal instance.
		~OrientationMarshal();

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// - DATA MEMBERS ------------------------------------

		static const char* const	scriptTypeName;
	};

}	// namespace Scripting
}	// namespace Eldritch2