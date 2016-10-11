/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Core/EngineService.hpp>
#include <Utility/ComPointer.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <XAudio2.h>
#else
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {
		class	Engine;
	}
}

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	class EngineService : public Core::EngineService, public IXAudio2EngineCallback {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref EngineService instance.
		EngineService( const Core::Engine& owningEngine );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

		~EngineService();

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	public:
		Eldritch2::Result<Eldritch2::UniquePointer<Core::WorldService>>	CreateWorldService( Eldritch2::Allocator& allocator, const Core::World& world ) override;

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ConfigurationBroadcastVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ServiceTickVisitor ) override;
		void	AcceptVisitor( Configuration::ConfigurationRegistrar& registrar ) override;
		void	AcceptVisitor( Scripting::ApiRegistrar& registrar ) override;

	// ---------------------------------------------------

	private:
		void	OnCriticalError( HRESULT error ) override;

		void	OnProcessingPassStart() override;

		void	OnProcessingPassEnd() override;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator	_allocator;
		mutable Logging::ChildLog			_log;

		Eldritch2::uint32					_forcedSpeakerCount;
		Eldritch2::uint32					_processorAffinityMask;
		Eldritch2::Utf8String<>				_deviceName;

		Eldritch2::uint32					_audioGlitchCount;
		Eldritch2::uint64					_audioProcessingPassTick;

		Eldritch2::ComPointer<IXAudio2>		_audio;
	};

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2