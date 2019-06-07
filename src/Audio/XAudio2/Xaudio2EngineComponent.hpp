/*==================================================================*\
  Xaudio2EngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/Xaudio2Selector.hpp>
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
#include <Audio/PcmCache.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	class XAudio2AudioScene;
}}} // namespace Eldritch2::Audio::XAudio2

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	class XAudio2EngineComponent : public Core::EngineComponent, public IXAudio2EngineCallback {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class WorldComponent : public Core::WorldComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref XAudio2WorldComponent instance.
			WorldComponent(const ObjectInjector& services) ETNoexceptHint;
			//!	Disable copy construction.
			WorldComponent(const WorldComponent&) = delete;

			~WorldComponent() = default;

			// ---------------------------------------------------

		public:
			void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) ETNoexceptHint override;

			void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime delta) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder&) override;

			using Core::WorldComponent::PublishApi;

			// - DATA MEMBERS ------------------------------------

		private:
			XAudio2AudioScene* _scene;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref XAudio2EngineComponent instance.
		XAudio2EngineComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		XAudio2EngineComponent(const XAudio2EngineComponent&) = delete;

		~XAudio2EngineComponent();

		// ---------------------------------------------------

	public:
		uint32 GetFrameOperationSet(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		void BindConfigurableResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		void TickEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void PublishApi(Core::PropertyApiBuilder& api) override;

		using EngineComponent::PublishApi;

		// ---------------------------------------------------

	private:
		void OnCriticalError(HRESULT error) override;

		void OnProcessingPassStart() override;

		void OnProcessingPassEnd() override;

		// - DATA MEMBERS ------------------------------------

	private:
		HMODULE                 _xaudioLibrary;
		String                  _deviceName;
		uint32                  _affinityMask;
		uint32                  _channelCount;
		uint32                  _sampleRateHz;
		Atomic<uint32>          _operationSet;
		Atomic<uint32>          _glitchCount;
		ComPointer<IXAudio2>    _xaudio;
		IXAudio2MasteringVoice* _output;
	};

}}} // namespace Eldritch2::Audio::XAudio2
