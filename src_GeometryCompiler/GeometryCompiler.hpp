/*==================================================================*\
  GeometryCompiler.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Tools/ToolCRTPBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	class GeometryCompiler : public Tools::ToolCRTPBase<GeometryCompiler<GlobalAllocator, FileAccessorFactory>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref GeometryCompiler instance.
		GeometryCompiler();

		~GeometryCompiler() = default;

	// ---------------------------------------------------

		ETInlineHint FileAccessorFactory&	GetFileAccessorFactory();

		//!	Retrieves the @ref Allocator the tool uses to perform internal memory allocations.
		/*!	@returns A reference to the @ref GlobalAllocator the tool should use to make memory allocations.
			*/
		ETInlineHint GlobalAllocator&		GetAllocator();

	// ---------------------------------------------------

		void	RegisterOptions( OptionRegistrationVisitor& visitor );

		int		Process();

	// ---------------------------------------------------

	protected:
		int	SetPassword( const Eldritch2::Utf8Char* const password, const Eldritch2::Utf8Char* const passwordEnd );

		int	AddInputFile( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd );

		int SetSkipTextureCoordinates( const bool value );

		int	SetSkipOrientation( const bool value );

	// - DATA MEMBERS ------------------------------------

	private:
		GlobalAllocator										_allocator;
		FileAccessorFactory									_fileAccessorFactory;

		Eldritch2::Utf8String<>								_password;
		Eldritch2::ResizableArray<Eldritch2::Utf8String<>>	_inputNames;
		bool												_skipTextureCoordinates;
		bool												_skipOrientation;
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <GeometryCompiler.inl>
//------------------------------------------------------------------//