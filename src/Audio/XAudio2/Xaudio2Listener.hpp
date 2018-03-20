/*==================================================================*\
  XAudio2Listener.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/Listener.hpp>
//------------------------------------------------------------------//
#if ET_PLATFORM_WINDOWS
#	include <windows.h>
#endif
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/X3DAudio.h>
#else
#	include <X3DAudio.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Transformation;
}

struct	IXAudio2MasteringVoice;
struct	IXAudio2SourceVoice;
struct	IXAudio2Voice;

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	class XAudio2Voice : public Voice {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref XAudio2Voice instance.
		XAudio2Voice( float32 loudnessDb, IXAudio2SourceVoice* sourceVoice, IXAudio2Voice* effectVoice );
	//!	Disable copy construction.
		XAudio2Voice( const XAudio2Voice& ) = delete;
	//!	Constructs this @ref XAudio2Voice instance.
		XAudio2Voice( XAudio2Voice&& );

		~XAudio2Voice();

	// ---------------------------------------------------

	public:
		void	UpdateDspSettings( const X3DAUDIO_HANDLE settings, const X3DAUDIO_LISTENER& listener, float32 timeScalar ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		X3DAUDIO_EMITTER		_emitter;

		IXAudio2SourceVoice*	_sourceVoice;
		IXAudio2Voice*			_effectVoice;
	};

// ---

	class XAudio2Listener : public Listener<XAudio2Voice> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref XAudio2Listener instance.
		XAudio2Listener( Transformation localToWorld );
	//!	Disable copy construction.
		XAudio2Listener( const XAudio2Listener& ) = delete;

		~XAudio2Listener() = default;

	// ---------------------------------------------------

	public:
		Transformation ETSimdCall	GetLocalToWorld() const;
		Transformation ETSimdCall	GetWorldToLocal() const;

		void ETSimdCall				SetLocalToWorld( Transformation value );

	// ---------------------------------------------------

	public:
		void	UpdateVoices( const X3DAUDIO_HANDLE settings, float32 timeScalar );

	// - DATA MEMBERS ------------------------------------

	private:
		X3DAUDIO_LISTENER		_listener;

		IXAudio2MasteringVoice*	_inWorldMaster;
		IXAudio2MasteringVoice*	_musicMaster;
	};

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2