/*==================================================================*\
  StreamingVoice.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	template <typename Sample, typename Decoder>
	ETInlineHint ETForceInlineHint StreamingVoice<Sample, Decoder>::StreamingVoice() ETNoexceptHint : _client(nullptr), _voice(nullptr), _channels(0u), _sampleHz(0u), _position(0u), _decoder() {}

	// ---------------------------------------------------

	template <typename Sample, typename Decoder>
	ETInlineHint ETForceInlineHint StreamingVoice<Sample, Decoder>::StreamingVoice(StreamingVoice<Sample, Decoder>&& voice) :
		_client(voice._client.exchange(nullptr, std::memory_order_relaxed)),
		_voice(Exchange(voice._voice, nullptr)),
		_channels(Exchange(voice._channels, 0u)),
		_sampleHz(Exchange(voice._sampleHz, 0u)),
		_position(Exchange(voice._position, 0u)),
		_decoder(Move(voice._decoder)) {}

	// ---------------------------------------------------

	template <typename Sample, typename Decoder>
	ETInlineHint ETForceInlineHint HRESULT StreamingVoice<Sample, Decoder>::Decode(PcmCache<Sample>& target, uint32 mixaheadLimit) {
		ETInfiniteLoop {
			XAUDIO2_VOICE_STATE current;
			_voice->GetState(ETAddressOf(current));
			if (current.BuffersQueued >= mixaheadLimit) {
				break;
			}

			Sample* samples;
			if (Failed(_decoder.DecodeGrain(samples, target, _sampleHz, _position++))) {
				return E_FAIL;
			}

			const XAUDIO2_BUFFER buffer{
				/*Flags =*/0,
				/*AudioBytes =*/0u,
				/*pAudioData =*/reinterpret_cast<BYTE*>(samples),
				/*PlayBegin =*/0u,
				/*PlayLength =*/0u,
				/*LoopBegin =*/0u,
				/*LoopLength =*/0u,
				/*LoopCount =*/XAUDIO2_NO_LOOP_REGION,
				/*pContext =*/samples
			};

			const auto result(_voice->SubmitSourceBuffer(ETAddressOf(buffer)));
			if (FAILED(result)) {
				return result;
			}
		}

		return S_OK;
	}

	// ---------------------------------------------------

	template <typename Sample, typename Decoder>
	ETInlineHint ETForceInlineHint HRESULT StreamingVoice<Sample, Decoder>::UpdateDsp(UINT32 operationSet, float32 volume, float32 doppler, const float32 levelMatrix[]) {
		HRESULT result(_voice->SetOutputMatrix(_client.load(std::memory_order_consume), settings.SrcChannelCount, settings.DstChannelCount, levelMatrix, operationSet));
		if (FAILED(result)) {
			return result;
		}

		result = _voice->SetFrequencyRatio(doppler, operationSet);
		if (FAILED(result)) {
			return result;
		}

		return _voice->SetVolume(volume, operationSet)
	}

	// ---------------------------------------------------

	template <typename Sample, typename Decoder>
	template <typename... DecoderArgs>
	ETInlineHint ETForceInlineHint HRESULT StreamingVoice<Sample, Decoder>::Acquire(UINT32 operationSet, IXAudio2Voice* client, DecoderArgs&&... decoderArgs) ETNoexceptHint {
		Decoder decoder;
		if (Failed(decoder.BindResources(_channels, _sampleHz, Forward<DecoderArgs>(decoderArgs)...))) {
			_client.store(nullptr, std::memory_order_release);
			return E_FAIL;
		}

		IXAudio2Voice* expected(nullptr);
		if (_client.compare_exchange_strong(expected, client, std::memory_order_consume)) {
			return E_ACCESSDENIED;
		}

		if (FAILED(_voice->Start(/*Flags =*/0u, operationSet))) {
			_client.store(nullptr, std::memory_order_release);
			return E_FAIL;
		}

		Swap(_decoder, decoder);

		return S_OK;
	}

	// ---------------------------------------------------

	template <typename Sample, typename Decoder>
	ETInlineHint ETForceInlineHint void StreamingVoice<Sample, Decoder>::Release() ETNoexceptHint {
		_voice->Stop(/*Flags =*/0u, XAUDIO2_COMMIT_NOW);
		_voice->FlushSourceBuffers();
		_decoder.FreeResources();
		_client.store(nullptr, std::memory_order_release);
	}

	// ---------------------------------------------------

	template <typename Sample, typename Decoder>
	ETInlineHint ETForceInlineHint HRESULT StreamingVoice<Sample, Decoder>::BindResources(IXAudio2* xaudio, DWORD channelMask, DWORD sampleHz) {
		using ::Eldritch2::Swap;
		static ETConstexpr WORD BitsPerSample(sizeof(Sample) * CHAR_BIT);
		static ETConstexpr GUID SubFormat(eastl::is_floating_point_v<Sample> ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT : KSDATAFORMAT_SUBTYPE_PCM);

		uint32                     channels(CountSetBits(uint32(channelMask)));
		const WAVEFORMATEXTENSIBLE format{
			WAVEFORMATEX{ /*wFormatTag =*/WAVE_FORMAT_EXTENSIBLE,
						  /*nChannels =*/WORD(channels),
						  /*nSamplesPerSec =*/sampleHz,
						  /*nAvgBytesPerSec =*/DWORD(sampleHz * channels * sizeof(Sample)),
						  /*nBlockAlign =*/WORD(channels * sizeof(Sample)),
						  /*wBitsPerSample =*/BitsPerSample,
						  /*cbSize =*/WORD(sizeof(format) - sizeof(format.Format)) },
			{ /*wValidBitsPerSample =*/BitsPerSample },
			/*dwChannelMask =*/channelMask,
			/*SubFormat =*/SubFormat
		};

		IXAudio2SourceVoice* voice(nullptr);
		const HRESULT        result(xaudio->CreateSourceVoice(ETAddressOf(voice), ETAddressOf(format.Format), /*Flags =*/0u, XAUDIO2_DEFAULT_FREQ_RATIO));
		if (FAILED(result)) {
			return result;
		}

		Swap(_voice, voice);
		Swap(_channels, channels);
		Swap(_sampleHz, sampleHz);

		return S_OK;
	}

	// ---------------------------------------------------

	template <typename Sample, typename Decoder>
	ETInlineHint ETForceInlineHint void StreamingVoice<Sample, Decoder>::FreeResources(IXAudio2* /*xaudio*/) ETNoexceptHint {
		_decoder.FreeResources();
		if (IXAudio2SourceVoice* const voice = Exchange(_voice, nullptr)) {
			voice->DestroyVoice();
		}
		_client.store(nullptr, std::memory_order_release);
	}

}}} // namespace Eldritch2::Audio::XAudio2
