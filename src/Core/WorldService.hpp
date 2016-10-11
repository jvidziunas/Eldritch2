/*==================================================================*\
  WorldService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobFiber;
	}

	namespace Scripting {
		class	MessageBus;
	}

	namespace Core {
		class	ServiceBlackboard;
		class	World;
	}
}

namespace Eldritch2 {
namespace Core {

	class ETPureAbstractHint WorldService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct GameStartVisitor {};
		struct GameStopVisitor {};
		struct PreScriptTickVisitor {};
		struct ScriptTickVisitor {};
		struct PostScriptTickVisitor {};

	// ---

		class FixedTickVisitor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref FixedTickVisitor instance.
			FixedTickVisitor( Core::World& world, size_t arenaSizeInBytes );
		//!	Disable copying.
			FixedTickVisitor( const FixedTickVisitor& ) = delete;

			~FixedTickVisitor() = default;

		// ---------------------------------------------------

		//!	Disable assignment.
			FixedTickVisitor&	operator=( const FixedTickVisitor& ) = delete;

		// - DATA MEMBERS ------------------------------------

		public:
			mutable Eldritch2::ArenaChildAllocator	frameAllocator;
			Core::World&							world;
		};

	// ---

		class VariableTickVisitor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref VariableTickVisitor instance.
			VariableTickVisitor( Core::World& world, size_t arenaSizeInBytes );
		//!	Disable copying.
			VariableTickVisitor( const VariableTickVisitor& ) = delete;

			~VariableTickVisitor() = default;

		// ---------------------------------------------------

		//!	Disable assignment.
			VariableTickVisitor&	operator=( const VariableTickVisitor& ) = delete;

		// - DATA MEMBERS ------------------------------------

		public:
			mutable Eldritch2::ArenaChildAllocator	frameAllocator;
			Core::World&							world;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref WorldService instance.
		WorldService( const Core::ServiceBlackboard& blackboard );
	//!	Disable copying.
		WorldService( const WorldService& ) = delete;
	
		~WorldService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		virtual Eldritch2::Utf8Literal	GetName() const abstract;

	// - WORLD VIEW SANDBOX METHODS ----------------------

	public:
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStartVisitor );
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStopVisitor );
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, FixedTickVisitor& visitor );
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, VariableTickVisitor& visitor );
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const PreScriptTickVisitor );
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const ScriptTickVisitor );
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const PostScriptTickVisitor );
		virtual void	AcceptVisitor( Core::ServiceBlackboard& blackboard );
		virtual void	AcceptVisitor( Scripting::MessageBus& messageBus );

	// ---------------------------------------------------

	protected:
		template <typename ServiceType>
		ServiceType&	LocateService() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		WorldService&	operator=( const WorldService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const Core::ServiceBlackboard&	_blackboard;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/WorldService.inl>
//------------------------------------------------------------------//
