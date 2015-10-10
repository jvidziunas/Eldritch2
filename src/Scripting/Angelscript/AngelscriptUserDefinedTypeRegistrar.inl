/*==================================================================*\
  AngelscriptUserDefinedTypeRegistrar.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/ResultPair.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2Detail {
namespace Scripting {

	const char* const	angelscriptOperatorNameTable[static_cast<size_t>(::Eldritch2::Scripting::BinaryOperatorClass::OPERATOR_COUNT)] = {
		"opAdd",
		"opSub",
		"opMul",
		"opDiv",

		"opAssign",

		"opAddAssign",
		"opSubAssign",
		"opMulAssign",
		"opDivAssign"
	};

// ---------------------------------------------------

	template <typename Type>
	struct AngelscriptTypeDeclarationFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			outputStream << ::Eldritch2::Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Type>::GetPrefix()
						 << ::Eldritch2::Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Type>::GetTypeName()
						 << ::Eldritch2::Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Type>::GetSuffix();

			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename Type>
	struct AngelscriptPropertyDeclarationFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream, const char* propertyName ) {
			AngelscriptTypeDeclarationFormatter<Type>::Format( outputStream ) << " " << propertyName << '\0';
			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename... Arguments>
	struct AngelscriptFunctionArgumentFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			AngelscriptTypeDeclarationFormatter<Type>::Format( outputStream ) << " " << propertyName;
			return outputStream;
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptFunctionArgumentFormatter<> {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename Argument, typename... AntecedentArguments>
	struct AngelscriptFunctionArgumentFormatter<Argument, AntecedentArguments...> {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			AngelscriptTypeDeclarationFormatter<Argument>::Format( outputStream );
			if( 0 != sizeof...(AntecedentArguments) ) {
				outputStream << ", ";
			}

			AngelscriptFunctionArgumentFormatter<AntecedentArguments...>::Format( outputStream );

			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename Return, typename... Arguments>
	struct AngelscriptFunctionDeclarationFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream, const char* const functionName, bool isConst = false ) {
			AngelscriptTypeDeclarationFormatter<Return>::Format( outputStream ) << " " << functionName << "(";
			AngelscriptFunctionArgumentFormatter<Arguments...>::Format( outputStream ) << (isConst ? ") const" : ")") << '\0';
			return outputStream;
		}
	};

}	// namespace Scripting
}	// namespace Eldritch2Detail

namespace Eldritch2 {
namespace Scripting {

	namespace Scripting	= ::Eldritch2::Scripting;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename Native>
	const char* AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetPrefix() {
		return "";
	}

// ---------------------------------------------------

	template <typename Native>
	const char* AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName() {
		return Native::scriptTypeName;
	}

// ---------------------------------------------------

	template <typename Native>
	const char* AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetSuffix() {
		return "";
	}

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetPrefix() {
			return "";
		}

		static const char*	GetTypeName() {
			return "void";
		}

		static const char*	GetSuffix() {
			return "";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int8> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int8";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint8> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint8";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int16> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int16";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint16> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint16";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int32> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int32";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint32> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint32";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int64> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int64";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint64> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint64";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::float32> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "float";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::float64> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "double";
		}
	};

// ---------------------------------------------------

	template <>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<bool> : public Scripting::AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "bool";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native*> : public AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native> {
		static const char* GetSuffix() {
			return "@";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native* const> : public AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native*> {};

// ---------------------------------------------------

	template <typename Native>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<const Native*> : public AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native*> {
		static const char* GetPrefix() {
			return "const ";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<const Native* const> : public AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<const Native*> {};

// ---------------------------------------------------

	template <typename Native>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native&> : public AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native> {
		static const char* GetSuffix() {
			return "&";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<const Native&> : public AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native&> {
		static const char* GetPrefix() {
			return "const ";
		}

		static const char* GetSuffix() {
			return "&in";
		}
	};

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	ETInlineHint AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::TypeBuilderCRTPBase( ::asIScriptEngine& scriptEngine ) : _scriptEngine( scriptEngine ) {}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename ResultObject>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeValueCastTo() {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

		struct CastFunctionHelper {
			static ETNoAliasHint ResultObject ETScriptAPICall Cast( const Native* const thisPointer ) {
				return static_cast<ResultObject>(*thisPointer);
			}
		};

	// ---

		char						signature[128];
		basic_array_sink<char>		sink( signature );
		stream<decltype(sink)>		stream( sink );

		AngelscriptFunctionDeclarationFormatter<ResultObject>::Format( stream, "f", true );

		const int	result( _scriptEngine.RegisterObjectBehaviour( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_VALUE_CAST, signature, ::asFunctionPtr( &CastFunctionHelper::Cast ), ::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeProperty( const char* const propertyName, Property Native::* propertyPointer ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char						signature[128];
		basic_array_sink<char>		sink( signature );
		stream<decltype(sink)>		stream( sink );

		AngelscriptPropertyDeclarationFormatter<Property>::Format( stream, propertyName );

		const int	result( _scriptEngine.RegisterObjectProperty( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), signature, ::std::distance( reinterpret_cast<char*>(1), reinterpret_cast<char*>(&(reinterpret_cast<Native*>(1)->*propertyPointer)) ) ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (Native::*getterFunction)() ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( StringLength( propertyName ) + 5u )), "get_%s", propertyName ) );

		return this->RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(getterFunction) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (Native::*getterFunction)() const ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		AngelscriptFunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ), true );

		return this->RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(getterFunction) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( Native* ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		AngelscriptFunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ) );

		return this->RegisterFunction( signature, ::asFunctionPtr( getterFunction ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( const Native* ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		AngelscriptFunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ), true );

		return this->RegisterFunction( signature, ::asFunctionPtr( getterFunction ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, void (Native::*setterFunction)(Property) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		AngelscriptFunctionDeclarationFormatter<void, Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "set_%s", propertyName ) );

		return this->RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(setterFunction) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( Native*, Property ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		AngelscriptFunctionDeclarationFormatter<void, Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "set_%s", propertyName ) );

		return this->RegisterFunction( signature, ::asFunctionPtr( setterFunction ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName );

		return this->RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(method) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) const ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName, true );

		return this->RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(method) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( Native*, Arguments... ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName );

		return this->RegisterFunction( signature, ::asFunctionPtr( method ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const Native*, Arguments... ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName, true );

		return this->RegisterFunction( signature, ::asFunctionPtr( method ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( Scripting::BinaryOperatorClass operatorClass, Return (Native::*operatorMethod)( RightHandOperand ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		AngelscriptFunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, angelscriptOperatorNameTable[operatorClass] );

		return this->RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(operatorMethod) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( Scripting::BinaryOperatorClass operatorClass, Return (Native::*operatorMethod)( RightHandOperand ) const ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		AngelscriptFunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, angelscriptOperatorNameTable[operatorClass], true );

		return this->RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(operatorMethod) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( Scripting::BinaryOperatorClass operatorClass, Return (ETScriptAPICall* operatorMethod)( Native*, RightHandOperand ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		AngelscriptFunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, angelscriptOperatorNameTable[static_cast<size_t>(operatorClass)] );

		return this->RegisterFunction( signature, ::asFunctionPtr( operatorMethod ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( Scripting::BinaryOperatorClass operatorClass, Return (ETScriptAPICall* operatorMethod)( const Native*, RightHandOperand ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		AngelscriptFunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, angelscriptOperatorNameTable[static_cast<size_t>(operatorClass)] );

		return this->RegisterFunction( signature, ::asFunctionPtr( operatorMethod ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::RegisterFunction( const char* const signature, void (Native::*untypedNativePointer)() ) {
		const auto result( _scriptEngine.RegisterObjectMethod( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), signature, ::asSMethodPtr<sizeof(untypedNativePointer)>::Convert( untypedNativePointer ), ::asCALL_THISCALL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	ImplementingType& AngelscriptUserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::RegisterFunction( const char* const signature, const ::asSFuncPtr& functionPointer ) {
		const auto result( _scriptEngine.RegisterObjectMethod( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), signature, functionPointer, ::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native>
	ETInlineHint AngelscriptUserDefinedTypeRegistrar::ValueTypeBuilder<Native>::ValueTypeBuilder( ::asIScriptEngine& scriptEngine ) : TypeBuilderCRTPBase<Native, ValueTypeBuilder<Native>>( scriptEngine ) {}

// ---------------------------------------------------

	template <typename Native>
	template <typename... Arguments>
	AngelscriptUserDefinedTypeRegistrar::ValueTypeBuilder<Native>& AngelscriptUserDefinedTypeRegistrar::ValueTypeBuilder<Native>::ExposeConstructor( void (ETScriptAPICall* function)( void* const, Arguments... ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<void, Arguments...>::Format( stream, "f" );

		const int	result( _scriptEngine.RegisterObjectBehaviour( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_CONSTRUCT, signature, ::asFunctionPtr( function ), ::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	ETInlineHint AngelscriptUserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>::ReferenceTypeBuilder( ::asIScriptEngine& scriptEngine ) : TypeBuilderCRTPBase<Native, ReferenceTypeBuilder<Native>>( scriptEngine ) {}

// ---------------------------------------------------

	template <typename Native>
	template <typename ResultObject>
	AngelscriptUserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>& AngelscriptUserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>::ExposeReferenceCastTo() {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

		using ConstCastFunction	= const ResultObject* (ETScriptAPICall*)( const Native* const );
		using CastFunction		= ResultObject* (ETScriptAPICall*)( Native* const );

	// ---

		char	signature[128];

		{
			basic_array_sink<char>	sink( signature );
			stream<decltype(sink)>	stream( sink );

			AngelscriptFunctionDeclarationFormatter<ResultObject*>::Format( stream, "opCast" );
			this->RegisterFunction( signature, ::asFunctionPtr( static_cast<CastFunction>([] ( Native* const thisPointer ) { return static_cast<ResultObject*>(thisPointer); }) ) );
		}

		{
			basic_array_sink<char>	sink( signature );
			stream<decltype(sink)>	stream( sink );

			AngelscriptFunctionDeclarationFormatter<ResultObject*>::Format( stream, "opCast", true );
			this->RegisterFunction( signature, ::asFunctionPtr( static_cast<ConstCastFunction>([] ( const Native* const thisPointer ) { return static_cast<const ResultObject*>(thisPointer); }) ) );
		}
		
		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	template <typename... Arguments>
	AngelscriptUserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>& AngelscriptUserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>::ExposeFactory( Native* (ETScriptAPICall* function)( Arguments... ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<Native*, Arguments...>::Format( stream, "f" );

		const int	result( _scriptEngine.RegisterObjectBehaviour( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_FACTORY, signature, ::asFUNCTION( function ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	Utility::ResultPair<AngelscriptUserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>> AngelscriptUserDefinedTypeRegistrar::RegisterUserDefinedReferenceType( Allocator& builderAllocator ) {
		using BuilderType = AngelscriptUserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>;

	// ---

		EnsureReferenceTypeDeclared<Native>();

		int	result( _scriptEngine.RegisterObjectBehaviour( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_ADDREF, "void f()", ::asMETHOD( Native, AddReference ), ::asCALL_THISCALL ) );
		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );
		result = _scriptEngine.RegisterObjectBehaviour( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_RELEASE, "void f()", ::asMETHOD( Native, ReleaseReference ), ::asCALL_THISCALL );
		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		Utility::ResultPair<BuilderType>	resultPair { new(builderAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) BuilderType( _scriptEngine ), Errors::NONE };

		if( !resultPair.object ) {
			resultPair.resultCode = ::Eldritch2::Errors::OUT_OF_MEMORY;
		}

		return resultPair;
	}

// ---------------------------------------------------

	template <typename Native>
	Utility::ResultPair<AngelscriptUserDefinedTypeRegistrar::ValueTypeBuilder<Native>> AngelscriptUserDefinedTypeRegistrar::RegisterUserDefinedValueType( ::Eldritch2::Allocator& builderAllocator ) {
		struct DestructorWrapper {
			static void ETCDecl	Invoke( void* const object ) {
				static_cast<Native*>(object)->~Native();
			}
		};

		using AllocationOption	= Allocator::AllocationOption;
		using BuilderType		= AngelscriptUserDefinedTypeRegistrar::ValueTypeBuilder<Native>;

	// ---

		EnsureValueTypeDeclared<Native>();

		const int	result( _scriptEngine.RegisterObjectBehaviour( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_DESTRUCT, "void f()", ::asFUNCTION( &DestructorWrapper::Invoke ), ::asCALL_CDECL_OBJLAST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		Utility::ResultPair<BuilderType>	resultPair { new(builderAllocator, AllocationOption::TEMPORARY_ALLOCATION) BuilderType( _scriptEngine ), Errors::NONE };

		if( !resultPair.object ) {
			resultPair.resultCode = ::Eldritch2::Errors::OUT_OF_MEMORY;
		}

		return resultPair;
	}

// ---------------------------------------------------

	template <typename Enum>
	Utility::ResultPair<AngelscriptUserDefinedTypeRegistrar::UserDefinedEnumBuilder<Enum>> AngelscriptUserDefinedTypeRegistrar::RegisterUserDefinedEnumType( ::Eldritch2::Allocator& builderAllocator ) {
		using AllocationOption	= Allocator::AllocationOption;
		using BuilderType		= AngelscriptUserDefinedTypeRegistrar::UserDefinedEnumBuilder<Enum>;

	// ---

		EnsureEnumDeclared<Enum>();

		Utility::ResultPair<BuilderType>	result { new(builderAllocator, AllocationOption::TEMPORARY_ALLOCATION) BuilderType( _scriptEngine ), Errors::NONE };

		if( !result.object ) {
			result.resultCode = ::Eldritch2::Errors::OUT_OF_MEMORY;
		}

		return result;
	}

// ---------------------------------------------------

	template <typename Native>
	Scripting::AngelscriptUserDefinedTypeRegistrar& AngelscriptUserDefinedTypeRegistrar::EnsureReferenceTypeDeclared() {
		static_assert( ::std::is_class<Native>::value, "Type must be a class or struct!" );

	// ---

		const int	result( _scriptEngine.RegisterObjectType( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), sizeof(Native), ::asOBJ_REF ) );
		
		ETRuntimeVerificationWithMsg( (::asSUCCESS <= result) || (::asALREADY_REGISTERED == result), "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	Scripting::AngelscriptUserDefinedTypeRegistrar& AngelscriptUserDefinedTypeRegistrar::EnsureValueTypeDeclared() {
		static_assert( ::std::is_class<Native>::value, "Type must be a class or struct!" );

	// ---

		const int	result( _scriptEngine.RegisterObjectType( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), sizeof(Native), ::asOBJ_VALUE | ::asGetTypeTraits<Native>() ) );
		
		ETRuntimeVerificationWithMsg( (::asSUCCESS <= result) || (::asALREADY_REGISTERED == result), "Failed exposing script API to engine!" );
		
		return *this;
	}

// ---------------------------------------------------

	template <typename Enum>
	Scripting::AngelscriptUserDefinedTypeRegistrar& AngelscriptUserDefinedTypeRegistrar::EnsureEnumDeclared() {
		static_assert( ::std::is_enum<Enum>::value, "Type must be an enum!" );
		static_assert( ::std::is_same<::std::underlying_type<Enum>::type, int>::value, "Enum types must be backed by an int for correct handing.!" );

	// ---

		const int	result( _scriptEngine.RegisterEnum( AngelscriptUserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName() ) );

		ETRuntimeVerificationWithMsg( (::asSUCCESS <= result) || (::asALREADY_REGISTERED == result), "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Return, typename... Arguments>
	Scripting::AngelscriptUserDefinedTypeRegistrar& AngelscriptUserDefinedTypeRegistrar::ExposeFunction( const char* const functionName, Return (ETScriptAPICall* function)( Arguments... ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		AngelscriptFunctionDeclarationFormatter<Return, Arguments...>::Format( stream, functionName );
		const int	result( _scriptEngine.RegisterGlobalFunction( signature, ::asFUNCTION( function ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Property>
	Scripting::AngelscriptUserDefinedTypeRegistrar& AngelscriptUserDefinedTypeRegistrar::ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)() ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( ::Eldritch2::StringLength( propertyName ) + 5u );

		AngelscriptFunctionDeclarationFormatter<Property>::Format( stream, ::Eldritch2::PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ) );
		const int	result( _scriptEngine.RegisterGlobalFunction( signature, ::asFUNCTION( getterFunction ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Property>
	Scripting::AngelscriptUserDefinedTypeRegistrar& AngelscriptUserDefinedTypeRegistrar::ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( Property ) ) {
		using namespace ::Eldritch2Detail::Scripting;
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( ::Eldritch2::StringLength( propertyName ) + 5u );

		AngelscriptFunctionDeclarationFormatter<void, Property>::Format( stream, ::Eldritch2::PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "set_%s", propertyName) );
		const int	result( _scriptEngine.RegisterGlobalFunction( signature, ::asFUNCTION( setterFunction ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

}	// namespace Scripting
}	// namespace Eldritch2
