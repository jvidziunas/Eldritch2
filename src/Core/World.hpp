/*==================================================================*\
  World.hpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/TimeAccumulator.hpp>
#include <Core/WorldComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}

	namespace Assets {
		class	Package;
	}
}

namespace Eldritch2 {
namespace Core {

	class World {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class ManagementComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//! Disable copy construction.
			ManagementComponent( const ManagementComponent& ) = delete;
		//! Constructs this @ref ManagementComponent instance.
			ManagementComponent( const World& owner );
			
			~ManagementComponent() = default;

		// ---------------------------------------------------

		public:
			void	AcceptVisitor( Scripting::Wren::ApiBuilder& api ) override;
		};

	// ---

	public:
		template <typename Value>
		using ComponentList	= ArrayList<UniquePointer<Value>>;

		template <typename Value>
		using PackageList	= ArrayList<CountedPointer<Value>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref World instance.
		World( const Blackboard& services, Logging::Log& log );
	//! Disable copy construction.
		World( const World& ) = delete;

		~World() = default;

	// - STATE INSPECTION --------------------------------
		
	public:
		const ComponentList<WorldComponent>&	GetComponents() const;

		const Blackboard&						GetServices() const;

		Logging::Log&							GetLog() const;

	// ---------------------------------------------------

	public:
		template <typename Iterator>
		ErrorCode	BindResources( Scheduling::JobExecutor& executor, Iterator firstComponent, Iterator lastComponent );
		ErrorCode	BindResources( Scheduling::JobExecutor& executor );

		void		FreeResources( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	public:
		void	Tick( Scheduling::JobExecutor& executor );

	// - WORLD CONTENT MANAGEMENT ------------------------

	public:
		ErrorCode	Import( const Utf8Char* const path );

	// ---------------------------------------------------

	public:
		float32	GetTimeScalar() const;

		void	SetTimeScalar( float32 scalar );

	// ---------------------------------------------------

	public:
		bool	ShouldShutDown( std::memory_order order = std::memory_order_consume ) const;

		bool	IsRunning( std::memory_order order = std::memory_order_consume ) const;

	// ---------------------------------------------------

	public:
	//!	Notifies the world that it should cease updating as soon as possible.
	/*!	@param[in] andEngine boolean option indicating whether or not a shutdown request should be
			placed for the owning @ref Engine as well.
		@remarks Idempotent, thread-safe. */
		void	SetShouldShutDown( bool andEngine = false ) const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		World&	operator=( const World& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Mutable as objects created using the allocator do not (directly) contribute to this @ref World's state.
		mutable MallocAllocator				_allocator;
	//!	Mutable so logs can be written even if we only have a const reference to the world.
		mutable Logging::ChildLog			_log;
	//!	Mutable so termination can be requested even if we only have a const reference to the world.
		mutable Atomic<bool>				_shouldShutDown;

		Blackboard							_services;
		TimeAccumulator						_timeAccumulator;

		mutable Mutex						_importsMutex;
	//!	List of garbage collection roots for this world.
		PackageList<const Assets::Package>	_imports;
	//!	Collection of external modules that operate at a per-world scope.
		ComponentList<WorldComponent>		_components;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/World.inl>
//------------------------------------------------------------------//