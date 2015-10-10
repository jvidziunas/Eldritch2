/*==================================================================*\
  Direct3D11WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Utility/Memory/ObjectPoolAllocator.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Foundation/WorldView.hpp>
#include <Renderer/Camera.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}

	namespace Animation {
		class	Armature;
	}

	namespace Renderer {
		class	Direct3D11SwapChain;
		class	Direct3D11MeshView;
	}
}

namespace Eldritch2 {
namespace Renderer {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Animation		= ::Eldritch2::Animation;
	namespace Renderer		= ::Eldritch2::Renderer;

// ---------------------------------------------------

	class Direct3D11WorldView : public Foundation::WorldView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Direct3D11WorldView instance.
		Direct3D11WorldView( Foundation::World& owningWorld, const Renderer::Direct3D11MeshView& defaultMesh );

		//! Destroys this @ref Direct3D11WorldView instance.
		~Direct3D11WorldView();

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		static Renderer::Direct3D11WorldView&	GetActiveWorldView();

	// ---------------------------------------------------

		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PreScriptTickTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PostScriptTickTaskVisitor ) override sealed;

	// - TYPE PUBLISHING ---------------------------------

		class SceneCameraComponent : public ::Eldritch2::IntrusiveForwardListBaseHook, public Scripting::ReferenceTypeBase, public Renderer::Camera {
		// - TYPE PUBLISHING ---------------------------------

		public:
			struct Viewport {
				::Eldritch2::float32	offsetX;
				::Eldritch2::float32	offsetY;
				::Eldritch2::float32	width;
				::Eldritch2::float32	height;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref SceneCameraComponent instance.
			SceneCameraComponent( Renderer::Direct3D11SwapChain& swapChain, Renderer::Direct3D11WorldView& owningView = GetActiveWorldView() );

			//! Destroys this @ref SceneCameraComponent instance.
			~SceneCameraComponent();

		// - SCRIPT API REFERENCE ----------------------------

		public:
			//! Registers all script-callable methods for the @ref SceneCameraComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const										scriptTypeName;

		private:
			mutable Scripting::ObjectHandle<Renderer::Direct3D11SwapChain>	_swapChain;
		};

	// ---

		class MeshComponent : public Scripting::ReferenceTypeBase {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref MeshComponent instance.
			MeshComponent( const Renderer::Direct3D11MeshView& mesh, Animation::Armature& armature );

			//! Destroys this @ref MeshComponent instance.
			~MeshComponent();

		// ---------------------------------------------------

			//! Registers all script-callable methods for the @ref MeshComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------
	
			static const char* const								scriptTypeName;

		private:
			const Renderer::Direct3D11MeshView&						_mesh;
			mutable Scripting::ObjectHandle<Animation::Armature>	_armature;

			::Eldritch2::uint16										_castsShadows		: 1u;
			::Eldritch2::uint16										_receivesShadows	: 1u;
			::Eldritch2::uint16										_visible			: 1u;
		};

	// - DATA MEMBERS ------------------------------------

		const Renderer::Direct3D11MeshView&						_defaultMesh;
		::Eldritch2::ObjectPoolAllocator<MeshComponent>			_meshPool;
		::Eldritch2::IntrusiveForwardList<SceneCameraComponent>	_attachedCameras;
	};

}	// namespace Renderer
}	// namespace Eldritch2