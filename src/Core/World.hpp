/*==================================================================*\
  World.hpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/HashMultiMap.hpp>
#include <Utility/Containers/ArraySet.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Core/ServiceBlackboard.hpp>
#include <Utility/Mpl/FloatTypes.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/UniquePointer.hpp>
#include <Core/WorldService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobFiber;
	}

	namespace Assets {
		enum class	ResidencyState;
		class		AssetPackage;
		class		LiveSet;
	}

	namespace Core {
		class	Engine;
	}
}

namespace Eldritch2 {
namespace Core {

	class World {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using PropertySetter	= std::function<void (Eldritch2::Range<const Eldritch2::Utf8Char*>)>;

	// ---

		struct GameStartVisitor {};
		struct TickVisitor {};
		struct GameStopVisitor {};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref World instance.
		World( const Core::Engine& owningEngine );
	//! Disable copying.
		World( const World& ) = delete;

		~World() = default;

	// ---------------------------------------------------

	public:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStartVisitor );
		void	AcceptVisitor( Scheduling::JobFiber& executor, const TickVisitor );
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStopVisitor );
		void	AcceptVisitor( Assets::LiveSet& livePackageSet );

	// ---------------------------------------------------

	public:
		template <typename... Arguments>
		void	VisitServices( Scheduling::JobFiber& executor, Arguments&&... arguments );
		template <typename... Arguments>
		void	VisitServices( Arguments&&... arguments );

	// ---------------------------------------------------

	public:
		void	AttachAndInitializeWith( Eldritch2::UniquePointer<Eldritch2::UniquePointer<Core::WorldService>[]> services );

	// - STATE INSPECTION --------------------------------
		
	public:
		const Core::ServiceBlackboard&	GetServiceBlackboard() const;

		Eldritch2::Allocator&			GetAllocator() const;

		Logging::Log&					GetLog() const;

	// ---------------------------------------------------

	public:
		void	AddReference( const Assets::AssetPackage& package );

	// ---------------------------------------------------

	public:
		bool	HasShutDown() const;

		bool	IsRunningGame() const;

		bool	IsPaused() const;

	// ---------------------------------------------------

	public:
		void	ShutDown() const;

	// ---------------------------------------------------

	public:
		void	SetProperties( std::initializer_list<Eldritch2::Pair<const Eldritch2::Utf8Char*, const Eldritch2::Utf8Char*>> properties );

		void	SetProperty( const Eldritch2::Utf8Char* name, const Eldritch2::Utf8Char* value );

	// ---------------------------------------------------

	public:
		static ETPureFunctionHint Eldritch2::Utf8Literal	GetRequiredPackagesKey();

		static ETPureFunctionHint Eldritch2::Utf8Literal	GetFixedTickFramerateKey();

		static ETPureFunctionHint Eldritch2::Utf8Literal	GetTimeScaleKey();

		static ETPureFunctionHint Eldritch2::Utf8Literal	GetRulesKey();

	// ---------------------------------------------------

	//!	Disable assignment.
		World&	operator=( const World& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//	Mutable as objects created using the allocator do not (directly) contribute to this @ref World's state.
		mutable Eldritch2::ChildAllocator											_allocator;
	//!	Mutable so logs can be written even if we only have a const reference to the world.
		mutable Logging::ChildLog													_log;

	//!	Mutable so termination can be requested even if we only have a const reference to the world.
		mutable std::atomic<bool>													_hasShutDown;

		std::atomic<bool>															_isPaused;

	/*!	World-scope service blackboard. This will copy the contents of the global/application-scope blackboard initially,
		but world services can register their own distinct per-world services during initialization. */
		Core::ServiceBlackboard														_serviceBlackboard;

		mutable Platform::UserReadWriteMutex										_referencedPackageMutex;
		Eldritch2::ArraySet<const Assets::AssetPackage*>							_referencedPackages;
		Eldritch2::ArraySet<const Assets::AssetPackage*>							_remainingLoads;

		Eldritch2::HashMultiMap<Eldritch2::Utf8Literal, PropertySetter>				_propertySetters;

		Eldritch2::uint64															_gameTime;

		Eldritch2::uint64															_lastFixedTickRealTime;
		Eldritch2::uint64															_nextFixedTickRealTime;

		Eldritch2::float32															_inverseTimeScale;
		Eldritch2::uint32															_timeStep;

		Eldritch2::UniquePointer<Eldritch2::UniquePointer<Core::WorldService>[]>	_services;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/World.inl>
//------------------------------------------------------------------//