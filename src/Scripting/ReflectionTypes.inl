/*==================================================================*\
  ReflectionTypes.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace {

	template <typename T>
	struct SignatureBuilder {
	public:
		static TypeDescriptor Describe() {
			using Type = typename eastl::remove_reference<eastl::remove_pointer<T>::type>::type;

			return { typeid(Type), eastl::is_const<Type>::value, eastl::is_pointer<T>::value, eastl::is_reference<T>::value };
		}
	};

// ---------------------------------------------------

	template <typename Function>
	struct FunctionTraits {
		static_assert( eastl::is_function<eastl::remove_pointer<Function>>::value, "Type must be a function" );
	};

// ---

//	Specialization for functions.
	template <typename Return, typename... Arguments>
	struct FunctionTraits<Return (*)( Arguments... )> {
	public:
		static const Argument* DescribeArguments() {
			static const Argument	descriptors[] = { Argument( SignatureBuilder<Arguments>::Describe() )..., TypeDescriptor() };

			return descriptors;
		}

		static TypeDescriptor DescribeReturn() {
			return SignatureBuilder<Return>::Describe();
		}
	};

// ---

//	Specialization for member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (Class::*)( Arguments... )> : public FunctionTraits<Return (*)( Arguments... )>{};

// ---

//	Specialization for const member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (Class::*)( Arguments... ) const> : public FunctionTraits<Return (Class::*)( Arguments... )>{};

#if ET_COMPILER_IS_MSVC
//	Specialization for SIMDCall functions.
	template <typename Return, typename... Arguments>
	struct FunctionTraits<Return (ETSimdCall*)( Arguments... )> : public FunctionTraits<Return (*)( Arguments... )> {};

// ---

//	Specialization for SIMDCall member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (ETSimdCall Class::*)( Arguments... )> : public FunctionTraits<Return (Class::*)( Arguments... )>{};

// ---

//	Specialization for SIMDCall const member functions.
	template <typename Return, class Class, typename... Arguments>
	struct FunctionTraits<Return (ETSimdCall Class::*)( Arguments... ) const> : public FunctionTraits<Return (Class::*)( Arguments... ) const>{};
#endif	// if ET_COMPILER_IS_MSVC

// ---------------------------------------------------

	template <typename T>
	struct FieldTraits {};

	template <typename Class, typename Field>
	struct FieldTraits<Field Class::*> {
	public:
		using ClassType	= Class;
		using FieldType	= Field;
	};

// ---------------------------------------------------

	template <typename Function>
	struct StripInjected {
	public:
		using Type = Function;
	};

// ---

	template <typename Return, typename... InjectedArguments, typename... Arguments>
	struct StripInjected<Return (*)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (*)( Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct StripInjected<Return (Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (Class::*)( Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct StripInjected<Return (Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... ) const> {
	public:
		using Type = Return (Class::*)( Arguments... ) const;
	};

#if ET_COMPILER_IS_MSVC
	template <typename Return, typename... InjectedArguments, typename... Arguments>
	struct StripInjected<Return (ETSimdCall *)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (ETSimdCall*)( Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct StripInjected<Return (ETSimdCall Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (ETSimdCall Class::*)( Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct StripInjected<Return (ETSimdCall Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... ) const> {
	public:
		using Type = Return (ETSimdCall Class::*)( Arguments... ) const;
	};
#endif	// if ET_COMPILER_IS_MSVC

// ---------------------------------------------------

	template <typename Function>
	struct StripConstructorLocation {
	public:
		using Type = Function;
	};

// ---

	template <typename... Arguments>
	struct StripConstructorLocation<void (*)( void*, Arguments... )> {
	public:
		using Type = void (*)( Arguments... );
	};

// ---

#if ET_COMPILER_IS_MSVC
	template <typename... Arguments>
	struct StripConstructorLocation<void (ETSimdCall *)( void*, Arguments... )> {
	public:
		using Type = void (ETSimdCall*)( Arguments... );
	};
#endif	// if ET_COMPILER_IS_MSVC

}	// anonymous namespace

namespace Detail {

	template <typename Return, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (*)( void*, Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (*)( void*, InjectedArguments..., Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (Class::*)( void*, Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (Class::*)( void*, InjectedArguments..., Arguments... );
	};

// ---

	template <typename Return, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (*)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (*)( InjectedArguments..., Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (Class::*)( InjectedArguments..., Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... ) const> {
	public:
		using Type = Return (Class::*)( InjectedArguments..., Arguments... ) const;
	};

// ---

//	MSVC seems to really struggle expanding empty parameter packs, so we work around that via manual specialization.
	template <typename Return, typename... InjectedArguments>
	struct FlattenInjected<Return (*)( void*, Scripting::Injected<InjectedArguments...> )> {
	public:
		using Type = Return (*)( void*, InjectedArguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments>
	struct FlattenInjected<Return (Class::*)( void*, Scripting::Injected<InjectedArguments...> )> {
	public:
		using Type = Return (Class::*)( void*, InjectedArguments... );
	};

// ---

	template <typename Return, typename... InjectedArguments>
	struct FlattenInjected<Return (*)( Scripting::Injected<InjectedArguments...>)> {
	public:
		using Type = Return (*)( InjectedArguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments>
	struct FlattenInjected<Return (Class::*)( Scripting::Injected<InjectedArguments...> )> {
	public:
		using Type = Return (Class::*)( InjectedArguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments>
	struct FlattenInjected<Return (Class::*)( Scripting::Injected<InjectedArguments...> ) const> {
	public:
		using Type = Return (Class::*)( InjectedArguments... ) const;
	};

#if ET_COMPILER_IS_MSVC
	template <typename Return, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (ETSimdCall *)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (ETSimdCall*)( InjectedArguments..., Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (ETSimdCall Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... )> {
	public:
		using Type = Return (ETSimdCall Class::*)( InjectedArguments..., Arguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments, typename... Arguments>
	struct FlattenInjected<Return (ETSimdCall Class::*)( Scripting::Injected<InjectedArguments...>, Arguments... ) const> {
	public:
		using Type = Return (ETSimdCall Class::*)( InjectedArguments..., Arguments... ) const;
	};

// ---

	template <typename Return, typename... InjectedArguments>
	struct FlattenInjected<Return (ETSimdCall *)( Scripting::Injected<InjectedArguments...> )> {
	public:
		using Type = Return (ETSimdCall*)( InjectedArguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments>
	struct FlattenInjected<Return (ETSimdCall Class::*)( Scripting::Injected<InjectedArguments...> )> {
	public:
		using Type = Return (ETSimdCall Class::*)( InjectedArguments... );
	};

// ---

	template <typename Return, class Class, typename... InjectedArguments>
	struct FlattenInjected<Return (ETSimdCall Class::*)( Scripting::Injected<InjectedArguments...> ) const> {
	public:
		using Type = Return (ETSimdCall Class::*)( InjectedArguments... ) const;
	};
#endif	// if ET_COMPILER_IS_MSVC

}	// namespace Detail

	ETInlineHint TypeDescriptor::TypeDescriptor( Eldritch2::TypeHandle handle, bool isConst, bool isPointer, bool isReference ) : handle( handle ), isConst( isConst ), isPointer( isPointer ), isReference( isReference ) {}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator==( const TypeDescriptor& lhs, const TypeDescriptor& rhs ) {
		return 0 == Eldritch2::CompareMemory( &lhs, &rhs, sizeof(TypeDescriptor) );
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator!=( const TypeDescriptor& lhs, const TypeDescriptor& rhs ) {
		return !(lhs == rhs);
	}

// ---------------------------------------------------

	ETInlineHint Argument::Argument( Scripting::TypeDescriptor type, Eldritch2::Utf8Literal name ) : type( type ), name( name ) {}

// ---------------------------------------------------

	ETInlineHint Argument::Argument( Scripting::TypeDescriptor type ) : Argument( type, "" ) {}

// ---------------------------------------------------

	ETInlineHint Callable::Callable( Scripting::TypeDescriptor returnDescriptor, const Scripting::Argument* argumentDescriptors, Scripting::NativeFunction* nativeFunction ) : returnDescriptor( returnDescriptor ), argumentDescriptors( argumentDescriptors ), nativeFunction( nativeFunction ) {}

// ---------------------------------------------------

	ETInlineHint Constructor::Constructor( const Scripting::Argument* argumentDescriptors, Scripting::NativeFunction* nativeFunction ) : Callable( SignatureBuilder<void>::Describe(), argumentDescriptors, nativeFunction ) {}

// ---------------------------------------------------

	ETInlineHint Factory::Factory( Scripting::TypeDescriptor returnDescriptor, const Scripting::Argument* argumentDescriptors, Scripting::NativeFunction* nativeFunction ) : Callable( returnDescriptor, argumentDescriptors, nativeFunction ) {}

// ---------------------------------------------------

	ETInlineHint Function::Function( Scripting::TypeDescriptor returnDescriptor, Eldritch2::Utf8Literal name, const Scripting::Argument* argumentDescriptors, NativeFunction* nativeFunction ) : Callable( returnDescriptor, argumentDescriptors, nativeFunction ), name( name ) {}

// ---------------------------------------------------

	ETInlineHint Method::Method( Scripting::TypeDescriptor returnDescriptor, Eldritch2::Utf8Literal name, const Scripting::Argument* argumentDescriptors, bool isConst, NativeFunction* nativeFunction ) : Function( returnDescriptor, name, argumentDescriptors, nativeFunction ), isConst( isConst ) {}

// ---------------------------------------------------

	ETInlineHint Property::Property( Scripting::TypeDescriptor type, Eldritch2::Utf8Literal name, NativeFunction* getterFunction, NativeFunction* setterFunction ) : type( type ), name( name ), getterFunction( getterFunction ), setterFunction( setterFunction ) {}

// ---------------------------------------------------

	ETInlineHint Field::Field( Scripting::TypeDescriptor type, Eldritch2::Utf8Literal name, Eldritch2::ptrdiff offset ) : type( type ), name( name ), offset( offset ) {}

// ---------------------------------------------------

	template <typename Signature, typename FunctionPointer, FunctionPointer functionPointer>
	ETInlineHint Scripting::Constructor DefineConstructor_() {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, FunctionPointer>::value, "Signature mismatch" );

		using ScriptSignature = typename StripInjected<StripConstructorLocation<Signature>::Type>::Type;

		return { FunctionTraits<ScriptSignature>::DescribeArguments(), [] ( VirtualMachineContext& /*context*/ ) {} };
	}

