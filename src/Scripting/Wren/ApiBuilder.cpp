/*==================================================================*\
  ApiBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace Detail {
namespace {

	ETInlineHint ETPureFunctionHint Method AsWrenMethod( const ForeignMethod& foreignMethod ) {
		Method	method;

		method.type			= METHOD_FOREIGN;
		method.fn.foreign	= foreignMethod.body;

		return method;
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Method AsWrenMethod( void (*finalizer)( void* ) ) {
		Method	method;

		method.type			= METHOD_FOREIGN;
		method.fn.foreign	= reinterpret_cast<WrenForeignMethodFn>(finalizer);

		return method;
	}

}	// anonymous namespace

	ETPureFunctionHint size_t HashForeignMethod( const char* const module, const char* const className, bool isStatic, const char* const signature, size_t seed ) {
		return HashMemory( signature, StringLength( signature ), HashForeignClass( module, className, seed + (isStatic ? 0u : 31u) ) );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t HashForeignClass( const char* const module, const char* const className, size_t seed ) {
		return HashMemory( module, StringLength( module ), HashMemory( className, StringLength( className ), seed ) );
	}

// ---------------------------------------------------

	void BindFinalizer( WrenVM* vm, WrenHandle* target, void (*finalizer)( void* ) ) {
		static const char	finalizerSymbol[] = "<finalizer>";
		wrenBindMethod(
			vm,
			AS_CLASS( target->value ),
			wrenSymbolTableEnsure( vm, &vm->methodNames, finalizerSymbol, _countof(finalizerSymbol) ),
			AsWrenMethod( finalizer )
		);
	}

// ---------------------------------------------------

	void Bind( WrenVM* vm, WrenHandle* target, const ForeignMethod& foreignMethod ) {
		wrenBindMethod(
			vm,
			foreignMethod.isStatic ? AS_CLASS( target->value )->obj.classObj : AS_CLASS( target->value ),
			wrenSymbolTableEnsure( vm, &vm->methodNames, foreignMethod.signature, StringLength( foreignMethod.signature ) ),
			AsWrenMethod( foreignMethod )
		);
	}

}	// namespace Detail

	ForeignMethod::ForeignMethod(
		Utf8Literal name,
		Utf8Literal argumentSpecifier,
		Body body,
		bool isStatic
	) : body( body ),
		isStatic( isStatic ) {
		ET_ASSERT( body != nullptr, "Wren foreign method must have a body!" );

		AppendString( CopyString( signature, name ), argumentSpecifier );
	}

// ---------------------------------------------------

	ApiBuilder::ApiBuilder( Context& wren ) : _context( &wren ) {
		ET_ASSERT( wren.GetVm() != nullptr, "Wren VM must be initialized before API construction!" );
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2