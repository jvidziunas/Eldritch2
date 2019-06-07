/*==================================================================*\
  WorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
	class JobExecutor;
} // namespace Scheduling

namespace Scripting { namespace Wren {
	class ApiBuilder;
}} // namespace Scripting::Wren
} // namespace Eldritch2

#define ET_TERMINATE_WORLD_IF(cond, ...)                    \
	if ((cond)) {                                           \
		__VA_ARGS__;                                        \
		Inject<Core::AbstractWorld>()->SetShouldShutDown(); \
		return;                                             \
	}
#define ET_TERMINATE_WORLD_UNLESS(cond, ...)                \
	if (!(cond)) {                                          \
		__VA_ARGS__;                                        \
		Inject<Core::AbstractWorld>()->SetShouldShutDown(); \
		return;                                             \
	}
#define ET_TERMINATE_WORLD_IF_FAILED(cond, ...) ET_TERMINATE_WORLD_IF(Failed(cond), __VA_ARGS__)

namespace Eldritch2 { namespace Core {

	class ETPureAbstractHint WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldComponent instance.
		WorldComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		WorldComponent(const WorldComponent&) = delete;
		//!	Constructs this @ref WorldComponent instance.
		WorldComponent(WorldComponent&&) ETNoexceptHint = default;

		~WorldComponent() = default;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		virtual void BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint;

		virtual void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint;

		virtual void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		virtual void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) ETNoexceptHint;

		virtual void OnFixedRateTickEarly(Scheduling::JobExecutor& executor, MicrosecondTime delta) ETNoexceptHint;

		virtual void OnFixedRateTick(Scheduling::JobExecutor& executor, MicrosecondTime delta) ETNoexceptHint;

		virtual void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime delta) ETNoexceptHint;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		//! Interested service classes should override this method in order to participate in script API setup.
		/*!	@param[in] @parblock @ref Scripting::Wren::ApiBuilder that will handle publishing the service's types,
				methods and variables to scripts. @endparblock
			@remark The default implementation does nothing. */
		virtual void PublishApi(Scripting::Wren::ApiBuilder&);

		virtual void PublishApi(ObjectInjector&);

		// - WORLD SERVICE DISCOVERY -------------------------

	protected:
		template <typename Service>
		Service* Inject() const ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		WorldComponent& operator=(const WorldComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const ObjectInjector* _services;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/WorldComponent.inl>
//------------------------------------------------------------------//