// ---------------------------------------------------

	template <typename Signature, typename FunctionPointer, FunctionPointer functionPointer>
	ETInlineHint Scripting::Factory DefineFactory_() {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, FunctionPointer>::value, "Signature mismatch" );

		using ScriptSignature = typename StripInjected<Signature>::Type;

		return{ FunctionTraits<ScriptSignature>::DescribeReturn(), FunctionTraits<ScriptSignature>::DescribeArguments(), [] ( VirtualMachineContext& /*context*/ ) {} };
	}

// ---------------------------------------------------

	template <typename Signature, typename FunctionPointer, FunctionPointer functionPointer>
	ETInlineHint Scripting::Function DefineFunction_( Eldritch2::Utf8Literal name ) {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, FunctionPointer>::value, "Signature mismatch" );

		using ScriptSignature = typename StripInjected<Signature>::Type;
		
		return { FunctionTraits<ScriptSignature>::DescribeReturn(), name, FunctionTraits<ScriptSignature>::DescribeArguments(), [] ( VirtualMachineContext& /*context*/ ) {} };
	}

// ---------------------------------------------------

	template <typename Signature, typename MethodPointer, MethodPointer methodPointer>
	ETInlineHint Scripting::Method DefineMethod_( Eldritch2::Utf8Literal name ) {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, MethodPointer>::value, "Signature mismatch" );

		using ScriptSignature = typename StripInjected<Signature>::Type;

		return { FunctionTraits<ScriptSignature>::DescribeReturn(), name, FunctionTraits<MethodPointer>::DescribeArguments(), false, [] ( VirtualMachineContext& /*context*/ ) {} };
	}

