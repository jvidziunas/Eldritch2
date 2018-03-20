/*==================================================================*\
  WorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}

	namespace Scripting {
		namespace Wren {
			class	ApiBuilder;
		}
	}

	namespace Core {
		class	World;
	}
}

namespace Eldritch2 {
namespace Core {

	class ETPureAbstractHint WorldComponent {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct EarlyInitializationVisitor	{};
		struct LateInitializationVisitor	{};
		struct TearDownVisitor				{};

	public:
		struct TickVisitor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref TickVisitor instance,
			TickVisitor( uint64 durationInMicroseconds );

			~TickVisitor() = default;

		public:
			const uint64 durationInMicroseconds;
		};

	// ---

	public:
		struct EarlyTickVisitor : public TickVisitor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref EarlyTickVisitor instance,
			EarlyTickVisitor( uint64 durationInMicroseconds );

			~EarlyTickVisitor() = default;
		};

	// ---

	public:
		struct StandardTickVisitor : public TickVisitor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref StandardTickVisitor instance,
			StandardTickVisitor( uint64 durationInMicroseconds );

			~StandardTickVisitor() = default;
		};

	// ---

	public:
		struct LateTickVisitor : public TickVisitor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref LateTickVisitor instance,
			LateTickVisitor( uint64 durationInMicroseconds );

			~LateTickVisitor() = default;
		};

	// ---

	public:
		struct VariableTickVisitor : public TickVisitor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref VariableTickVisitor instance,
			VariableTickVisitor( uint64 durationInMicroseconds );

			~VariableTickVisitor() = default;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref WorldComponent instance.
		WorldComponent( const Blackboard& services );
	//!	Disable copy construction.
		WorldComponent( const WorldComponent& ) = delete;
	
		~WorldComponent() = default;

	// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		virtual void	AcceptVisitor( Scheduling::JobExecutor& executor, const EarlyInitializationVisitor );
		virtual void	AcceptVisitor( Scheduling::JobExecutor& executor, const LateInitializationVisitor );
		virtual void	AcceptVisitor( Scheduling::JobExecutor& executor, const VariableTickVisitor& );
		virtual void	AcceptVisitor( Scheduling::JobExecutor& executor, const EarlyTickVisitor& );
		virtual void	AcceptVisitor( Scheduling::JobExecutor& executor, const StandardTickVisitor& );
		virtual void	AcceptVisitor( Scheduling::JobExecutor& executor, const LateTickVisitor& );
		virtual void	AcceptVisitor( Scheduling::JobExecutor& executor, const TearDownVisitor );
	//! Interested service classes should override this method in order to participate in script API setup.
	/*!	@param[in] registrar @parblock @ref Scripting::Wren::ApiBuilder that will handle publishing the service's types,
			methods and variables to scripts. @endparblock
		@remark The default implementation does nothing. */
		virtual void	AcceptVisitor( Scripting::Wren::ApiBuilder& api );
		virtual void	AcceptVisitor( Blackboard& services );

	// ---------------------------------------------------

	protected:
		template <typename ServiceType>
		ServiceType&	FindService() const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		WorldComponent&	operator=( const WorldComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const Blackboard*	_services;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/WorldComponent.inl>
//------------------------------------------------------------------//