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

namespace Eldritch2 { namespace Core {

	class ETPureAbstractHint WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldComponent instance.
		WorldComponent(const ObjectLocator& services);
		//!	Disable copy construction.
		WorldComponent(const WorldComponent&) = delete;

		~WorldComponent() = default;

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		virtual void BindResourcesEarly(Scheduling::JobExecutor& executor);

		virtual void BindResources(Scheduling::JobExecutor& executor);

		virtual void FreeResources(Scheduling::JobExecutor& executor);

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		virtual void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction);

		virtual void OnFixedRateTickEarly(Scheduling::JobExecutor& executor, MicrosecondTime delta);

		virtual void OnFixedRateTick(Scheduling::JobExecutor& executor, MicrosecondTime delta);

		virtual void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime delta);

		// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		//! Interested service classes should override this method in order to participate in script API setup.
		/*!	@param[in] registrar @parblock @ref Scripting::Wren::ApiBuilder that will handle publishing the service's types,
				methods and variables to scripts. @endparblock
			@remark The default implementation does nothing. */
		virtual void DefineScriptApi(Scripting::Wren::ApiBuilder& api);

		virtual void PublishServices(ObjectLocator& services);

		// - WORLD SERVICE DISCOVERY -------------------------

	protected:
		template <typename ServiceType>
		ServiceType* FindService() const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		WorldComponent& operator=(const WorldComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const ObjectLocator* _services;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/WorldComponent.inl>
//------------------------------------------------------------------//