// ---------------------------------------------------

	template <typename Signature, typename MethodPointer, MethodPointer methodPointer>
	ETInlineHint Scripting::Operator DefineUnaryOperator_( Scripting::OperatorType type ) {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, MethodPointer>::value, "Signature mismatch" );

		using ScriptSignature = typename StripInjected<Signature>::Type;

		return { FunctionTraits<ScriptSignature>::DescribeReturn(), type, FunctionTraits<MethodPointer>::DescribeArguments(), [] ( VirtualMachineContext& /*context*/ ) {} };
	}

// ---------------------------------------------------

	template <typename Signature, typename MethodPointer, MethodPointer methodPointer>
	ETInlineHint Scripting::Operator DefineBinaryOperator_( Scripting::OperatorType type ) {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, MethodPointer>::value, "Signature mismatch" );

		using ScriptSignature = typename StripInjected<Signature>::Type;

		return { FunctionTraits<ScriptSignature>::DescribeReturn(), type, FunctionTraits<MethodPointer>::DescribeArguments(), [] ( VirtualMachineContext& /*context*/ ) {} };
	}

// ---------------------------------------------------

	template <typename GetterSignature, typename GetterPointer, GetterPointer getterPointer, typename SetterSignature, typename SetterPointer, SetterPointer setterPointer>
	ETInlineHint Scripting::Property DefineProperty_( Eldritch2::Utf8Literal name ) {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<GetterSignature>::Type, GetterPointer>::value, "Signature mismatch" );
		static_assert( eastl::is_same<typename Detail::FlattenInjected<SetterSignature>::Type, SetterPointer>::value, "Signature mismatch" );

		using ScriptGetterSignature = typename StripInjected<GetterSignature>::Type;
		using ScriptSetterSignature = typename StripInjected<SetterSignature>::Type;

	//	static_assert( eastl::is_member_function_pointer<GetterPointer>::value, "Getter should reference a pointer to member function!" );
	//	static_assert( eastl::is_member_function_pointer<SetterPointer>::value, "Setter should reference a pointer to member function!" );

		return { FunctionTraits<ScriptGetterSignature>::DescribeReturn(), name, [] ( VirtualMachineContext& /*context*/ ) {}, [] ( VirtualMachineContext& /*context*/ ) {} };
	}
	
