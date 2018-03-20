/*==================================================================*\
  Context.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//


namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace {

	ETPureFunctionHint ETInlineHint bool Succeeded( WrenInterpretResult result ) {
		return result == WREN_RESULT_SUCCESS;
	}

// ---------------------------------------------------

	ETInlineHint ObjModule* CreateModule( WrenVM* vm, ObjString* name ) {
		ObjModule* const module( wrenNewModule( vm, name ) );

		wrenPushRoot( vm, reinterpret_cast<Obj*>(module) );
			wrenMapSet( vm, vm->modules, OBJ_VAL( name ), OBJ_VAL( module ) );
		wrenPopRoot( vm );

		return module;
	}

// ---------------------------------------------------

	ETInlineHint ObjModule* GetModule( WrenVM* vm, const char* moduleName ) {
		ObjString* const name( AS_STRING( wrenNewString( vm, moduleName ) ) );

		wrenPushRoot( vm, reinterpret_cast<Obj*>(name) );
			const Value	candidate( wrenMapGet( vm->modules, OBJ_VAL( name ) ) );
			ObjModule*	module( candidate != UNDEFINED_VAL ? AS_MODULE(candidate) : CreateModule( vm, name ));
		wrenPopRoot( vm );

		return module;
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	Context::Context(
		const AssetDatabase& assets,
		Log& log
	) : _allocator( "Wren Context Root Allocator" ),
		_log( log ),
		_assets( &assets ),
		_classesByType( MallocAllocator( "Wren Class By Type Allocator" ) ),
		_vm( nullptr ),
		_unaryCallStub( nullptr ) {
	}

// ---------------------------------------------------

	Context::~Context() {
		FreeResources();
	}

// ---------------------------------------------------

	WrenHandle*	Context::CreateForeignClass( const char* module, const char* name, Type type ) {
		ObjString* const wrenName( AS_STRING( wrenNewString( _vm, name ) ) );
		wrenPushRoot( _vm, reinterpret_cast<Obj*>(wrenName) );

		const Value wrenClass( OBJ_VAL( wrenNewClass( _vm, _vm->objectClass, -1, wrenName ) ) );
		const int	addResult( wrenDefineVariable( _vm, GetModule( _vm, module ), name, StringLength( name ), wrenClass ) );

		ET_ASSERT( addResult != -1, "Duplicate Wren class registration!" );
		ET_ASSERT( addResult != -2, "Error registering Wren class object with module!" );

		WrenHandle* klass( wrenMakeHandle( _vm, OBJ_VAL( wrenClass ) ) );
	//	Pop Wren class name.
		wrenPopRoot( _vm );

		_classesByType.Emplace( type, klass );

		return klass;
	}

// ---------------------------------------------------

	void* Context::CreateVariable( const char* module, const char* name, WrenHandle* klass, size_t size ) {
		ObjForeign* const variable( wrenNewForeign( _vm, AS_CLASS( klass->value ), size ) );

		wrenDefineVariable( _vm, GetModule( _vm, module ), name, StringLength( name ), OBJ_VAL( variable ) );

		return variable->data;
	}

// ---------------------------------------------------

	void Context::BindResources( size_t heapSizeInBytes ) {
		WrenConfiguration	configuration;

		wrenInitConfiguration( &configuration );

		configuration.userData            = this;
		configuration.bindForeignClassFn  = nullptr;
		configuration.bindForeignMethodFn = nullptr;
		configuration.initialHeapSize     = heapSizeInBytes;
		configuration.minHeapSize         = heapSizeInBytes;
		configuration.reallocateFn        = [] ( void* memory, size_t newSize ) {
			return _aligned_realloc( memory, newSize, 16u );
		};

		configuration.loadModuleFn = [] ( WrenVM* vm, const char* module ) -> char* {
			fmt::memory_buffer	path;
			fmt::format_to( path, "{}{}", module, static_cast<const Utf8Char*>(ScriptAsset::GetExtension()) );
			path.push_back( '\0' );

			const ScriptAsset* const script( AsContext( vm )._assets->Require<ScriptAsset>( path.data() ) );
			if (!script) {
				return nullptr;
			}

			char* const source( static_cast<char*>(wrenReallocate( vm, nullptr, 0u, script->GetLength() )) );
			if (source) {
				Copy( script->Begin(), script->End(), source );
			}

			return source;
		};

		configuration.errorFn = [] ( WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message ) {
			switch (type) {
				case WREN_ERROR_COMPILE:     AsContext( vm ).WriteLog( MessageType::Error, "{} ({}:{})" UTF8_NEWLINE, message, module, line ); break;
				case WREN_ERROR_RUNTIME:     AsContext( vm ).WriteLog( MessageType::Error, "Wren runtime error: {}" UTF8_NEWLINE, message ); break;
				case WREN_ERROR_STACK_TRACE: AsContext( vm ).WriteLog( MessageType::Error, "\tin {} ({}.wren:{})" UTF8_NEWLINE, message, module, line ); break;
			}	// switch (type)
		};

		configuration.writeFn = [] ( WrenVM* vm, const char* text ) {
			AsContext( vm ).WriteLog( MessageType::Message, "{}" UTF8_NEWLINE, text );
		};

		_vm				= wrenNewVM( &configuration );
		_unaryCallStub	= wrenMakeCallHandle( _vm, "call(_)" );
	}

// ---------------------------------------------------

	void Context::FreeResources() {
		if (WrenVM* const vm = eastl::exchange( _vm, nullptr )) {
			_classesByType.ClearAndDispose( [vm] ( const Pair<Type, WrenHandle*>& entry ) {
				wrenReleaseHandle( vm, entry.second );
			} );

			wrenReleaseHandle( vm, eastl::exchange( _unaryCallStub, nullptr ) );
			wrenFreeVM( vm );
		}
	}

// ---------------------------------------------------

	bool Context::Interpret( const char* code ) {
		return Succeeded( wrenInterpret( GetVm(), code ) );
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2