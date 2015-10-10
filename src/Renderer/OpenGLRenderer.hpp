/*==================================================================*\
  OpenGLRenderer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurableVariable.hpp>
#include <Foundation/GameEngineService.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationDatabase;
	}

	namespace Foundation {
		class	Window;
	}
}

namespace Eldritch2 {
namespace Renderer {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Renderer		= ::Eldritch2::Renderer;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class OpenGLRenderer : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Initializes this OpenGLRenderer instance.
		OpenGLRenderer( ::Eldritch2::Allocator& allocator );

		// Destroys this OpenGLRenderer instance.
		~OpenGLRenderer();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	Bind( Foundation::Application&	application,
									  Foundation::GameEngine&	engine,
									  Scheduler::TaskScheduler&	scheduler,
									  Scheduler::Task&			bindCompleteContinuation ) override sealed;

		::Eldritch2::ErrorCode	Tick( Foundation::Application&	application,
									  Foundation::GameEngine&	engine,
									  Scheduler::TaskScheduler&	scheduler,
									  Scheduler::Task&			frameCompleteContinuation ) override sealed;

	// ---------------------------------------------------

		void	AcceptInitializationVisitor( Configuration::ConfigurationDatabase& database ) override;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator		_allocator;

		Configuration::ConfigurableBool	_allowBindlessTextures;
		Configuration::ConfigurableBool	_allowSparseTextures;
	};

}	// namespace Renderer
}	// namespace Eldritch2