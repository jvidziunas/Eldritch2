/*==================================================================*\
  ScriptApiRegistrationInitializationVisitor.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Detail {

	using DataTypeFlags = Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeFlags;

	template <typename DataType>
	struct TypeDescriptorBuilder {
		static const DataTypeFlags	flags = DataTypeFlags::Normal;

		static ETNoAliasHint Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeDescriptor GetDescriptor() {
			return { Scripting::TypeStringGenerator<DataType>::GetTypeName(), flags };
		}
	};

// ---

	template <typename DataType>
	struct TypeDescriptorBuilder<const DataType> {
		static const DataTypeFlags	flags = static_cast<DataTypeFlags>( TypeDescriptorBuilder<DataType>::flags | DataTypeFlags::Const );

		static ETNoAliasHint Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeDescriptor GetDescriptor() {
			return { Scripting::TypeStringGenerator<DataType>::GetTypeName(), flags };
		}
	};

// ---

	template <typename DataType>
	struct TypeDescriptorBuilder<DataType&> {
		static const DataTypeFlags	flags = static_cast<DataTypeFlags>(TypeDescriptorBuilder<DataType>::flags | DataTypeFlags::Reference);

		static ETNoAliasHint Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeDescriptor GetDescriptor() {
			return { Scripting::TypeStringGenerator<DataType>::GetTypeName(), flags };
		}
	};

// ---

	template <typename DataType>
	struct TypeDescriptorBuilder<const DataType&> {
		static const DataTypeFlags	flags = static_cast<DataTypeFlags>(TypeDescriptorBuilder<const DataType>::flags | DataTypeFlags::Reference);

		static ETNoAliasHint Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeDescriptor GetDescriptor() {
			return { Scripting::TypeStringGenerator<DataType>::GetTypeName(), flags };
		}
	};

// ---

	template <typename DataType>
	struct TypeDescriptorBuilder<DataType*> {
		static const DataTypeFlags	flags = static_cast<DataTypeFlags>(TypeDescriptorBuilder<DataType>::flags | DataTypeFlags::Pointer);

		static ETNoAliasHint Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeDescriptor GetDescriptor() {
			return { Scripting::TypeStringGenerator<DataType>::GetTypeName(), flags };
		}
	};

// ---

	template <typename DataType>
	struct TypeDescriptorBuilder<const DataType*> {
		static const DataTypeFlags	flags = static_cast<DataTypeFlags>(TypeDescriptorBuilder<const DataType>::flags | DataTypeFlags::Pointer);

		static ETNoAliasHint Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeDescriptor GetDescriptor() {
			return { Scripting::TypeStringGenerator<DataType>::GetTypeName(), flags };
		}
	};

// ---

	template <typename DataType>
	struct TypeDescriptorBuilder<DataType* const> : TypeDescriptorBuilder<DataType*> {};

// ---

	template <typename DataType>
	struct TypeDescriptorBuilder<const DataType* const> : TypeDescriptorBuilder<const DataType*> {};

// ---

	template <typename ReferenceType>
	struct HasAddReference {
		struct Dummy { /* something */ };

		template <typename C, typename P>
		static auto					Test( P* ) -> decltype(::std::declval<C>().AddReference(), ::std::true_type());
		template <typename, typename>
		static ::std::false_type	Test( ... );

		typedef decltype(Test<ReferenceType, Dummy>( nullptr ))	type;

		static const bool	value = ::std::is_same<::std::true_type, decltype(Test<ReferenceType, Dummy>( nullptr ))>::value;
	};

// ---

	template <typename ReferenceType>
	struct HasReleaseReference {
		struct Dummy { /* something */ };

		template <typename C, typename P>
		static auto					Test( P* ) -> decltype(::std::declval<C>().ReleaseReference(), ::std::true_type());
		template <typename, typename>
		static ::std::false_type	Test( ... );

		typedef decltype(Test<ReferenceType, Dummy>( nullptr )) type;

		static const bool	value = ::std::is_same<::std::true_type, decltype(Test<ReferenceType, Dummy>( nullptr ))>::value;
	};

// ---

	template <typename RealClass>
	struct TypeHasDefaultReferenceCounting {
		static const bool	value = HasAddReference<RealClass>::value && HasReleaseReference<RealClass>::value;
	};

