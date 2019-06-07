/*==================================================================*\
  XAudio2EngineComponent.WorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
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
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Audio/XAudio2/Xaudio2Listener.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Core/Profiler.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	XAudio2EngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services), _scene(nullptr) {}

	// ---------------------------------------------------

	void XAudio2EngineComponent::WorldComponent::OnVariableRateTick(JobExecutor& executor, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/VariableTick/XAudio2", "Decode streaming audio", 0x2B2B2B);

		const auto monoVoices(_scene->GetMonoVoices());
		executor.ForEach<1u>(monoVoices.Begin(), monoVoices.End(), [this](JobExecutor& /*executor*/, XAudio2AudioScene::OpusVoice& voice) ETNoexceptHint {
			voice.Decode(_scene->GetMonoSamples(), /*mixaheadLimit =*/2u);
		});
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::WorldComponent::BindResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_TERMINATE_WORLD_UNLESS(_scene && SUCCEEDED(_scene->BindResources(nullptr, /*voices =*/32u)));
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::WorldComponent::FreeResources(JobExecutor& /*executor*/) ETNoexceptHint {
		if (XAudio2AudioScene* const scene = Exchange(_scene, nullptr)) {
			scene->FreeResources(nullptr);
		}
	}

	// ---------------------------------------------------

	void XAudio2EngineComponent::WorldComponent::OnFixedRateTickLate(JobExecutor& executor, MicrosecondTime /*delta*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/LateTick/XAudio2", "Update DSP", 0x2B2B2B);

		const auto listeners(_scene->GetListeners());
		executor.ForEach<1u>(listeners.Begin(), listeners.End(), [pitch = Inject<TimeAccumulator>()->GetTimeScalar()](JobExecutor& /*executor*/, XAudio2Listener* listener) ETNoexceptHint {
			listener->Mix(XAUDIO2_COMMIT_NOW, pitch);
		});
	}

}}} // namespace Eldritch2::Audio::XAudio2
