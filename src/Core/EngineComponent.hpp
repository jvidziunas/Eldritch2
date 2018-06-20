/*==================================================================*\
  EngineComponent.hpp
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
	class JobExecutor;
}

namespace Assets {
	class AssetApiBuilder;
}

namespace Core {
	class PropertyRegistrar;
	class WorldComponent;
	class World;
} // namespace Core
} // namespace Eldritch2

namespace Eldritch2 { namespace Core {

	class ETPureAbstractHint EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct InitializationVisitor {};
		struct ConfigurationBroadcastVisitor {};
		struct LateInitializationVisitor {};
		struct ServiceTickVisitor {};
		struct WorldTickVisitor {};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref EngineComponent instance.
		EngineComponent(const Blackboard& services);
		//!	Disable copy construction.
		EngineComponent(const EngineComponent&) = delete;

		~EngineComponent() = default;

		// ---------------------------------------------------

	protected:
		template <typename ServiceType>
		ServiceType& FindService() const;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		virtual Result<UniquePointer<WorldComponent>> CreateWorldComponent(Allocator& allocator, const Core::World& world);

		//! Interested service classes should override this method to schedule simple bootstrap tasks with no additional data dependencies.
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void AcceptVisitor(Scheduling::JobExecutor& executor, const InitializationVisitor);
		//! Interested service classes should override this method to schedule initialization tasks that require the use of user-configurable variables.
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void AcceptVisitor(Scheduling::JobExecutor& executor, const ConfigurationBroadcastVisitor);
		//! Interested service classes should override this method in order to participate in configurable variable setup.
		/*!	@remark The default implementation does nothing.
			@see @ref Core::PropertyRegistrar */
		virtual void AcceptVisitor(Core::PropertyRegistrar& properties);
		//! Interested service classes should override this method in order to participate in resource/world view creation.
		/*!	@remark The default implementation does nothing.
			@see @ref Assets::AssetFactoryRegistrar */
		virtual void AcceptVisitor(Assets::AssetApiBuilder& factories);
		//!	Interested service classes should override this method in order to participate in world service location.
		/*!	@remark the default implementation does nothing.
			@see @ref Core::Blackboard */
		virtual void AcceptVisitor(Blackboard& services);
		//! Interested service classes should override this method in order to perform any final initialization that interacts with other attached services.
		/*!	@remark The default implementation does nothing. */
		virtual void AcceptVisitor(Scheduling::JobExecutor& executor, const LateInitializationVisitor);
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void AcceptVisitor(Scheduling::JobExecutor& executor, const ServiceTickVisitor);
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void AcceptVisitor(Scheduling::JobExecutor& executor, const WorldTickVisitor);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		EngineComponent& operator=(const EngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const Blackboard* _services;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/EngineComponent.inl>
//------------------------------------------------------------------//
