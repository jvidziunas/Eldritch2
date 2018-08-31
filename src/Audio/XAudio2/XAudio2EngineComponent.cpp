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
#if (_WIN32_WINNT_WIN8 <= _WIN32_WINNT)
ET_LINK_LIBRARY("XAudio2.lib")
#else
ET_LINK_LIBRARY("x3daudio.lib")
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	namespace {

		MICROPROFILE_DEFINE(audioProcessingPass, "Engine/XAudio2", "Audio processing pass", 0xCCBBCB);

	} // anonymous namespace

	XAudio2EngineComponent::XAudio2EngineComponent(const ObjectLocator& services) :
		EngineComponent(services),
		_allocator("XAudio2 Audio Renderer Allocator"),
		_log(FindService<Engine>()->GetLog()),
		_speakerCount(0u),
		_affinityMask(uint32(XAUDIO2_PROCESSOR::XAUDIO2_DEFAULT_PROCESSOR)),
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

	UniquePointer<WorldComponent> XAudio2EngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<XAudio2WorldComponent>(allocator, services);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		ET_SUPPRESS_MSVC_WARNINGS(6239 6326)
		if (_WIN32_WINNT < _WIN32_WINNT_WIN8) {
			_xaudioLibrary = LoadLibraryExW(ETIsReleaseBuild() ? L"XAudio2_7.DLL" : L"XAudio2_7D.DLL", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
			ET_VERIFY(_xaudioLibrary, "Error loading XAudio2 DLL!");
		}
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) {
		ComPointer<IXAudio2> xaudio;
		if (FAILED(XAudio2Create(xaudio.GetInterfacePointer(), 0, XAUDIO2_DEFAULT_PROCESSOR))) {
			_log.Write(Severity::Error, "Unable to create XAudio2 instance!" ET_NEWLINE);
			FindService<Engine>()->SetShouldShutDown();
			return;
		}

		if (FAILED(xaudio->RegisterForCallbacks(this))) {
			_log.Write(Severity::Error, "Unable to register XAudio2 device callbacks!" ET_NEWLINE);
			FindService<Engine>()->SetShouldShutDown();
			return;
		}

		if (FAILED(xaudio->StartEngine())) {
			_log.Write(Severity::Error, "Unable to start XAudio2 engine!" ET_NEWLINE);
			FindService<Engine>()->SetShouldShutDown();
			return;
		}

		_log.Write(Severity::Message, "XAudio2 initialized successfully." ET_NEWLINE);

		//	Commit changes to the service.
		Swap(_xaudio, xaudio);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::PublishConfiguration(PropertyRegistrar& properties) {
		properties.BeginSection("XAudio2")
			.DefineProperty("AudioProcessingThreadAffinityMask", _affinityMask)
			.DefineProperty("ForcedSpeakerCount", _speakerCount)
			.DefineProperty("PreferredDeviceName", [this](StringView name) {
				_deviceName = name;
			});
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::TickEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/ServiceTick", "Update XAudio", 0xABBBCB);
		XAUDIO2_PERFORMANCE_DATA performance;

		_xaudio->GetPerformanceData(&performance);
		if (performance.GlitchesSinceEngineStarted != eastl::exchange(_glitchCount, performance.GlitchesSinceEngineStarted)) {
			_log.Write(Severity::Error, "XAudio processing stall since last invocation of IXAudio2::GetPerformanceData()!" ET_NEWLINE);
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
		_log.Write(Severity::Error, "Critical error in XAudio!" ET_NEWLINE);
	}

}}} // namespace Eldritch2::Audio::XAudio2
