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
#include <Scripting/ReferenceCountable.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Renderer/VisibilitySystem.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Foundation/WorldView.hpp>
#include <Animation/Armature.hpp>
#include <Renderer/Camera.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptApiRegistrationInitializationVisitor;
	}

	namespace Animation {
		class	Armature;
	}

	namespace Renderer {
		namespace Direct3D11 {
			class	HLSLPipelineDefinitionView;
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
		WorldView( Foundation::World& owningWorld );

		~WorldView() = default;

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;

	// ---------------------------------------------------

		void	OnPreScriptTick( Scheduler::WorkerContext& executingContext ) override sealed;

	// ---------------------------------------------------

		static WorldView&	GetActiveWorldView();

	// - TYPE PUBLISHING ---------------------------------

		using MaterialUsageCache = ::Eldritch2::HashMap<Direct3D11::HLSLPipelineDefinitionView*, int>;

	// ---

		class SceneCamera : public ::Eldritch2::IntrusiveForwardListBaseHook, public Scripting::ReferenceCountable, public Renderer::Camera {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref SceneCamera instance.
			SceneCamera( Direct3D11::SwapChain& swapChain, WorldView& owningView = GetActiveWorldView() );

			~SceneCamera() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
			//! Registers all script-callable methods for the @ref SceneCamera type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const								scriptTypeName;

		private:
			mutable Scripting::ObjectHandle<Direct3D11::SwapChain>	_swapChain;
			MaterialUsageCache										_cachedUsages;
		};

	// ---

		class MeshRenderer : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref MeshRenderer instance.
			MeshRenderer( Animation::Armature& armature, const Direct3D11::MeshResourceView& mesh );

			~MeshRenderer() = default;

		// ---------------------------------------------------

			//! Registers all script-callable methods for the @ref MeshRenderer type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------
	
			static const char* const								scriptTypeName;

		private:
			const Direct3D11::MeshResourceView&						_mesh;
			mutable Scripting::ObjectHandle<Animation::Armature>	_armature;

			::Eldritch2::uint16										_castsShadows	: 1u;
			::Eldritch2::uint16										_visible		: 1u;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ObjectPoolAllocator							_meshAllocator;
		::Eldritch2::IntrusiveForwardList<SceneCamera>				_attachedCameras;

		Renderer::VisibilitySystem<Direct3D11::MeshResourceView>	_visibilitySystem;

		MaterialUsageCache											_shadowMaterialUsageCache;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2