// ---------------------------------------------------

	template <typename Signature, typename GetterPointer, GetterPointer getterPointer>
	ETInlineHint Scripting::Property DefineGetter_( Eldritch2::Utf8Literal name ) {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, GetterPointer>::value, "Signature mismatch" );
	//	static_assert( eastl::is_member_function_pointer<GetterPointer>::value, "Template should reference a pointer to member function!" );
		using ScriptSignature = typename StripInjected<Signature>::Type;

		return { FunctionTraits<GetterPointer>::DescribeReturn(), name, [] ( VirtualMachineContext& /*context*/ ) {}, nullptr };
	}
	
// ---------------------------------------------------

	template <typename Signature, typename SetterPointer, SetterPointer setterPointer>
	ETInlineHint Scripting::Property DefineSetter_( Eldritch2::Utf8Literal name ) {
		static_assert( eastl::is_same<typename Detail::FlattenInjected<Signature>::Type, SetterPointer>::value, "Signature mismatch" );

	//	static_assert( eastl::is_member_function_pointer<SetterPointer>::value, "Template should reference a pointer to member function!" );
		using ScriptSignature = typename StripInjected<Signature>::Type;

		return{ Scripting::TypeDescriptor type, name, nullptr, [] ( VirtualMachineContext& /*context*/ ) {} };
	}

// ---------------------------------------------------

	template <typename FieldPointer, FieldPointer fieldPointer>
	ETInlineHint Scripting::Field DefineField_( Eldritch2::Utf8Literal name ) {
	//	static_assert( eastl::is_member_object_pointer<FieldPointer>::value, "Template should reference a pointer to member!" );
		using ClassType	= typename FieldTraits<FieldPointer>::ClassType;
		using FieldType = typename FieldTraits<FieldPointer>::FieldType;

		return { SignatureBuilder<FieldType>::Describe(), name, offsetof( ClassType, *fieldPointer ) };
	}

// ---------------------------------------------------

	template <typename Enum, Enum value>
	ETInlineHint Scripting::EnumValue DefineEnumValue_( Eldritch2::Utf8Literal name ) {
		return { name, reinterpret_cast<int>(value) };
	}

}	// namespace Scripting
}	// namespace Eldritch2