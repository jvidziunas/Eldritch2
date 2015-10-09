/*==================================================================*\
  ScriptCompilerCookerStrategy.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/ContentCookerStrategy.hpp>
#include <Tools/CookerOption.hpp>
//------------------------------------------------------------------//

struct	asSMessageInfo;

namespace Eldritch2
{
	class	ErrorCode;

	namespace Tools
	{
		class	FileReadStrategyFactory;
		class	FileWriteStrategyFactory;
	}

	namespace FileSystem
	{
		class	FileWriteAccessStrategy;
	}
}

namespace Eldritch2
{

	namespace Tools
	{

		namespace Tools			= ::Eldritch2::Tools;
		namespace FileSystem	= ::Eldritch2::FileSystem;

	// ---------------------------------------------------

		class ScriptCompilerCookerStrategy : public Tools::ContentCookerStrategy
		{
		public:
			typedef Tools::ScriptCompilerCookerStrategy						ThisType;
			typedef Tools::ContentCookerStrategy::OptionIterator<ThisType>	OptionIterator;

		// ---------------------------------------------------

			class OptimizeBytecodeOption : public Tools::PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this OptimizeBytecodeOption instance.
				OptimizeBytecodeOption();

				// Destroys this OptimizeBytecodeOption instance.
				~OptimizeBytecodeOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this ScriptCompilerCookerStrategy instance.
			ScriptCompilerCookerStrategy();

			// Destroys this ScriptCompilerCookerStrategy instance.
			~ScriptCompilerCookerStrategy();

		// ---------------------------------------------------

			OptionIterator	OptionsBegin();

			OptionIterator	OptionsEnd();

		// ---------------------------------------------------

			const ::Eldritch2::SystemChar*	GetCookedExtension() const;		

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Cook( FileSystem::FileWriteAccessStrategy&	output,
										  const ::Eldritch2::SystemChar* const*	sourceFiles,
										  const ::Eldritch2::SystemChar* const*	sourceFilesEnd,
										  Tools::FileReadStrategyFactory&		readStrategyFactory,
										  Tools::FileWriteStrategyFactory&		writeStrategyFactory ) const;

		// ---------------------------------------------------

		private:
			static void	MessageCallback( const ::asSMessageInfo* const						msg,
										 const Tools::ScriptCompilerCookerStrategy* const	cooker );

		// - DATA MEMBERS ------------------------------------

			bool	_optimizeBytecode;
		};

	}	// namespace Tools

}	// namespace Eldritch2