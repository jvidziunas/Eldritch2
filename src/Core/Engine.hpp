/*==================================================================*\
  Engine.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/StringLocalization.hpp>
#include <Core/EngineComponent.hpp>
#include <Logging/FileLog.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	class ETPureAbstractHint AbstractEngine {
		// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename ContentDatabase>
		class ManagementComponent : public EngineComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ManagementComponent instance.
			template <typename... ProviderCtorArgs>
			ManagementComponent(const ObjectInjector& services, ProviderCtorArgs&&...) ETNoexceptHint;
			//!	Disable copy construction.
			ManagementComponent(const ManagementComponent&) = delete;

			~ManagementComponent() = default;

			// - ENGINE SERVICE SANDBOX METHODS ------------------

		public:
			void BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void PublishApi(Core::PropertyApiBuilder& api) override;

			using EngineComponent::PublishApi;

			// - ENGINE SERVICE SANDBOX METHODS ------------------

		public:
			void TickEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void Tick(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			ManagementComponent& operator=(const ManagementComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			ContentDatabase _content;
			uint32          _maxSweepsPerFrame;
		};

		// ---

	public:
		using WorldList = ArrayList<UniquePointer<AbstractWorld>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		AbstractEngine(const AbstractEngine&) = delete;
		//!	Constructs this @ref AbstractEngine instance.
		AbstractEngine() ETNoexceptHint;

		virtual ~AbstractEngine() = default;

		// ---------------------------------------------------

	public:
		void SetShouldShutDown(MemoryOrder order = std::memory_order_release) const ETNoexceptHint;

		bool ShouldRun(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// - WORLD MANAGEMENT --------------------------------

	public:
		//!	Creates a new @ref World that will host a game instance.
		/*!	@param[in] executor @ref JobExecutor that will run the world's initialization jobs.
			@returns @ref Errors::None if the world was instantiated successfully, or an @ref ErrorCode with information on why the operation failed.
			@remarks See world service implementations for examples of properties that may be configured here. */
		void CreateWorld(Scheduling::JobExecutor& executor) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void ApplyProperties(Scheduling::JobExecutor& executor, PlatformStringSpan path) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void BootOnCaller(Scheduling::JobExecutor& executor) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		virtual void RunFrame(Scheduling::JobExecutor& executor) ETNoexceptHint abstract;

		void TickWorlds(Scheduling::JobExecutor& executor) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		template <typename ApiBuilder>
		ApiBuilder BuildApi(ApiBuilder api) const;

		Result BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint;

		void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		virtual void BindComponents(Scheduling::JobExecutor& executor) ETNoexceptHint abstract;

		virtual void FreeComponents(Scheduling::JobExecutor& executor) ETNoexceptHint abstract;

		virtual void BindConfigurableResources(Scheduling::JobExecutor& executor) ETNoexceptHint abstract;

		// ---------------------------------------------------

	private:
		virtual UniquePointer<AbstractWorld> AllocateWorld(Allocator&, const ObjectInjector&) abstract;

		virtual void PublishComponents(PropertyApiBuilder&) const abstract;

		virtual void PublishComponents(AssetApiBuilder&) const abstract;

		virtual void PublishComponents(ObjectInjector&) const abstract;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AbstractEngine& operator=(const AbstractEngine&) = delete;

		// - DATA MEMBERS ------------------------------------

	protected:
		ObjectInjector _services;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable UsageMixin<MallocAllocator> _allocator;
		StringLocalization                  _localization;
		Logging::FileLog                    _log;
		mutable Atomic<bool>                _shouldRun;
		ETCacheLineAligned mutable Mutex    _worldsMutex;
		WorldList                           _worlds;
	};

	// ---

	template <typename... Components>
	class Engine : public Core::AbstractEngine {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy assignment.
		Engine(const Engine&) = delete;
		//!	Constructs this @ref Engine instance.
		Engine() ETNoexceptHint;

		~Engine() override = default;

		// ---------------------------------------------------

	private:
		void RunFrame(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

	private:
		void BindComponents(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		void FreeComponents(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		void BindConfigurableResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

	private:
		UniquePointer<AbstractWorld> AllocateWorld(Allocator&, const ObjectInjector&) override;

		void PublishComponents(PropertyApiBuilder&) const override;

		void PublishComponents(AssetApiBuilder&) const override;

		void PublishComponents(ObjectInjector&) const override;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Tuple<Components...> _components;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/Engine.inl>
//------------------------------------------------------------------//
