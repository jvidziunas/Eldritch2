/*==================================================================*\
  WorldService.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AssetViews/ObjectGraphView.hpp>
#include <Scripting/Angelscript/WorldService.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ApiRegistrar.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Core/World.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	WorldService::WorldService( const World& owningWorld ) : Core::WorldService( owningWorld.GetServiceBlackboard() ),
															 _log( owningWorld.GetLog() ),
															 _rulesName( { owningWorld.GetAllocator(), "AngelScript World Service Rules Type Name Allocator" } ),
															 _baseWorldAssetName( { owningWorld.GetAllocator(), "AngelScript World Service Base Asset Name Allocator" } ),
															 _scriptEngine( LocateService<asIScriptEngine>() ),
															 _assetLibrary( LocateService<AssetLibrary>() ) {}

// ---------------------------------------------------

	Utf8Literal WorldService::GetName() const {
		return "Angelscript World Service";
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, const GameStartVisitor ) {
		MICROPROFILE_SCOPEI( "Angelscript Virtual Machine", "Process game start", 0xCDCDCD );

		if( !_rulesName.IsEmpty() ) {
			auto	entity( Spawn( _rulesName ) );
			if( !entity ) {
				_log( MessageSeverity::Error, "Unable to create rules entity '{}'!" ET_UTF8_NEWLINE_LITERAL, _rulesName.AsCString() );
				LocateService<World>().ShutDown();
				return;
			}
		}

		if( !_baseWorldAssetName.IsEmpty() ) {
			const auto	asset( _assetLibrary.ResolveView<AssetViews::ObjectGraphView>( _baseWorldAssetName ) );
			if( !asset ) {
				_log( MessageSeverity::Error, "World requires asset '{}', but it was unavailable." ET_UTF8_NEWLINE_LITERAL, _baseWorldAssetName.AsCString() );
				LocateService<World>().ShutDown();
				return;
			}

			asset->InstantiateInto( *this );
		}
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*executor*/, const GameStopVisitor ) {
	//	Nothing here for now.
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& executor, FixedTickVisitor& visitor ) {
		visitor.world.VisitServices( executor, PreScriptTickVisitor() );
		visitor.world.VisitServices( executor, ScriptTickVisitor() );
		visitor.world.VisitServices( executor, PostScriptTickVisitor() );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*executor*/, const ScriptTickVisitor ) {
		MICROPROFILE_SCOPEI( GetName(), "Process script messages", 0xCDCDCD );
	}

// ---------------------------------------------------

	void WorldService::RegisterScriptApi( ApiRegistrar& visitor ) {
#if 0
		visitor.ExposeFunction<void, const Utf8String<>&>( "Spawn", [] ( const Utf8String<>& typePath ) -> void {
				World::LocateScriptService<AngelScript::WorldService>().Spawn( typePath, nullptr );
			} ).ExposeFunction<void, const Utf8String<>&>( "ShutDown", [] ( const Utf8String<>& reason ) {
				World::LocateScriptService<AngelScript::WorldService>()._log( MessageSeverity::Message, "Script has requested world shutdown: {}." ET_UTF8_NEWLINE_LITERAL, reason );
				World::LocateScriptService<World>().ShutDown();
			} );
#endif
	}

// ---------------------------------------------------

	UniquePointer<asIScriptObject> WorldService::Spawn( const Utf8Char* const qualifiedTypePath, const asITypeInfo* requiredInterface ) {
		FixedStackAllocator<64u>	allocator( "WorldService::Spawn() Temporary Allocator" );

		const auto	typeName( FindLastInstance( qualifiedTypePath, '/' ) );
		if( nullptr == typeName ) {
			return { nullptr };
		}

	//	Get a null-terminated module name.
		const Utf8String<>		moduleName( qualifiedTypePath, typeName, { allocator, "Class Name Allocator" } );
		const asIScriptModule*	module( _scriptEngine.GetModule( moduleName, asEGMFlags::asGM_ONLY_IF_EXISTS ) );
		const asITypeInfo*		type( module->GetTypeInfoByName( typeName + 1u ) );

		if( !type || (requiredInterface && !type->Implements( requiredInterface ) ) ) {
			return nullptr;
		}

		return static_cast<asIScriptObject*>(_scriptEngine.CreateScriptObject( type ));
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2