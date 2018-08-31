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
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref EngineComponent instance.
		EngineComponent(const ObjectLocator& services);
		//!	Disable copy construction.
		EngineComponent(const EngineComponent&) = delete;

		~EngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void TickEarly(Scheduling::JobExecutor& executor);
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void Tick(Scheduling::JobExecutor& executor);

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		virtual UniquePointer<WorldComponent> CreateWorldComponent(Allocator& allocator, const ObjectLocator& services);

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		//! Interested service classes should override this method to schedule simple bootstrap tasks with no additional data dependencies.
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void BindResourcesEarly(Scheduling::JobExecutor& executor);

		//! Interested service classes should override this method to schedule initialization tasks that require the use of user-configurable variables.
		/*!	@param[in] executingContext The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void BindConfigurableResources(Scheduling::JobExecutor& executor);

		//! Interested service classes should override this method in order to perform any final initialization that interacts with other attached services.
		/*!	@remark The default implementation does nothing. */
		virtual void BindResources(Scheduling::JobExecutor& executor);

		//! Interested service classes should override this method in order to participate in configurable variable setup.
		/*!	@remark The default implementation does nothing.
			@see @ref Core::PropertyRegistrar */
		virtual void PublishConfiguration(Core::PropertyRegistrar& properties);

		//! Interested service classes should override this method in order to participate in resource/world view creation.
		/*!	@remark The default implementation does nothing.
			@see @ref Assets::AssetFactoryRegistrar */
		virtual void PublishAssetTypes(Assets::AssetApiBuilder& factories);

		//!	Interested service classes should override this method in order to participate in world service location.
		/*!	@remark the default implementation does nothing.
			@see @ref Core::Blackboard */
		virtual void PublishServices(ObjectLocator& services);

		// ---------------------------------------------------

	protected:
		template <typename ServiceType>
		ServiceType* FindService() const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		EngineComponent& operator=(const EngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const ObjectLocator* _services;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/EngineComponent.inl>
//------------------------------------------------------------------//
