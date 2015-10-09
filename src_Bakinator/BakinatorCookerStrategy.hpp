/*==================================================================*\
  BakinatorCookerStrategy.hpp
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
#include <Packages/PackageCommon.hpp>
#include <Tools/CookerOption.hpp>
//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace Tools
	{

		namespace FileSystem	= ::Eldritch2::FileSystem;
		namespace Tools			= ::Eldritch2::Tools;

	// ---------------------------------------------------

		class BakinatorCookerStrategy : public Tools::ContentCookerStrategy
		{
		public:
			typedef Tools::BakinatorCookerStrategy							ThisType;
			typedef Tools::ContentCookerStrategy::OptionIterator<ThisType>	OptionIterator;

		// ---------------------------------------------------

			class CompressorOption : public Tools::PODMemberCookerOption<ThisType, const ::Eldritch2::SystemChar*>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, const ::Eldritch2::SystemChar*>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this CompressorOption instance.
				CompressorOption();

				// Destroys this CompressorOption instance.
				~CompressorOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class CompressorArgumentsOption : public Tools::PODMemberCookerOption<ThisType, const ::Eldritch2::SystemChar*>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, const ::Eldritch2::SystemChar*>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this CompressorArgumentsOption instance.
				CompressorArgumentsOption();

				// Destroys this CompressorArgumentsOption instance.
				~CompressorArgumentsOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class AdditionalDependencyOption : public Tools::ArgumentOnlyCookerOption<ThisType>
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				// Constructs this AdditionalDependencyOption instance.
				AdditionalDependencyOption();

				// Destroys this AdditionalDependencyOption instance.
				~AdditionalDependencyOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;

			// ---------------------------------------------------

				bool	Parse( ThisType&						strategy,
							   const ::Eldritch2::SystemChar*	argument,
							   const ::Eldritch2::SystemChar*	argumentEnd ) override sealed;
			};

		// ---------------------------------------------------
		
			class HeaderVersionOption : public Tools::PODMemberCookerOption<ThisType, FileSystem::PackageVersionTag>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, const FileSystem::PackageVersionTag>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this HeaderVersionOption instance.
				HeaderVersionOption();

				// Destroys this HeaderVersionOption instance.
				~HeaderVersionOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;

			// ---------------------------------------------------

			private:
				FileSystem::PackageVersionTag	CoerceToValidValue( FileSystem::PackageVersionTag value ) const override sealed;
			};

		// ---------------------------------------------------

			// Constructs this BakinatorCookerStrategy instance.
			BakinatorCookerStrategy();

			// Destroys this BakinatorCookerStrategy instance.
			~BakinatorCookerStrategy();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Cook( FileSystem::FileWriteAccessStrategy&	output,
										  const ::Eldritch2::SystemChar* const*	sourceFiles,
										  const ::Eldritch2::SystemChar* const*	sourceFilesEnd,
										  Tools::FileReadStrategyFactory&		readStrategyFactory,
										  Tools::FileWriteStrategyFactory&		writeStrategyFactory ) const;

		// ---------------------------------------------------

			const ::Eldritch2::SystemChar*	GetCookedExtension() const override sealed;

		// ---------------------------------------------------

			OptionIterator	OptionsBegin();

			OptionIterator	OptionsEnd();

		// ---------------------------------------------------

		private:
			typedef ::std::vector<const ::Eldritch2::SystemChar*>	DependencyCollection;

		// ---------------------------------------------------

			void	AddDependency( const ::Eldritch2::SystemChar* const dependency );

		// ---------------------------------------------------

			DependencyCollection			_dependencies;

			const ::Eldritch2::SystemChar*	_compressorFourCC;
			const ::Eldritch2::SystemChar*	_compressorArguments;
			::Eldritch2::uint32				_headerVersion;
		};

	}	// namespace Tools

}	// namespace Eldritch2