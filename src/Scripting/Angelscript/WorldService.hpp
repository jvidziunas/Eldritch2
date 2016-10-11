/*==================================================================*\
  WorldService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/AngelScript/SmartPointers.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Core/WorldService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Assets {
		class	AssetLibrary;
	}
}

class	asIScriptEngine;
class	asITypeInfo;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class WorldService : public Core::WorldService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref WorldService instance.
		WorldService( const Core::World& world );
	//!	Disable copying.
		WorldService( const WorldService& ) = delete;

		~WorldService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	public:
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStartVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStopVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, FixedTickVisitor& visitor ) override;

		void	AcceptVisitor( Scheduling::JobFiber& executor, FixedTickVisitor& visitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ScriptTickVisitor ) override;

	// ---------------------------------------------------

	protected:
		UniquePointer<asIScriptObject>	Spawn( const Eldritch2::Utf8Char* const classPath, const asITypeInfo* requiredInterface = nullptr );
		
	// ---------------------------------------------------

	//!	Disable assignment.
		WorldService&	operator=( const WorldService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::ChildLog	_log;

		Eldritch2::Utf8String<>		_rulesName;
		Eldritch2::Utf8String<>		_baseWorldAssetName;

		asIScriptEngine&			_scriptEngine;
		const Assets::AssetLibrary&	_assetLibrary;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2