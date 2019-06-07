/*==================================================================*\
  Xaudio2EngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2EngineComponent.hpp>
#include <Core/PropertyApiBuilder.hpp>
#include <Core/Profiler.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	XAudio2EngineComponent::XAudio2EngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services),
																									_xaudioLibrary(nullptr),
																									_deviceName(MallocAllocator("XAudio2 Device Name Allocator")),
																									_affinityMask(uint32(XAUDIO2_DEFAULT_PROCESSOR)),
																									_channelCount(XAUDIO2_DEFAULT_CHANNELS),
																									_sampleRateHz(XAUDIO2_DEFAULT_SAMPLERATE),
																									_operationSet(0u),
																									_glitchCount(0u) {}

	// ---------------------------------------------------

	XAudio2EngineComponent::~XAudio2EngineComponent() {
		if (_output) {
			_output->DestroyVoice();
		}

		if (_xaudio) {
			_xaudio->StopEngine();
		}

		if (_xaudioLibrary) {
			FreeLibrary(_xaudioLibrary);
		}
	}

	// ---------------------------------------------------

	uint32 XAudio2EngineComponent::GetFrameOperationSet(MemoryOrder order) const ETNoexceptHint {
		return _operationSet.load(order);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_SUPPRESS_MSVC_WARNINGS(6239 6326)
		if (!IsWindows8OrNewer()) {
			_xaudioLibrary = LoadLibraryExW(ETReleaseToggle(SL("XAudio2_7.DLL"), SL("XAudio2_7D.DLL")), /*hFile =*/nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
			ETVerify(_xaudioLibrary != nullptr, "Error loading XAudio2 DLL!");
		}
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) ETNoexceptHint {
		using ::Eldritch2::Swap;

		ComPointer<IXAudio2> xaudio;
		if (FAILED(XAudio2Create(xaudio.GetInterfacePointer(), /*Flags =*/0u, XAUDIO2_PROCESSOR(_affinityMask))) || FAILED(xaudio->RegisterForCallbacks(this))) {
			Inject<Log>()->Write(Severity::Error, "Unable to create XAudio2 instance!" ET_NEWLINE);
			return Inject<AbstractEngine>()->SetShouldShutDown();
		}

		IXAudio2MasteringVoice* output(nullptr);
		ET_AT_SCOPE_EXIT(if (output) output->DestroyVoice());
		if (IsWindows8OrNewer() ? FAILED(xaudio->CreateMasteringVoice(ETAddressOf(output), _channelCount, _sampleRateHz, /*Flags =*/0u)) : true) {
			Inject<Log>()->Write(Severity::Error, "Unable to create XAudio2 mastering voice!" ET_NEWLINE);
			return Inject<AbstractEngine>()->SetShouldShutDown();
		}

		ET_TERMINATE_ENGINE_UNLESS(SUCCEEDED(xaudio->StartEngine()));

		_operationSet.store(0u, std::memory_order_release);
		Swap(_xaudio, xaudio);
		Swap(_output, output);

		Inject<Log>()->Write(Severity::Message, "XAudio2 initialized successfully." ET_NEWLINE);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::PublishApi(PropertyApiBuilder& api) {
		api.DefineGroup("XAudio2")
			.DefineProperty("AudioProcessingThreadAffinityMask", PropertyApiBuilder::MakeSetter(_affinityMask))
			.DefineProperty("ForcedChannelCount", PropertyApiBuilder::MakeSetter(_channelCount))
			.DefineProperty("ForcedSampleRateHz", PropertyApiBuilder::MakeSetter(_sampleRateHz))
			.DefineProperty("PreferredDeviceName", [this](StringSpan name) ETNoexceptHint {
				_deviceName = name;
			});
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::TickEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/ServiceTick", "Update XAudio", 0xABBBCB);

		ET_TERMINATE_ENGINE_UNLESS(SUCCEEDED(_xaudio->CommitChanges(_operationSet.fetch_add(1u, std::memory_order_acquire))));

		XAUDIO2_PERFORMANCE_DATA performance;
		_xaudio->GetPerformanceData(ETAddressOf(performance));
		if (performance.GlitchesSinceEngineStarted != _glitchCount.exchange(performance.GlitchesSinceEngineStarted, std::memory_order_consume)) {
			Inject<Log>()->Write(Severity::Error, "XAudio processing stall since last invocation of IXAudio2::GetPerformanceData()!" ET_NEWLINE);
		}
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::OnProcessingPassStart() {}

	// ---------------------------------------------------

	void XAudio2EngineComponent::OnProcessingPassEnd() {}

	// ---------------------------------------------------

	void XAudio2EngineComponent::OnCriticalError(HRESULT error) {
		Inject<Log>()->Write(Severity::Error, "Critical error in XAudio! (HRESULT: {#x})" ET_NEWLINE, error);
	}

}}} // namespace Eldritch2::Audio::XAudio2
