/*==================================================================*\
  AngelScript::UserDefinedTypeRegistrar.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <angelscript.h>
//------------------------------------------------------------------//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace Detail {

	const char* const	operatorNameTable[static_cast<size_t>(AngelScript::UserDefinedTypeRegistrar::BinaryOperatorClass::OPERATOR_COUNT)] = {
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
	struct TypeDeclarationFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			outputStream << AngelScript::UserDefinedTypeRegistrar::TypeStringGenerator<Type>::GetPrefix() << AngelScript::UserDefinedTypeRegistrar::TypeStringGenerator<Type>::GetTypeName() << AngelScript::UserDefinedTypeRegistrar::TypeStringGenerator<Type>::GetSuffix();

			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename Type>
	struct PropertyDeclarationFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream, const char* propertyName ) {
			Detail::TypeDeclarationFormatter<Type>::Format( outputStream ) << " " << propertyName << '\0';
			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename... Arguments>
	struct FunctionArgumentFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			Detail::TypeDeclarationFormatter<Type>::Format( outputStream ) << " " << propertyName;
			return outputStream;
		}
	};

// ---------------------------------------------------

	template <>
	struct FunctionArgumentFormatter<> {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename Argument, typename... AntecedentArguments>
	struct FunctionArgumentFormatter<Argument, AntecedentArguments...> {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream ) {
			Detail::TypeDeclarationFormatter<Argument>::Format( outputStream );
			if( 0 != sizeof...(AntecedentArguments) ) {
				outputStream << ", ";
			}

			Detail::FunctionArgumentFormatter<AntecedentArguments...>::Format( outputStream );

			return outputStream;
		}
	};

// ---------------------------------------------------

	template <typename Return, typename... Arguments>
	struct FunctionDeclarationFormatter {
		template <typename OutputStream>
		static OutputStream& Format( OutputStream& outputStream, const char* const functionName, bool isConst = false ) {
			Detail::TypeDeclarationFormatter<Return>::Format( outputStream ) << " " << functionName << "(";
			Detail::FunctionArgumentFormatter<Arguments...>::Format( outputStream ) << (isConst ? ") const" : ")") << '\0';
			return outputStream;
		}
	};

}	// namespace Detail

	template <typename Native>
	const char* UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetPrefix() {
		return "";
	}

// ---------------------------------------------------

	template <typename Native>
	const char* UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName() {
		return Native::scriptTypeName;
	}

// ---------------------------------------------------

	template <typename Native>
	const char* UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetSuffix() {
		return "";
	}

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<void> {
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
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int8> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int8";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint8> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint8";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int16> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int16";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint16> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint16";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int32> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int32";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint32> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint32";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::int64> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int64";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::uint64> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint64";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::float32> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "float";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<::Eldritch2::float64> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "double";
		}
	};

// ---------------------------------------------------

	template <>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<bool> : public UserDefinedTypeRegistrar::TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "bool";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<Native*> : public UserDefinedTypeRegistrar::TypeStringGenerator<Native> {
		static const char* GetSuffix() {
			return "@";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<Native* const> : public UserDefinedTypeRegistrar::TypeStringGenerator<Native*> {};

// ---------------------------------------------------

	template <typename Native>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<const Native*> : public UserDefinedTypeRegistrar::TypeStringGenerator<Native*> {
		static const char* GetPrefix() {
			return "const ";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<const Native* const> : public UserDefinedTypeRegistrar::TypeStringGenerator<const Native*> {};

// ---------------------------------------------------

	template <typename Native>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<Native&> : public UserDefinedTypeRegistrar::TypeStringGenerator<Native> {
		static const char* GetSuffix() {
			return "&";
		}
	};

// ---------------------------------------------------

	template <typename Native>
	struct UserDefinedTypeRegistrar::TypeStringGenerator<const Native&> : public UserDefinedTypeRegistrar::TypeStringGenerator<Native&> {
		static const char* GetPrefix() {
			return "const ";
		}

		static const char* GetSuffix() {
			return "&in";
		}
	};

// ---------------------------------------------------

	template <class Message>
	ETInlineHint UserDefinedTypeRegistrar::MessageTypeBuilder<Message>::MessageTypeBuilder( ::asIScriptEngine& scriptEngine ) : _scriptEngine( scriptEngine ) {}

// ---------------------------------------------------

	template <class Message>
	template <typename Property>
	UserDefinedTypeRegistrar::MessageTypeBuilder<Message>& UserDefinedTypeRegistrar::MessageTypeBuilder<Message>::ExposeProperty( const char* const propertyName, Property Message::* propertyPointer ) {
		using namespace ::boost::iostreams;

	// ---

		char						signature[128];
		basic_array_sink<char>		sink( signature );
		stream<decltype(sink)>		stream( sink );

		Detail::PropertyDeclarationFormatter<Property>::Format( stream, propertyName );

		const int	result( _scriptEngine.RegisterObjectProperty( UserDefinedTypeRegistrar::TypeStringGenerator<Message>::GetTypeName(), signature, ::std::distance( reinterpret_cast<char*>(1), reinterpret_cast<char*>(&(reinterpret_cast<Message*>(1)->*propertyPointer)) ) ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	ETInlineHint UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::TypeBuilderCRTPBase( ::asIScriptEngine& scriptEngine ) : _scriptEngine( scriptEngine ) {}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename ResultObject>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeValueCastTo() {
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

		Detail::FunctionDeclarationFormatter<ResultObject>::Format( stream, "f", true );

		const int	result( _scriptEngine.RegisterObjectBehaviour( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_VALUE_CAST, signature, ::asFunctionPtr( &CastFunctionHelper::Cast ), ::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeProperty( const char* const propertyName, Property Native::* propertyPointer ) {
		using namespace ::boost::iostreams;

	// ---

		char						signature[128];
		basic_array_sink<char>		sink( signature );
		stream<decltype(sink)>		stream( sink );

		Detail::PropertyDeclarationFormatter<Property>::Format( stream, propertyName );

		const int	result( _scriptEngine.RegisterObjectProperty( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), signature, ::std::distance( reinterpret_cast<char*>(1), reinterpret_cast<char*>(&(reinterpret_cast<Native*>(1)->*propertyPointer)) ) ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (Native::*getterFunction)() ) {
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( StringLength( propertyName ) + 5u )), "get_%s", propertyName ) );

		return RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(getterFunction) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (Native::*getterFunction)() const ) {
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		Detail::FunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ), true );

		return RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(getterFunction) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( Native* ) ) {
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		Detail::FunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ) );

		return RegisterFunction( signature, ::asFunctionPtr( getterFunction ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( const Native* ) ) {
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		Detail::FunctionDeclarationFormatter<Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ), true );

		return RegisterFunction( signature, ::asFunctionPtr( getterFunction ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, void (Native::*setterFunction)(Property) ) {
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		Detail::FunctionDeclarationFormatter<void, Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "set_%s", propertyName ) );

		return RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(setterFunction) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Property>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( Native*, Property ) ) {
		using namespace ::boost::iostreams;
		using namespace ::Eldritch2;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( StringLength( propertyName ) + 5u );

		Detail::FunctionDeclarationFormatter<void, Property>::Format( stream, PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "set_%s", propertyName ) );

		return RegisterFunction( signature, ::asFunctionPtr( setterFunction ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName );

		return RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(method) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) const ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName, true );

		return RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(method) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( Native*, Arguments... ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName );

		return RegisterFunction( signature, ::asFunctionPtr( method ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename... Arguments>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const Native*, Arguments... ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<Return, Arguments...>::Format( stream, methodName, true );

		return RegisterFunction( signature, ::asFunctionPtr( method ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (Native::*operatorMethod)( RightHandOperand ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		Detail::FunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, angelscriptOperatorNameTable[operatorClass] );

		return RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(operatorMethod) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (Native::*operatorMethod)( RightHandOperand ) const ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		Detail::FunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, angelscriptOperatorNameTable[operatorClass], true );

		return RegisterFunction( signature, reinterpret_cast<void (Native::*)()>(operatorMethod) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* operatorMethod)( Native*, RightHandOperand ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		Detail::FunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, Detail::operatorNameTable[static_cast<size_t>(operatorClass)] );

		return RegisterFunction( signature, ::asFunctionPtr( operatorMethod ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	template <typename Return, typename RightHandOperand>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* operatorMethod)( const Native*, RightHandOperand ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		ETRuntimeVerificationWithMsg( BinaryOperatorClass::OPERATOR_COUNT != operatorClass, "Failed exposing script API to engine!" );

		Detail::FunctionDeclarationFormatter<Return, RightHandOperand>::Format( stream, Detail::operatorNameTable[static_cast<size_t>(operatorClass)] );

		return RegisterFunction( signature, ::asFunctionPtr( operatorMethod ) );
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::RegisterFunction( const char* const signature, void (Native::*untypedNativePointer)() ) {
		const auto result( _scriptEngine.RegisterObjectMethod( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), signature, ::asSMethodPtr<sizeof(untypedNativePointer)>::Convert( untypedNativePointer ), ::asCALL_THISCALL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native, typename ImplementingType>
	ImplementingType& UserDefinedTypeRegistrar::TypeBuilderCRTPBase<Native, ImplementingType>::RegisterFunction( const char* const signature, const ::asSFuncPtr& functionPointer ) {
		const auto result( _scriptEngine.RegisterObjectMethod( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), signature, functionPointer, ::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return static_cast<ImplementingType&>(*this);
	}

// ---------------------------------------------------

	template <typename Native>
	ETInlineHint UserDefinedTypeRegistrar::ValueTypeBuilder<Native>::ValueTypeBuilder( ::asIScriptEngine& scriptEngine ) : TypeBuilderCRTPBase<Native, ValueTypeBuilder<Native>>( scriptEngine ) {}

// ---------------------------------------------------

	template <typename Native>
	template <typename... Arguments>
	UserDefinedTypeRegistrar::ValueTypeBuilder<Native>& UserDefinedTypeRegistrar::ValueTypeBuilder<Native>::ExposeConstructor( void (ETScriptAPICall* function)( void* const, Arguments... ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<void, Arguments...>::Format( stream, "f" );

		const int	result( _scriptEngine.RegisterObjectBehaviour( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_CONSTRUCT, signature, ::asFunctionPtr( function ), ::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	ETInlineHint UserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>::ReferenceTypeBuilder( ::asIScriptEngine& scriptEngine ) : TypeBuilderCRTPBase<Native, ReferenceTypeBuilder<Native>>( scriptEngine ) {}

// ---------------------------------------------------

	template <typename Native>
	template <typename ResultObject>
	UserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>& UserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>::ExposeReferenceCastTo() {
		using namespace ::boost::iostreams;

		using ConstCastFunction	= const ResultObject* (ETScriptAPICall*)( const Native* const );
		using CastFunction		= ResultObject* (ETScriptAPICall*)( Native* const );

	// ---

		char	signature[128];

		{
			basic_array_sink<char>	sink( signature );
			stream<decltype(sink)>	stream( sink );

			Detail::FunctionDeclarationFormatter<ResultObject*>::Format( stream, "opCast" );
			RegisterFunction( signature, ::asFunctionPtr( static_cast<CastFunction>([] ( Native* const thisPointer ) { return static_cast<ResultObject*>(thisPointer); }) ) );
		}

		{
			basic_array_sink<char>	sink( signature );
			stream<decltype(sink)>	stream( sink );

			Detail::FunctionDeclarationFormatter<ResultObject*>::Format( stream, "opCast", true );
			RegisterFunction( signature, ::asFunctionPtr( static_cast<ConstCastFunction>([] ( const Native* const thisPointer ) { return static_cast<const ResultObject*>(thisPointer); }) ) );
		}
		
		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	template <typename... Arguments>
	UserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>& UserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>::ExposeFactory( Native* (ETScriptAPICall* function)( Arguments... ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<Native*, Arguments...>::Format( stream, "f" );

		const int	result( _scriptEngine.RegisterObjectBehaviour( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_FACTORY, signature, ::asFUNCTION( function ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	::Eldritch2::Result<UserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>> UserDefinedTypeRegistrar::RegisterUserDefinedReferenceType( Allocator& builderAllocator ) {
		using AllocationOption	= ::Eldritch2::Allocator::AllocationOption;
		using BuilderType		= UserDefinedTypeRegistrar::ReferenceTypeBuilder<Native>;

	// ---

		EnsureReferenceTypeDeclared<Native>();

		int	result( _scriptEngine.RegisterObjectBehaviour( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_ADDREF, "void f()", ::asMETHOD( Native, AddReference ), ::asCALL_THISCALL ) );
		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );
		result = _scriptEngine.RegisterObjectBehaviour( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_RELEASE, "void f()", ::asMETHOD( Native, ReleaseReference ), ::asCALL_THISCALL );
		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		if( auto builder = new(builderAllocator, AllocationOption::TEMPORARY_ALLOCATION) BuilderType( _scriptEngine ) ) {
			return { ::std::move( builder ) };
		}

		return { Error::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	template <typename Native>
	::Eldritch2::Result<UserDefinedTypeRegistrar::ValueTypeBuilder<Native>> UserDefinedTypeRegistrar::RegisterUserDefinedValueType( ::Eldritch2::Allocator& builderAllocator ) {
		struct DestructorWrapper {
			static void ETCDecl	Invoke( void* const object ) {
				static_cast<Native*>(object)->~Native();
			}
		};

		using AllocationOption	= ::Eldritch2::Allocator::AllocationOption;
		using BuilderType		= UserDefinedTypeRegistrar::ValueTypeBuilder<Native>;

	// ---

		EnsureValueTypeDeclared<Native>();

		const int	result( _scriptEngine.RegisterObjectBehaviour( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), ::asBEHAVE_DESTRUCT, "void f()", ::asFUNCTION( &DestructorWrapper::Invoke ), ::asCALL_CDECL_OBJLAST ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		if( auto builder = new(builderAllocator, AllocationOption::TEMPORARY_ALLOCATION) BuilderType( _scriptEngine ) ) {
			return { ::std::move( builder ) };
		}

		return { Error::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	template <typename Enum>
	::Eldritch2::Result<UserDefinedTypeRegistrar::EnumTypeBuilder<Enum>> UserDefinedTypeRegistrar::RegisterUserDefinedEnumType( ::Eldritch2::Allocator& builderAllocator ) {
		using AllocationOption	= ::Eldritch2::Allocator::AllocationOption;
		using Builder			= UserDefinedTypeRegistrar::EnumTypeBuilder<Enum>;

	// ---

		EnsureEnumTypeDeclared<Enum>();

		if( auto builder = new(builderAllocator, AllocationOption::TEMPORARY_ALLOCATION) Builder( _scriptEngine ) ) {
			return { ::std::move( builder ) };
		}

		return { Error::OUT_OF_MEMORY };
	}

// ---------------------------------------------------

	template <typename Native>
	UserDefinedTypeRegistrar& UserDefinedTypeRegistrar::EnsureReferenceTypeDeclared() {
		static_assert( ::std::is_class<Native>::value, "Type must be a class or struct!" );

	// ---

		const int	result( _scriptEngine.RegisterObjectType( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), sizeof(Native), ::asOBJ_REF ) );
		
		ETRuntimeVerificationWithMsg( (::asSUCCESS <= result) || (::asALREADY_REGISTERED == result), "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Native>
	UserDefinedTypeRegistrar& UserDefinedTypeRegistrar::EnsureValueTypeDeclared() {
		static_assert( ::std::is_class<Native>::value, "Type must be a class or struct!" );

	// ---

		const int	result( _scriptEngine.RegisterObjectType( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName(), sizeof(Native), ::asOBJ_VALUE | ::asGetTypeTraits<Native>() ) );
		
		ETRuntimeVerificationWithMsg( (::asSUCCESS <= result) || (::asALREADY_REGISTERED == result), "Failed exposing script API to engine!" );
		
		return *this;
	}

// ---------------------------------------------------

	template <typename Enum>
	UserDefinedTypeRegistrar& UserDefinedTypeRegistrar::EnsureEnumTypeDeclared() {
		static_assert( ::std::is_enum<Enum>::value, "Type must be an enum!" );
		static_assert( ::std::is_same<::std::underlying_type<Enum>::type, int>::value, "Enum types must be backed by an int for correct handing!" );

	// ---

		const int	result( _scriptEngine.RegisterEnum( UserDefinedTypeRegistrar::TypeStringGenerator<Native>::GetTypeName() ) );

		ETRuntimeVerificationWithMsg( (::asSUCCESS <= result) || (::asALREADY_REGISTERED == result), "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Return, typename... Arguments>
	UserDefinedTypeRegistrar& UserDefinedTypeRegistrar::ExposeFunction( const char* const functionName, Return (ETScriptAPICall* function)( Arguments... ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );

		Detail::FunctionDeclarationFormatter<Return, Arguments...>::Format( stream, functionName );
		const int	result( _scriptEngine.RegisterGlobalFunction( signature, ::asFUNCTION( function ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Property>
	UserDefinedTypeRegistrar& UserDefinedTypeRegistrar::ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)() ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( ::Eldritch2::StringLength( propertyName ) + 5u );

		Detail::FunctionDeclarationFormatter<Property>::Format( stream, ::Eldritch2::PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "get_%s", propertyName ) );
		const int	result( _scriptEngine.RegisterGlobalFunction( signature, ::asFUNCTION( getterFunction ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

// ---------------------------------------------------

	template <typename Property>
	UserDefinedTypeRegistrar& UserDefinedTypeRegistrar::ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( Property ) ) {
		using namespace ::boost::iostreams;

	// ---

		char					signature[128];
		basic_array_sink<char>	sink( signature );
		stream<decltype(sink)>	stream( sink );
		const size_t			nameBufferSize( ::Eldritch2::StringLength( propertyName ) + 5u );

		Detail::FunctionDeclarationFormatter<void, Property>::Format( stream, ::Eldritch2::PrintFormatted( static_cast<char*>(_alloca( nameBufferSize )), nameBufferSize, "set_%s", propertyName) );
		const int	result( _scriptEngine.RegisterGlobalFunction( signature, ::asFUNCTION( setterFunction ), ::asCALL_CDECL ) );

		ETRuntimeVerificationWithMsg( ::asSUCCESS <= result, "Failed exposing script API to engine!" );

		return *this;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2
