/*==================================================================*\
  OpusStreamingXAudio2VoiceCallback.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#else
#	include <XAudio2.h>
#endif
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	class OpusStreamingCallback : public IXAudio2VoiceCallback {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32 {
			OutputSampleRateInHz	= 48000
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		OpusStreamingCallback( const OpusStreamingCallback& ) = delete;
	//!	Constructs this @ref OpusStreamingCallback instance.
		OpusStreamingCallback( int channelCount );
		
		~OpusStreamingCallback();

	// ---------------------------------------------------

	public:
		void	OnBufferStart( void* completedBuffer ) override;
   
		void	OnBufferEnd( void* completedBuffer ) override;

		void	OnLoopEnd( void* completedBuffer ) override;

	// ---------------------------------------------------

	private:
		std::atomic<uint32>	_producedBuffers;
		std::atomic<uint32>	_consumedBuffers;
		char				_decoderMemory[];
	};

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2