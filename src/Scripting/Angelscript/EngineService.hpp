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
#include <Scripting/AngelScript/SmartPointers.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Core/EngineService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {
		class	Engine;
	}
}

class	asIScriptContext;
class	asIScriptEngine;
struct	asSMessageInfo;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class EngineService : public Core::EngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref EngineService instance.
		EngineService( const Core::Engine& engine );
	//!	Disable copying.
		EngineService( const EngineService& );

		~EngineService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	public:
		Eldritch2::Result<Eldritch2::UniquePointer<Core::WorldService>>	CreateWorldService( Eldritch2::Allocator& allocator, const Core::World& world ) override;

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const BeginInitializationVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const InitializationCompleteVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ServiceTickVisitor ) override;
		void	AcceptVisitor( Assets::AssetViewFactoryRegistrar& registrar ) override;
		void	AcceptVisitor( Core::ServiceBlackboard& blackboard ) override;
		void	AcceptVisitor( Scripting::ApiRegistrar& registrar ) override;

	// - SCRIPT MARSHALING -------------------------------

	protected:
		Eldritch2::Utf8String<>	MarshalStringLiteral( const unsigned int literalLengthInOctets, const Eldritch2::Utf8Char* const literal );

		void					MessageCallback( const asSMessageInfo* messageInfo );

	// ---------------------------------------------------

	protected:
		asIScriptContext*	GetContext();

		void				ReturnContext( asIScriptContext* context );

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator				_allocator;
		mutable Logging::ChildLog						_log;

		bool											_forceExport;

		AngelScript::UniquePointer<asIScriptEngine>		_scriptEngine;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2