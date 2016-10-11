/*==================================================================*\
  World.cpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Math/StandardLibrary.hpp>
#include <Platform/PlatformInterface.hpp>
#include <Assets/AssetPackage.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Assets/LiveSet.hpp>
#include <Utility/Assert.hpp>
#include <Core/Engine.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Core {
namespace {

	enum : uint32 {
		MicrosecondsPerSecond = 1000000u
	};

// ---

	static ETInlineHint ETPureFunctionHint uint32 TimestepFromFramerate( uint32 framerateInHz ) {
		return MicrosecondsPerSecond / framerateInHz;
	}

}	// anonymous namespace

	World::World( const Engine& engine ) : _allocator( engine.GetAllocator(), "World Allocator" ),
										   _log( engine.GetLog() ),
										   _serviceBlackboard( engine.GetServiceBlackboard(), engine.GetAllocator() ),
										   _referencedPackages( { engine.GetAllocator(), "World Referenced Package Collection Allocator" } ),
										   _remainingLoads( { engine.GetAllocator(), "World Remaining Package Load Collection Allocator" } ),
										   _propertySetters( { engine.GetAllocator(), "World Property Setter Directory Allocator" } ),
										   _gameTime( 0u ),
										   _lastFixedTickRealTime( 0u ),
										   _nextFixedTickRealTime( eastl::numeric_limits<uint64>::max() ),
										   _inverseTimeScale( 1.0f ),
										   _timeStep( TimestepFromFramerate( 60u ) ),
										   _isPaused( true ),
										   _hasShutDown( false ),
										   _services( nullptr ) {
	//	Add the core property handlers.
		_propertySetters.Emplace( GetFixedTickFramerateKey(), [this] ( Range<const Utf8Char*> value ) {
			boost::iostreams::basic_array_source<Utf8Char>	source( value.Begin(), value.End() );
			boost::iostreams::stream<decltype( source )>	inStream( source );

			int32	frameRate;
			inStream >> frameRate;

			if( frameRate == 0 || frameRate > MicrosecondsPerSecond ) {
				_log( MessageSeverity::Warning, "Tried to set invalid fixed-tick frame rate value ({}) for world {}, ignoring." ET_UTF8_NEWLINE_LITERAL, frameRate, static_cast<void*>(this) );
				return;
			}

			_timeStep = TimestepFromFramerate( frameRate );
		} );

		_propertySetters.Emplace( GetTimeScaleKey(), [this] ( Range<const Utf8Char*> value ) {
			boost::iostreams::basic_array_source<Utf8Char>	source( value.Begin(), value.End() );
			boost::iostreams::stream<decltype( source )>	inStream( source );

			float32	timescale;
			inStream >> timescale;

			if( timescale <= 0.0f || timescale > 100.0f ) {
				_log( MessageSeverity::Warning, "Tried to set invalid time scaling value ({}) for world {}, ignoring." ET_UTF8_NEWLINE_LITERAL, timescale, static_cast<void*>(this) );
				return;
			}

			_inverseTimeScale = 1.0f / timescale;
		} );
	}

// ---------------------------------------------------

	void World::AcceptVisitor( Scheduling::JobFiber& executor, const GameStartVisitor ) {
		if( _gameTime != 0 ) {
			return;
		}

		_log( MessageSeverity::Message, "Beginning play for world {}.", static_cast<void*>(this) );

		VisitServices( executor, WorldService::GameStartVisitor() );
	}

// ---------------------------------------------------

	void World::AcceptVisitor( JobFiber& executor, const TickVisitor ) {
		enum : size_t {
			TickVisitorArenaSizeInBytes	= 16384u
		};

	// ---

		{	ScopedWriteLock	_( _referencedPackageMutex );
			_remainingLoads.RemoveIf( [this] ( const AssetPackage* const package ) -> bool {
				switch( package->GetResidencyState() ) {
					case ResidencyState::Loading:	return false;
					case ResidencyState::Published: return true;
					case ResidencyState::Failed:	ShutDown();  return true;
				}	// switch( package->GetResidencyState() )

			//	Unreachable code.
				ETRuntimeAssert( false );
				return false;
			} );

		//	Don't tick the world if we're missing content.
			if( !_remainingLoads.IsEmpty() ) {
				return;
			}
		}	// End of lock scope.

	//	Check if a view requested termination, and tear down if so.
		if( ETBranchUnlikelyHint( HasShutDown() ) ) {
			AcceptVisitor( executor, GameStopVisitor() );
			return;
		}

		const uint64	currentTimestamp( _serviceBlackboard.Locate<PlatformInterface>().ReadTimestampCounter() );

	//	We execute a variable-rate tick exactly once per main loop iteration.
		VisitServices( executor, WorldService::VariableTickVisitor( *this, TickVisitorArenaSizeInBytes ) );

	//	Finally, execute fixed-rate ticks until we are 'caught up' with the real-time clock.
		while( _nextFixedTickRealTime < currentTimestamp ) {
			VisitServices( executor, WorldService::FixedTickVisitor( *this, TickVisitorArenaSizeInBytes ) );

			_gameTime += _timeStep;
			_lastFixedTickRealTime = _nextFixedTickRealTime;
			_nextFixedTickRealTime += IntFromFloat( _inverseTimeScale * _timeStep );
		}
	}

// ---------------------------------------------------

	void World::AcceptVisitor( JobFiber& executor, const GameStopVisitor ) {
		_log( MessageSeverity::Message, "Stopping play for world {}.", static_cast<void*>(this) );

		VisitServices( executor, WorldService::GameStopVisitor() );
	}

// ---------------------------------------------------

	void World::AcceptVisitor( LiveSet& livePackageSet ) {
		for( const AssetPackage* package : _referencedPackages ) {
			livePackageSet.AddPackageAndAllDependencies( *package );
		}
	}

// ---------------------------------------------------

	void World::AttachAndInitializeWith( UniquePointer<UniquePointer<WorldService>[]> services ) {
	//	Ensure we are not trying to initialize more than once.
		ETRuntimeAssert( !_services );

		_services = eastl::move( services );
	}

// ---------------------------------------------------

	void World::AddReference( const AssetPackage& package ) {
		ScopedWriteLock	_( _referencedPackageMutex );

		if( ResidencyState::Published != package.GetResidencyState() ) {
			_remainingLoads.Insert( &package );
		}

		_referencedPackages.Insert( &package );
	}

// ---------------------------------------------------

	void World::SetProperty( const Utf8Char* name, const Utf8Char* value ) {
		Range<const Utf8Char*>	valueRange( value, FindEndOfString( value ) );

		for( const auto& handler : _propertySetters.EqualRange( Utf8Literal( name ) ) ) {
			handler.second( valueRange );
		}
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal World::GetRequiredPackagesKey() {
		return "RequiredPackages";
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal World::GetFixedTickFramerateKey() {
		return "TicksPerSecond";
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal World::GetTimeScaleKey() {
		return "TimeScale";
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal World::GetRulesKey() {
		return "Rules";
	}

}	// namespace Core
}	// namespace Eldritch2