/*==================================================================*\
  AngelscriptObjectFactory.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
//------------------------------------------------------------------//

class	asIObjectType;

#ifndef ANGELSCRIPT_OBJECT_FACTORY_MAX_CONSTRUCTOR_PARAMS
#	define ANGELSCRIPT_OBJECT_FACTORY_MAX_CONSTRUCTOR_PARAMS	5
#endif

namespace Eldritch2 {
namespace Scripting {

	template<typename T>
	struct AngelscriptTypeNameGen {
	public:
		static ETForceInlineHint const char*	GetTypeName( const T& parameter );
	};

// ---------------------------------------------------

	class AngelscriptObjectFactory {
	public:
		// Constructs this AngelscriptObjectFactory instance.
		AngelscriptObjectFactory( ::asIObjectType* const objectType, ::asIScriptContext* const scriptContext );

		// Destroys thus AngelscriptObjectFactory instance.
		~AngelscriptObjectFactory();

	// ---------------------------------------------------

		ETInlineHint void*	Construct() {
			const char* const	objectTypeName = _objectType->GetName();
			const char			formatString[]	= "%s@ %s()";
			char				factoryString[256];

			if( asIScriptFunction* const factory = _objectType->GetFactoryByDecl( PrintFormatted( factoryString, formatString, objectTypeName, objectTypeName ) ) ) {
				if( ETBranchUnlikelyHint( 0 > _scriptContext->Prepare( factory ) ) ) {
					return nullptr;
				}

				_scriptContext->Execute();

				if( ETBranchLikelyHint( ::asEXECUTION_FINISHED == _scriptContext->GetState() ) ) {
					_scriptContext->GetEngine()->AddRefScriptObject( _scriptContext->GetReturnAddress(), _objectType );
					return _scriptContext->GetReturnAddress();
				}
			}

			return nullptr;
		}

#define PARAMETER_TOKEN( z, parameterCount, unused ) BOOST_PP_IF( parameterCount, ", ", " " ) "%s"
#define ARGUMENT_ATTACH( z, parameterCount, unused ) ::Eldritch2::CopyMemory( _scriptContext->GetAddressOfArg( parameterCount ), &BOOST_PP_CAT( arg, parameterCount ), sizeof( BOOST_PP_CAT( ArgType, parameterCount ) ) )
#define GET_TYPE_NAME( z, parameterCount, unused ) ::Eldritch2::Scripting::AngelscriptTypeNameGen< BOOST_PP_CAT( ArgType, parameterCount )>::GetTypeName( BOOST_PP_CAT( arg, parameterCount ) )
#define	OBJECT_CONSTRUCT( z, parameterCount, unused )																												\
		template <BOOST_PP_ENUM_PARAMS( parameterCount, class ArgType )>																							\
		ETInlineHint void* Construct( BOOST_PP_ENUM_BINARY_PARAMS( parameterCount, const ArgType, & arg ) ) {														\
			const char* objectTypeName = _objectType->GetName();																									\
			const char	formatString[]	= "%s@ %s(" BOOST_PP_REPEAT( parameterCount, PARAMETER_TOKEN, unused ) " )";												\
			char		factoryString[256];																															\
																																									\
			PrintFormatted( factoryString, formatString, objectTypeName, objectTypeName BOOST_PP_ENUM_TRAILING( parameterCount, GET_TYPE_NAME, unused ) );			\
																																									\
			if( asIScriptFunction* const factory = _objectType->GetFactoryByDecl( factoryString ) ) {																\
				if( ETBranchUnlikelyHint( 0 > _scriptContext->Prepare( factory ) ) )																				\
					return nullptr;																																	\
																																									\
				BOOST_PP_ENUM( parameterCount, ARGUMENT_ATTACH, _scriptContext );																					\
																																									\
				_scriptContext->Execute();																															\
																																									\
				if( ETBranchLikelyHint( asEXECUTION_FINISHED == _scriptContext->GetState() ) ) {																	\
					_scriptContext->GetEngine()->AddRefScriptObject( _scriptContext->GetReturnAddress(), _objectType );												\
					return _scriptContext->GetReturnAddress();																										\
				}																																					\
			}																																						\
																																									\
			return nullptr;																																			\
		}

#define		BOOST_PP_LOCAL_MACRO( n )	OBJECT_CONSTRUCT( ~, n, ~ )
#define		BOOST_PP_LOCAL_LIMITS		( 1,  ANGELSCRIPT_OBJECT_FACTORY_MAX_CONSTRUCTOR_PARAMS )

#include	BOOST_PP_LOCAL_ITERATE()

// BOOST_PP_LOCAL_LIMITS and BOOST_PP_LOCAL_MACRO are automatically undefined.
#undef		OBJECT_CONSTRUCT
#undef		ARGUMENT_ATTACH
#undef		PARAMETER_TOKEN

	// ---------------------------------------------------

	private:
		::asIObjectType* const		_objectType;
		::asIScriptContext* const	_scriptContext;
	};

}	// namespace Scripting

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Angelscript/Utilities/AngelscriptObjectFactory.inl>
//------------------------------------------------------------------//