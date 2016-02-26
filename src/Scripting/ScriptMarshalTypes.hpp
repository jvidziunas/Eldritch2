/*==================================================================*\
  ScriptMarshalTypes.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
		class	ScriptApiRegistrationInitializationVisitor;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scripting {

	class StringMarshal : public ::Eldritch2::UTF8String<> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StringMarshal instance.
		StringMarshal( ::Eldritch2::Allocator& allocator );
		//!	Constructs this @ref StringMarshal instance.
		StringMarshal( const ::Eldritch2::UTF8Char* const string, const size_t stringLengthInOctets, ::Eldritch2::Allocator& allocator );
		//!	Constructs this @ref StringMarshal instance.
		StringMarshal( const Scripting::StringMarshal& string, ::Eldritch2::Allocator& allocator );

		//!	Destroys this @ref StringMarshal instance.
		~StringMarshal() = default;

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

		static void	AttachWorldAllocator( ::Eldritch2::Allocator& allocator );

	// - DATA MEMBERS ------------------------------------

		static const char* const	scriptTypeName;
	};

// ---

	class Float4Marshal : public ::Eldritch2::Float4 {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Float4Marshal instance.
		Float4Marshal( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		//!	Constructs this @ref Float4Marshal instance.
		Float4Marshal( const Scripting::Float4Marshal& marshal ) = default;
		//!	Constructs this @ref Float4Marshal instance.
		Float4Marshal( const ::Eldritch2::Float4 initializer );

		//!	Destroys this @ref Float4Marshal instance.
		~Float4Marshal() = default;

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// - DATA MEMBERS ------------------------------------

		static const char* const	scriptTypeName;
	};

// ---

	class OrientationMarshal : public ::Eldritch2::Quaternion {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref OrientationMarshal instance.
		OrientationMarshal( const ::Eldritch2::float32 x, const ::Eldritch2::float32 y, const ::Eldritch2::float32 z, const ::Eldritch2::float32 w );
		//!	Constructs this @ref OrientationMarshal instance.
		OrientationMarshal( const Scripting::OrientationMarshal& initializer ) = default;
		//!	Constructs this @ref OrientationMarshal instance.
		OrientationMarshal( const ::Eldritch2::Quaternion initializer );

		//!	Destroys this @ref OrientationMarshal instance.
		~OrientationMarshal() = default;

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// - DATA MEMBERS ------------------------------------

		static const char* const	scriptTypeName;
	};

// ---

	class RigidTransformMarshal {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RigidTransformMarshal instance.
		RigidTransformMarshal( const Scripting::Float4Marshal& translation, const Scripting::OrientationMarshal& orientation );
		//!	Constructs this @ref RigidTransformMarshal instance.
		RigidTransformMarshal( const Scripting::RigidTransformMarshal& transform ) = default;
		//!	Constructs this @ref RigidTransformMarshal instance.
		RigidTransformMarshal( const ::Eldritch2::RigidTransform& transform );

		//!	Destroys this @ref RigidTransformMarshal instance.
		~RigidTransformMarshal() = default;

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// - DATA MEMBERS ------------------------------------

		static const char* const	scriptTypeName;

		Float4Marshal				translation;
		OrientationMarshal			orientation;
	};

}	// namespace Scripting
}	// namespace Eldritch2