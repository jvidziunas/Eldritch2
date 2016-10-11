/*==================================================================*\
  Bakinator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
	class Bakinator : public Tools::ToolCRTPBase<Bakinator<GlobalAllocator, FileAccessorFactory>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Bakinator instance.
		Bakinator();

		~Bakinator() = default;

	// ---------------------------------------------------

		ETInlineHint FileAccessorFactory&	GetFileAccessorFactory();

	//!	Retrieves the @ref GlobalAllocator the tool uses to perform internal memory allocations.
	/*!	@returns A reference to the @ref GlobalAllocator the tool should use to make memory allocations. */
		ETInlineHint GlobalAllocator&		GetAllocator();

	// ---------------------------------------------------

		void	RegisterOptions( OptionRegistrationVisitor& visitor );

		int		Process();

	// ---------------------------------------------------

	protected:
		int	SetOutputFileName( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd );

		int	AddImport( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd );

		int	AddExport( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd );

	// - DATA MEMBERS ------------------------------------

	private:
		GlobalAllocator										_allocator;
		FileAccessorFactory									_fileAccessorFactory;

		Eldritch2::Utf8String<>								_outputFileName;
		Eldritch2::Utf8String<>								_outputDataBlobName;
		Eldritch2::ResizableArray<Eldritch2::Utf8String<>>	_importNames;
		Eldritch2::ResizableArray<Eldritch2::Utf8String<>>	_exportNames;
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Bakinator.inl>
//------------------------------------------------------------------//