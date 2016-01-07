/*==================================================================*\
  ScriptCompiler.hpp
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
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/SystemString.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Tools/ToolCRTPBase.hpp>
//------------------------------------------------------------------//
#include <angelscript/sdk/add_on/scriptbuilder/scriptbuilder.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	class ScriptCompilerTool : public Tools::ToolCRTPBase<ScriptCompilerTool<GlobalAllocator, FileAccessorFactory>> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using	FileAccessorFactoryType = FileAccessorFactory;
		using	AllocatorType			= GlobalAllocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref ScriptCompilerTool instance.
		ScriptCompilerTool();

		~ScriptCompilerTool();

	// ---------------------------------------------------

		ETInlineHint FileAccessorFactoryType&	GetFileAccessorFactory();

		ETInlineHint ::CScriptBuilder&			GetScriptBuilder();

		//!	Retrieves the @ref Allocator the compiler uses to perform internal memory allocations.
		/*!	@returns A reference to the @ref AllocatorType the tool should use to make memory allocations.
			*/
		ETInlineHint AllocatorType&				GetAllocator();

	// ---------------------------------------------------

		void	RegisterOptions( OptionRegistrationVisitor& visitor );

		int		Process();

	// ---------------------------------------------------

	protected:
		template <typename MetadataVisitor>
		int	AcceptMetadataVisitor( MetadataVisitor&& visitor );

	// ---------------------------------------------------

		int	SetOutputModuleName( const ::Eldritch2::UTF8Char* const argument, const ::Eldritch2::UTF8Char* const argumentEnd );

		int	SetOptimizationLevel( const int level );

		int	AddInputFile( const ::Eldritch2::UTF8Char* const argument, const ::Eldritch2::UTF8Char* const argumentEnd );

	// - DATA MEMBERS ------------------------------------

	private:
		Scripting::AngelScript::EngineHandle					_engine;
		GlobalAllocator											_globalAllocator;
		FileAccessorFactory										_fileAccessorFactory;
		::CScriptBuilder										_scriptBuilder;
		::Eldritch2::UTF8String<>								_outputModuleName;
		::Eldritch2::ResizableArray<::Eldritch2::UTF8String<>>	_inputFiles;
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <ScriptCompiler.inl>
//------------------------------------------------------------------//