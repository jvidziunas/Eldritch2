/*==================================================================*\
  AngelscriptObjectFactory.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <boost/preprocessor/punctuation/comma_if.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

#define IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( namespaceQualifier, name )																	\
	template<>																																	\
	struct AngelscriptTypeNameGen< BOOST_PP_CAT( namespaceQualifier, BOOST_PP_CAT( ::, name ) )> {												\
	public:																																		\
		static ETForceInlineHint const char* GetTypeName( const BOOST_PP_CAT( namespaceQualifier, BOOST_PP_CAT( ::, name ) ) & parameter ) {	\
			ETUnreferencedParameter( parameter );																								\
			static const char	typeName[]	= STRINGIZE( name );																				\
																																				\
			return typeName;																													\
		}																																		\
	}

	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, int8 );
	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, uint8 );
	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, int16 );
	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, uint16 );
	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, int32 );
	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, uint32 );
	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, int64 );
	IMPLEMENT_ANGELSCRIPT_TYPE_SPECIALIZATION( ::Eldritch2, uint64 );

// ---------------------------------------------------

	template <>
	struct AngelscriptTypeNameGen<::Eldritch2::float32> {
	public:
		static ETForceInlineHint const char* GetTypeName( const ::Eldritch2::float32& parameter ) {
			static const char	typeName[]	= "float";

			return typeName;
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptTypeNameGen<::Eldritch2::float64> {
	public:
		static ETForceInlineHint const char* GetTypeName( const ::Eldritch2::float64& parameter ) {
			static const char	typeName[]	= "double";

			return typeName;
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptTypeNameGen<bool> {
	public:
		static ETForceInlineHint const char* GetTypeName( const bool& parameter ) {
			static const char	typeName[]	= "bool";

			return typeName;
		}
	};

// ---------------------------------------------------

	template<>
	struct AngelscriptTypeNameGen<::asIScriptObject*> {
	public:
		static ETForceInlineHint const char* GetTypeName( const ::asIScriptObject* parameter ) {
			return parameter->GetObjectType()->GetName();
		}
	};

}	// namespace Scripting
}	// namespace Eldritch2