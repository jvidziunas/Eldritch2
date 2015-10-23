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
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Configuration/ConfigurableUTF8String.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <XAudio2.h>
#else
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Sound {
namespace XAudio2 {

	class EngineService : public Foundation::GameEngineService, public ::IXAudio2EngineCallback {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref EngineService instance.
		~EngineService();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) override sealed;

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const ServiceTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

	private:
		::Eldritch2::ErrorCode	InitializeXAudio();

	// ---------------------------------------------------

		void	OnProcessingPassStart() override;

		void	OnProcessingPassEnd() override;

		void	OnCriticalError( ::HRESULT error ) override;

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator									_allocator;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_forcedSpeakerCount;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_processorAffinityMask;
		Configuration::ConfigurableUTF8String						_deviceName;

		::Eldritch2::uint32											_audioGlitchCount;

		Utility::COMPointer<::IXAudio2>								_audio;
	};

}	// namespace XAudio2
}	// namespace Sound
}	// namespace Eldritch2