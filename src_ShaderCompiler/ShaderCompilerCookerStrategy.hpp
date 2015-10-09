/*==================================================================*\
  ShaderCompilerCookerStrategy.hpp
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

		namespace FileSystem	= ::Eldritch2::FileSystem;
		namespace Tools			= ::Eldritch2::Tools;

	// ---------------------------------------------------

		class ShaderCompilerCookerStrategy : public Tools::ContentCookerStrategy
		{
		public:
			typedef Tools::ShaderCompilerCookerStrategy						ThisType;
			typedef Tools::ContentCookerStrategy::OptionIterator<ThisType>	OptionIterator;

		// ---------------------------------------------------

			enum D3DShaderProfile
			{
				_4_0_level_9_1,
				_4_0_level_9_3,
				_4_0,
				_4_1,
				_5_0,

				COUNT,
				DEFAULT	= _5_0
			};

		// ---------------------------------------------------

			enum ShaderClass
			{
				UNKNOWN,
				VERTEX_SHADER,
				HULL_SHADER,
				DOMAIN_SHADER,
				GEOMETRY_SHADER,
				PIXEL_SHADER,
				COMPUTE_SHADER,
			};

		// ---------------------------------------------------

			class ShaderProfileOption : public Tools::EnumCookerOption<ThisType, ThisType::D3DShaderProfile>
			{
			public:
				typedef Tools::EnumCookerOption<ThisType, ThisType::D3DShaderProfile>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this ShaderProfileOption instance.
				ShaderProfileOption();

				// Destroys this ShaderProfileOption instance.
				~ShaderProfileOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class OptimizationLevelOption : public Tools::PODMemberCookerOption<ThisType, ::Eldritch2::uint32>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, ::Eldritch2::uint32>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this OptimizationLevelOption instance.
				OptimizationLevelOption();

				// Destroys this ShaderProfileOption instance.
				~OptimizationLevelOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;

			// ---------------------------------------------------

			protected:
				::Eldritch2::uint32	CoerceToValidValue( ::Eldritch2::uint32 value ) const override sealed;
			};

		// ---------------------------------------------------

			class WarningsAreErrorsOption : public PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this WarningsAreErrorsOption instance.
				WarningsAreErrorsOption();

				// Destroys this WarningsAreErrorsOption instance.
				~WarningsAreErrorsOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class StripDebugInfoOption : public PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this StripDebugInfoOption instance.
				StripDebugInfoOption();

				// Destroys this StripDebugInfoOption instance.
				~StripDebugInfoOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this ShaderCompilerCookerStrategy instance.
			ShaderCompilerCookerStrategy();

			// Destroys this ShaderCompilerCookerStrategy instance.
			~ShaderCompilerCookerStrategy();

		// ---------------------------------------------------

			OptionIterator	OptionsBegin();
			OptionIterator	OptionsEnd();

		// ---------------------------------------------------

			const ::Eldritch2::SystemChar*	GetCookedExtension() const;

			void							ProcessContentExtensionHint( const ::Eldritch2::SystemChar*	contentPath,
																		 const ::Eldritch2::SystemChar*	contentExtension );			

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Cook( FileSystem::FileWriteAccessStrategy&	output,
										  const ::Eldritch2::SystemChar* const*	sourceFiles,
										  const ::Eldritch2::SystemChar* const*	sourceFilesEnd,
										  Tools::FileReadStrategyFactory&		readStrategyFactory,
										  Tools::FileWriteStrategyFactory&		writeStrategyFactory ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			::Eldritch2::uint32	_optimizationLevel;
			D3DShaderProfile	_shaderProfile;
			ShaderClass			_shaderClass;
			bool				_warningsAreErrors;
			bool				_stripDebugInfo;
		};

	}	// namespace Tools

}	// namespace Eldritch2