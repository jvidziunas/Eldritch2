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
#include <Core/EngineComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#else
#	include <XAudio2.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	class XAudio2EngineComponent : public Core::EngineComponent, public IXAudio2EngineCallback {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref XAudio2EngineComponent instance.
		XAudio2EngineComponent(const ObjectLocator& services, Logging::Log& log);
		//!	Disable copy construction.
		XAudio2EngineComponent(const XAudio2EngineComponent&) = delete;

		~XAudio2EngineComponent();

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		Result<UniquePointer<Core::WorldComponent>> CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) override;

		void BindResourcesEarly(Scheduling::JobExecutor& executor) override;

		void BindConfigurableResources(Scheduling::JobExecutor& executor) override;

		void TickEarly(Scheduling::JobExecutor& executor) override;

		void PublishConfiguration(Core::PropertyRegistrar& properties) override;

		// ---------------------------------------------------

	private:
		void OnCriticalError(HRESULT error) override;

		void OnProcessingPassStart() override;

		void OnProcessingPassEnd() override;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::ChildLog           _log;
		mutable UsageMixin<MallocAllocator> _allocator;
		HMODULE                             _xaudioLibrary;
		uint32                              _forcedSpeakerCount;
		uint32                              _affinityMask;
		String<>                            _deviceName;
		uint32                              _glitchCount;
		uint64                              _audioProcessingPassTick;
		ComPointer<IXAudio2>                _xaudio;
	};

}}} // namespace Eldritch2::Audio::XAudio2
