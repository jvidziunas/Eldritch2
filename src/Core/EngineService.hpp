/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/StringLiteral.hpp>
#include <Utility/UniquePointer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationRegistrar;
	}

	namespace Scheduling {
		class	JobFiber;
	}

	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Platform {
		class	ContentProvider;
	}

	namespace Assets {
		class	AssetViewFactoryRegistrar;
	}

	namespace Core {
		class	ServiceBlackboard;
		class	WorldService;
		class	World;
	}

	class	ErrorCode;
	class	Allocator;
	template <typename>
	class	Result;
}

namespace Eldritch2 {
namespace Core {

	class ETPureAbstractHint EngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct BeginInitializationVisitor {};
		struct ConfigurationBroadcastVisitor {};
		struct InitializationCompleteVisitor {};
		struct ServiceTickVisitor {};
		struct WorldTickVisitor {};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref EngineService instance.
		EngineService( const Core::ServiceBlackboard& serviceBlackboard );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

		~EngineService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
	//!	Retrieves the human-readable name of this @ref EngineService instance.
		virtual Eldritch2::Utf8Literal	GetName() const abstract;

	// ---------------------------------------------------

	public:
		virtual Eldritch2::Result<Eldritch2::UniquePointer<Core::WorldService>>	CreateWorldService( Eldritch2::Allocator& allocator, const Core::World& world );

	// ---------------------------------------------------

	public:
	//! Interested service classes should override this method to schedule simple bootstrap tasks with no additional data dependencies.
	/*!	@param[in] executingContext The Scheduling::JobFiber instance that should execute any internal work.
		@remark The default implementation does nothing.
		@see @ref Scheduling::JobFiber */
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const BeginInitializationVisitor );
	//! Interested service classes should override this method to schedule initialization tasks that require the use of user-configurable variables.
	/*!	@param[in] executingContext The Scheduling::JobFiber instance that should execute any internal work.
		@remark The default implementation does nothing.
		@see @ref Scheduling::JobFiber */
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const ConfigurationBroadcastVisitor );
	//! Interested service classes should override this method in order to participate in configurable variable setup.
	/*!	@remark The default implementation does nothing.
		@see @ref Configuration::ConfigurationRegistrar */
		virtual void	AcceptVisitor( Configuration::ConfigurationRegistrar& registrar );
	//! Interested service classes should override this method in order to participate in resource/world view creation.
	/*!	@remark The default implementation does nothing.
		@see @ref Assets::AssetViewFactoryRegistrar */
		virtual void	AcceptVisitor( Assets::AssetViewFactoryRegistrar& registrar );
	//!	Interested service classes should override this method in order to participate in world service location.
	/*!	@remark the default implementation does nothing.
		@see @ref Core::ServiceBlackboard */
		virtual void	AcceptVisitor( Core::ServiceBlackboard& serviceBlackboard );
	//! Interested service classes should override this method in order to participate in script API setup.
	/*!	@param[in] registrar The type registrar that will handle publishing the service's types, methods and variables to script objects.
		@remark The default implementation does nothing.
		@see @ref Scripting::ApiRegistrar */
		virtual void	AcceptVisitor( Scripting::ApiRegistrar& registrar );
	//! Interested service classes should override this method in order to perform any final initialization that interacts with other attached services.
	/*!	@remark The default implementation does nothing. */
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const InitializationCompleteVisitor );
	/*!	@param[in] executingContext The Scheduling::JobFiber instance that should execute any internal work.
		@remark The default implementation does nothing.
		@see @ref Scheduling::JobFiber */
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const ServiceTickVisitor );
	/*!	@param[in] executingContext The Scheduling::JobFiber instance that should execute any internal work.
		@remark The default implementation does nothing.
		@see @ref Scheduling::JobFiber */
		virtual void	AcceptVisitor( Scheduling::JobFiber& executor, const WorldTickVisitor );

	// ---------------------------------------------------

	protected:
		template <typename ServiceType>
		ServiceType&	LocateService() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const Core::ServiceBlackboard&	_serviceLocator;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/EngineService.inl>
//------------------------------------------------------------------//