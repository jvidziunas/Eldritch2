/*==================================================================*\
  WorldView.hpp
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
#include <Renderer/Direct3D11/SwapChain.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Foundation/WorldView.hpp>
#include <Animation/Armature.hpp>
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
		namespace Direct3D11 {
			class	MeshResourceView;
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class WorldView : public Foundation::WorldView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref WorldView instance.
		WorldView( Foundation::World& owningWorld, const Direct3D11::MeshResourceView& defaultMesh );

		~WorldView() = default;

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		static WorldView&	GetActiveWorldView();

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
			SceneCameraComponent( Direct3D11::SwapChain& swapChain, WorldView& owningView = GetActiveWorldView() );

			//! Destroys this @ref SceneCameraComponent instance.
			~SceneCameraComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
			//! Registers all script-callable methods for the @ref SceneCameraComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const								scriptTypeName;

		private:
			mutable Scripting::ObjectHandle<Direct3D11::SwapChain>	_swapChain;
		};

	// ---

		class MeshComponent : public Scripting::ReferenceTypeBase {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref MeshComponent instance.
			MeshComponent( const Direct3D11::MeshResourceView& mesh, Animation::Armature& armature );

			//! Destroys this @ref MeshComponent instance.
			~MeshComponent() = default;

		// ---------------------------------------------------

			//! Registers all script-callable methods for the @ref MeshComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------
	
			static const char* const								scriptTypeName;

		private:
			const Direct3D11::MeshResourceView&						_mesh;
			mutable Scripting::ObjectHandle<Animation::Armature>	_armature;

			::Eldritch2::uint16										_castsShadows		: 1u;
			::Eldritch2::uint16										_receivesShadows	: 1u;
			::Eldritch2::uint16										_visible			: 1u;
		};

	// - DATA MEMBERS ------------------------------------

		const Direct3D11::MeshResourceView&						_defaultMesh;
		::Eldritch2::ObjectPoolAllocator<MeshComponent>			_meshPool;
		::Eldritch2::IntrusiveForwardList<SceneCameraComponent>	_attachedCameras;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2