// ---

	template <typename RealClass, bool hasDefaultReferenceCounting = Detail::TypeHasDefaultReferenceCounting<RealClass>::value>
	struct DispatchReferenceCountingIfAvailable {
		static void Dispatch( Scripting::ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& /*builder*/ ) {}
	};

// ---

	template <typename RealClass>
	struct DispatchReferenceCountingIfAvailable<RealClass, true> {
		static void Dispatch( Scripting::ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& builder ) {
			// If you're getting weird template/lambda errors around here, chances are you're registering the same type multiple times (or registering reference counting yourself when you can have the compiler do it for you!)
			builder.ExposeReferenceCountingMethods( [] ( RealClass* object ) {
				object->AddReference();
			}, [] ( RealClass* object ) {
				object->ReleaseReference();
			} );
		}
	};

// ---

	const Scripting::ScriptApiRegistrationInitializationVisitor::DataTypeDescriptor	nullDescriptor = { nullptr, DataTypeFlags::Normal };

}	// namespace Detail

	template <typename Native>
	const char* TypeStringGenerator<Native>::GetTypeName() {
		return Native::scriptTypeName;
	}

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "void";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::int8> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int8";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::uint8> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint8";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::int16> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int16";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::uint16> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint16";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::int32> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int32";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::uint32> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint32";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::int64> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "int64";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::uint64> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "uint64";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::float32> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "float";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<::Eldritch2::float64> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "double";
		}
	};

// ---------------------------------------------------

	template <>
	struct TypeStringGenerator<bool> : public TypeStringGenerator<void> {
		static const char*	GetTypeName() {
			return "bool";
		}
	};

// ---------------------------------------------------

	template <typename RealEnum>
	ScriptApiRegistrationInitializationVisitor::EnumTypeBuilder<RealEnum>::EnumTypeBuilder( ScriptApiRegistrationInitializationVisitor& owner, TypeToken token ) : _owner( owner ), _token( token ) {}

// ---------------------------------------------------

	template <typename RealEnum>
	ScriptApiRegistrationInitializationVisitor::EnumTypeBuilder<RealEnum>::EnumTypeBuilder( EnumTypeBuilder&& builder ) : _owner( builder._owner ), _token( nullptr ) {
		::std::swap( _token, builder._token );
	}

// ---------------------------------------------------

	template <typename RealEnum>
	ScriptApiRegistrationInitializationVisitor::EnumTypeBuilder<RealEnum>::~EnumTypeBuilder() {
		if( _token ) {
			_owner.CompleteEnumTypeRegistration( _token );
		}
	}

