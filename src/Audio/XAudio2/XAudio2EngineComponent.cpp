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
#include <microprofile/microprofile.h>
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

	XAudio2EngineComponent::XAudio2EngineComponent(
		const Blackboard& services,
		Log&              log) :
		EngineComponent(services),
		_allocator("XAudio2 Audio Renderer Allocator"),
		_log(log),
		_forcedSpeakerCount(0u),
		_affinityMask(static_cast<uint32>(XAUDIO2_PROCESSOR::XAUDIO2_DEFAULT_PROCESSOR)),
		_deviceName(MallocAllocator("XAudio2 Device Name Allocator")),
		_glitchCount(0u) {
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
		_xaudioDll = LoadLibraryExW(L"XAudio2_7.DLL", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);
		ET_VERIFY(_xaudioDll, "Error loading XAudio2 DLL!");
#endif
	}

	// ---------------------------------------------------

	XAudio2EngineComponent::~XAudio2EngineComponent() {
		if (_xaudio) {
			_xaudio->StopEngine();
		}

#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
		if (_xaudioDll) {
			FreeLibrary(_xaudioDll);
		}
#endif
	}

	// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> XAudio2EngineComponent::CreateWorldComponent(Allocator& allocator, const World& world) {
		UniquePointer<WorldComponent> xaudioComponent(MakeUnique<XAudio2WorldComponent>(allocator, world));
		if (!xaudioComponent) {
			return Error::OutOfMemory;
		}

		return eastl::move(xaudioComponent);
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::AcceptVisitor(JobExecutor& /*executor*/, const ConfigurationBroadcastVisitor) {
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

	void XAudio2EngineComponent::AcceptVisitor(PropertyRegistrar& properties) {
		properties.BeginSection("XAudio2")
			.WhenPropertyChanged("ForcedSpeakerCount", _forcedSpeakerCount)
			.WhenPropertyChanged("AudioProcessingThreadAffinityMask", _affinityMask)
			.WhenPropertyChanged("PreferredDeviceName", [this](Range<const Utf8Char*> value) {
				_deviceName.Assign(value.Begin(), value.End());
			});
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::AcceptVisitor(JobExecutor& /*executor*/, const ServiceTickVisitor) {
		MICROPROFILE_SCOPEI("Engine/ServiceTick", "Update XAudio", 0xABBBCB);

		XAUDIO2_PERFORMANCE_DATA performance;

		_xaudio->GetPerformanceData(&performance);

		if (_glitchCount != performance.GlitchesSinceEngineStarted) {
			_log.Write(MessageType::Error, "XAudio reported audio processing stall since last invocation of IXAudio2::GetPerformanceData()!" UTF8_NEWLINE);

			_glitchCount = performance.GlitchesSinceEngineStarted;
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
