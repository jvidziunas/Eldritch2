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

namespace Core {
	class PropertyApiBuilder;
	class AssetApiBuilder;
} // namespace Core
} // namespace Eldritch2

#define ET_TERMINATE_ENGINE_UNLESS(cond)                          \
	if (!(cond)) {                                                \
		Inject<Core::AbstractEngine>()->SetShouldShutDown(); \
		return;                                                   \
	}
#define ET_TERMINATE_ENGINE_IF_FAILED(cond)                       \
	if (Failed(cond)) {                                           \
		Inject<Core::AbstractEngine>()->SetShouldShutDown(); \
		return;                                                   \
	}

namespace Eldritch2 { namespace Core {

	class ETPureAbstractHint EngineComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref EngineComponent instance.
		EngineComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		EngineComponent(const EngineComponent&) = delete;

		~EngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		/*!	@param[in] executor The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void TickEarly(Scheduling::JobExecutor& executor) ETNoexceptHint;
		/*!	@param[in] executor The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void Tick(Scheduling::JobExecutor& executor) ETNoexceptHint;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		//! Interested service classes should override this method to schedule simple bootstrap tasks with no additional data dependencies.
		/*!	@param[in] executor The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint;

		//! Interested service classes should override this method to schedule initialization tasks that require the use of user-configurable variables.
		/*!	@param[in] executor The Scheduling::JobExecutor instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduling::JobExecutor */
		virtual void BindConfigurableResources(Scheduling::JobExecutor& executor) ETNoexceptHint;

		//! Interested service classes should override this method in order to perform any final initialization that interacts with other attached services.
		/*!	@remark The default implementation does nothing. */
		virtual void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint;

		//! Interested service classes should override this method in order to perform any cleanup that can safely be performed in parallel.
		/*!	@remark The default implementation does nothing. */
		virtual void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint;

		//! Interested service classes should override this method in order to participate in configurable variable setup.
		/*!	@remark The default implementation does nothing.
			@see @ref Core::PropertyApiBuilder */
		virtual void PublishApi(Core::PropertyApiBuilder&);

		//! Interested service classes should override this method in order to participate in resource/world view creation.
		/*!	@remark The default implementation does nothing.
			@see @ref Core::AssetApiBuilder */
		virtual void PublishApi(Core::AssetApiBuilder&);

		//!	Interested service classes should override this method in order to participate in world service location.
		/*!	@remark the default implementation does nothing.
			@see @ref ObjectInjector */
		virtual void PublishApi(ObjectInjector&);

		// ---------------------------------------------------

	protected:
		template <typename Service>
		Service* Inject() const ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		EngineComponent& operator=(const EngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const ObjectInjector* _services;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/EngineComponent.inl>
//------------------------------------------------------------------//