// ---------------------------------------------------

	template <typename RealEnum>
	ScriptApiRegistrationInitializationVisitor::EnumTypeBuilder<RealEnum>& ScriptApiRegistrationInitializationVisitor::EnumTypeBuilder<RealEnum>::ExposeValue( const char* const name, RealEnum value ) {
		return _owner.RegisterEnumValue( _token, name, reinterpret_cast<int>(value) );
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ReferenceTypeBuilder( Scripting::ScriptApiRegistrationInitializationVisitor& owner, TypeToken token ) : _owner( owner ), _token( token ) {
		Detail::DispatchReferenceCountingIfAvailable<RealClass>::Dispatch( *this );
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ReferenceTypeBuilder( ReferenceTypeBuilder&& builder ) : _owner( builder._owner ), _token( nullptr ) {
		::std::swap( _token, builder._token );
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::~ReferenceTypeBuilder() {
		if( _token ) {
			_owner.CompleteReferenceTypeRegistration( _token );
		}
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeFactory( RealClass* (ETScriptAPICall* factory)() ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::nullDescriptor };

		_owner.RegisterReferenceTypeFactory( _token, Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(factory) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename... Arguments>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeFactory( RealClass* (ETScriptAPICall* factory)( Arguments... ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { (Detail::TypeDescriptorBuilder<Arguments>::GetDescriptor())..., Detail::nullDescriptor };

		_owner.RegisterReferenceTypeFactory( _token, Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(factory) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeReferenceCountingMethods( void (ETScriptAPICall* addReferenceFunction)(RealClass*), void (ETScriptAPICall* removeReferenceFunction)(RealClass*) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeReferenceCountMethods( _token, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(addReferenceFunction),
																   Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(removeReferenceFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Result>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeValueCast( Result (ETScriptAPICall* castFunction)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeValueCast( _token, Detail::TypeDescriptorBuilder<Result>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(castFunction) );

		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Result>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeValueCast( Result (ETScriptAPICall* castFunction)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeValueCast( _token, Detail::TypeDescriptorBuilder<Result>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(castFunction) );

		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Result>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeReferenceCast( Result* (ETScriptAPICall* castFunction)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeReferenceCast( _token, Detail::TypeDescriptorBuilder<Result*>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(castFunction) );

		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Result>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeReferenceCast( const Result* (ETScriptAPICall* castFunction)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeReferenceCast( _token, Detail::TypeDescriptorBuilder<const Result*>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(castFunction) );

		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeProperty( const char* const name, Property RealClass::* propertyPointer ) {
		_owner.RegisterReferenceTypeProperty( _token, name, Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), reinterpret_cast<char*>(&(reinterpret_cast<RealClass*>(1)->*propertyPointer)) - reinterpret_cast<char*>(reinterpret_cast<RealClass*>(1)) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, Property (ETScriptAPICall* getterFunction)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(getterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, Property (ETScriptAPICall* getterFunction)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(getterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, void (ETScriptAPICall* setterFunction)( RealClass*, Property ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(setterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, void (ETScriptAPICall* setterFunction)( const RealClass*, Property ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(setterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename... Arguments>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( RealClass*, Arguments... ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), (Detail::TypeDescriptorBuilder<Arguments>::GetDescriptor())..., Detail::nullDescriptor };

		_owner.RegisterReferenceTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename... Arguments>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( const RealClass*, Arguments... ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), (Detail::TypeDescriptorBuilder<Arguments>::GetDescriptor())..., Detail::nullDescriptor };

		_owner.RegisterReferenceTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeUnaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeUnaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename RightHandOperandType>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass*, RightHandOperandType ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<RightHandOperandType>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeBinaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename RightHandOperandType>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass*, RightHandOperandType ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<RightHandOperandType>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterReferenceTypeBinaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ValueTypeBuilder( Scripting::ScriptApiRegistrationInitializationVisitor& owner, TypeToken token ) : _owner( owner ), _token( token ) {
		using	TypedShim = void (ETScriptAPICall*)( RealClass* );

	// ---

		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };
		auto							callShim( [] ( RealClass* object ) { static_cast<void>(object); object->~RealClass(); } );

		owner.RegisterValueTypeDestructor( _token, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(static_cast<TypedShim>(callShim)) );
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ValueTypeBuilder( ValueTypeBuilder&& builder ) : _owner( builder._owner ), _token( nullptr ) {
		::std::swap( _token, builder._token );
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::~ValueTypeBuilder() {
		if( _token ) {
			_owner.CompleteValueTypeRegistration( _token );
		}
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeConstructor( void (ETScriptAPICall* constructor)( void* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<void*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeConstructor( _token, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(constructor) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename... Arguments>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeConstructor( void (ETScriptAPICall* constructor)( void*, Arguments... ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<void*>::GetDescriptor(), (Detail::TypeDescriptorBuilder<Arguments>::GetDescriptor())..., Detail::nullDescriptor };

		_owner.RegisterValueTypeConstructor( _token, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(constructor) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Result>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeValueCast( Result (ETScriptAPICall* castFunction)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeValueCast( _token, Detail::TypeDescriptorBuilder<Result>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(castFunction) );

		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Result>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeValueCast( Result (ETScriptAPICall* castFunction)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeValueCast( _token, Detail::TypeDescriptorBuilder<Result>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(castFunction) );

		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeProperty( const char* const name, Property RealClass::* propertyPointer ) {
		_owner.RegisterValueTypeProperty( _token, name, Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), reinterpret_cast<char*>(&(reinterpret_cast<RealClass*>(1)->*propertyPointer)) - reinterpret_cast<char*>(reinterpret_cast<RealClass*>(1)) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, Property (ETScriptAPICall* getterFunction)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(getterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, Property (ETScriptAPICall* getterFunction)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(getterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, void (ETScriptAPICall* setterFunction)( RealClass*, Property ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(setterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Property>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeVirtualProperty( const char* const name, void (ETScriptAPICall* setterFunction)( const RealClass*, Property ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<Property>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeVirtualProperty( _token, name, Detail::TypeDescriptorBuilder<void>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(setterFunction) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename... Arguments>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( RealClass*, Arguments... ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), (Detail::TypeDescriptorBuilder<Arguments>::GetDescriptor())..., Detail::nullDescriptor };

		_owner.RegisterValueTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename... Arguments>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeMethod( const char* const name, Return (ETScriptAPICall* method)( const RealClass*, Arguments... ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), (Detail::TypeDescriptorBuilder<Arguments>::GetDescriptor())..., Detail::nullDescriptor };

		_owner.RegisterValueTypeMethod( _token, name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeUnaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass* ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeUnaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename RightHandOperandType>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass*, RightHandOperandType ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<RightHandOperandType>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeBinaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	template <typename Return, typename RightHandOperandType>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>& ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass>::ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass*, RightHandOperandType ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::TypeDescriptorBuilder<const RealClass*>::GetDescriptor(), Detail::TypeDescriptorBuilder<RightHandOperandType>::GetDescriptor(), Detail::nullDescriptor };

		_owner.RegisterValueTypeBinaryOperator( _token, operatorClass, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );
		return *this;
	}

// ---------------------------------------------------

	template <typename RealEnum>
	ScriptApiRegistrationInitializationVisitor::EnumTypeBuilder<RealEnum> ScriptApiRegistrationInitializationVisitor::BeginEnumTypeRegistration() {
		return { *this, RegisterEnumType( Scripting::TypeStringGenerator<RealEnum>::GetTypeName() ) };
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ReferenceTypeBuilder<RealClass> ScriptApiRegistrationInitializationVisitor::BeginReferenceTypeRegistration() {
		return { *this, RegisterReferenceType( Scripting::TypeStringGenerator<RealClass>::GetTypeName() ) };
	}

// ---------------------------------------------------

	template <typename RealClass>
	ScriptApiRegistrationInitializationVisitor::ValueTypeBuilder<RealClass> ScriptApiRegistrationInitializationVisitor::BeginValueTypeRegistration() {
		return { *this, RegisterValueType( Scripting::TypeStringGenerator<RealClass>::GetTypeName(), sizeof(RealClass) ) };
	}

// ---------------------------------------------------

	template <typename Return>
	ScriptApiRegistrationInitializationVisitor& ScriptApiRegistrationInitializationVisitor::ExposeFunction( const char* const name, Return (ETScriptAPICall* method)() ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { Detail::nullDescriptor };

		RegisterFunction( name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );

		return *this;
	}
	
// ---------------------------------------------------

	template <typename Return, typename... Arguments>
	ScriptApiRegistrationInitializationVisitor& ScriptApiRegistrationInitializationVisitor::ExposeFunction( const char* const name, Return(ETScriptAPICall* method)( Arguments... ) ) {
		static const DataTypeDescriptor	argumentDescriptors[] = { (Detail::TypeDescriptorBuilder<Arguments>::GetDescriptor())..., Detail::nullDescriptor };

		RegisterFunction( name, Detail::TypeDescriptorBuilder<Return>::GetDescriptor(), argumentDescriptors, reinterpret_cast<UntypedNativeFunctionPointer>(method) );

		return *this;
	}

// ---------------------------------------------------

	template <typename RealClass>
	void ScriptApiRegistrationInitializationVisitor::EnsureReferenceTypeDeclaredToScript() {
		this->DeclareReferenceType( Scripting::TypeStringGenerator<RealClass>::GetTypeName() );
	}

// ---------------------------------------------------

	template <typename RealClass>
	void ScriptApiRegistrationInitializationVisitor::EnsureValueTypeDeclaredToScript() {
		this->DeclareValueType( Scripting::TypeStringGenerator<RealClass>::GetTypeName(), sizeof(RealClass) );
	}

}	// namespace Scripting
}	// namespace Eldritch2