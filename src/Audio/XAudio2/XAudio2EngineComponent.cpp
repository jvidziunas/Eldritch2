/*==================================================================*\
  Xaudio2EngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2EngineComponent.hpp>
#include <Audio/XAudio2/XAudio2WorldComponent.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
ET_LINK_LIBRARY("XAudio2.lib")
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	namespace {

		MICROPROFILE_DEFINE(audioProcessingPass, "Engine/XAudio2", "Audio processing pass", 0xCCBBCB);

	} // anonymous namespace

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	XAudio2EngineComponent::XAudio2EngineComponent(const ObjectLocator& services, Log& log) :
		EngineComponent(services),
		_allocator("XAudio2 Audio Renderer Allocator"),
		_log(log),
		_forcedSpeakerCount(0u),
		_affinityMask(XAUDIO2_PROCESSOR::XAUDIO2_DEFAULT_PROCESSOR),
		_deviceName(MallocAllocator("XAudio2 Device Name Allocator")),
		_glitchCount(0u) {
	}

	// ---------------------------------------------------

	XAudio2EngineComponent::~XAudio2EngineComponent() {
		if (_xaudio) {
			_xaudio->StopEngine();
		}

		if (_xaudioLibrary) {
			FreeLibrary(_xaudioLibrary);
		}
	}

	// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> XAudio2EngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<XAudio2WorldComponent>(allocator, services);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		if (_WIN32_WINNT < _WIN32_WINNT_WIN8 && IsWindows8OrNewer()) {
			return;
		}

		_xaudioLibrary = LoadLibraryExW(L"XAudio2_7.DLL", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32 /* LOAD_LIBRARY_SEARCH_SYSTEM32*/);
		ET_VERIFY(_xaudioLibrary, "Error loading XAudio2 DLL!");
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) {
		ComPointer<IXAudio2> xaudio;

		if (FAILED(XAudio2Create(xaudio.GetInterfacePointer(), 0, XAUDIO2_DEFAULT_PROCESSOR))) {
			_log.Write(MessageType::Error, "Unable to create XAudio2 instance!" UTF8_NEWLINE);
			FindService<Engine>().SetShouldShutDown();
			return;
		}

		if (FAILED(xaudio->RegisterForCallbacks(this))) {
			_log.Write(MessageType::Error, "Unable to register XAudio2 device callbacks!" UTF8_NEWLINE);
			FindService<Engine>().SetShouldShutDown();
			return;
		}

		if (FAILED(xaudio->StartEngine())) {
			_log.Write(MessageType::Error, "Unable to start XAudio2 engine!" UTF8_NEWLINE);
			FindService<Engine>().SetShouldShutDown();
			return;
		}

		_log.Write(MessageType::Message, "XAudio2 initialized successfully." UTF8_NEWLINE);

		//	Commit changes to the service.
		Swap(_xaudio, xaudio);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::PublishConfiguration(PropertyRegistrar& properties) {
		properties.BeginSection("XAudio2")
			.DefineProperty("AudioProcessingThreadAffinityMask", _affinityMask)
			.DefineProperty("ForcedSpeakerCount", _forcedSpeakerCount)
			.DefineProperty("PreferredDeviceName", [this](StringView<Utf8Char> value) {
				_deviceName.Assign(value);
			});
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::TickEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/ServiceTick", "Update XAudio", 0xABBBCB);
		XAUDIO2_PERFORMANCE_DATA performance;

		_xaudio->GetPerformanceData(&performance);
		if (eastl::exchange(_glitchCount, performance.GlitchesSinceEngineStarted) != performance.GlitchesSinceEngineStarted) {
			_log.Write(MessageType::Error, "XAudio processing stall since last invocation of IXAudio2::GetPerformanceData()!" UTF8_NEWLINE);
		}
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::OnProcessingPassStart() {
		MICROPROFILE_ENTER(audioProcessingPass);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::OnProcessingPassEnd() {
		MICROPROFILE_LEAVE();
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::OnCriticalError(HRESULT /*error*/) {
		//	TODO: Convert the HRESULT value into something printable.
		_log.Write(MessageType::Error, "Critical error in XAudio!" UTF8_NEWLINE);
	}

}}} // namespace Eldritch2::Audio::XAudio2
