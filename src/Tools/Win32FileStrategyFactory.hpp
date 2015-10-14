/*==================================================================*\
  Win32FileStrategyFactory.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/FileWriteStrategyFactory.hpp>
#include <Tools/FileReadStrategyFactory.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;
	namespace Tools			= ::Eldritch2::Tools;

// ---------------------------------------------------

	class Win32FileStrategyFactory : public Tools::FileReadStrategyFactory, public Tools::FileWriteStrategyFactory {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Win32FileStrategyFactory instance.
		Win32FileStrategyFactory( ::Eldritch2::Allocator& fileDataAllocator );

		//!	Destroys this @ref Win32FileStrategyFactory instance.
		~Win32FileStrategyFactory() = default;

	// - FILE READER OBJECT CONSTRUCTION -----------------

		Utility::Result<FileSystem::FileReadAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const char* filePath, Tools::FileReadAccessorBehavior behaviors = Tools::FileReadAccessorBehavior::DEFAULT ) override sealed;
		Utility::Result<FileSystem::FileReadAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const wchar_t* filePath, Tools::FileReadAccessorBehavior behaviors = Tools::FileReadAccessorBehavior::DEFAULT ) override sealed;

	// - FILE WRITER OBJECT CONSTRUCTION -----------------

		Utility::Result<FileSystem::FileWriteAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const char* filePath, Tools::FileWriteAccessorBehavior behaviors = Tools::FileWriteAccessorBehavior::DEFAULT ) override sealed;
		Utility::Result<FileSystem::FileWriteAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const wchar_t* filePath, Tools::FileWriteAccessorBehavior behaviors = Tools::FileWriteAccessorBehavior::DEFAULT ) override sealed;

	// ---------------------------------------------------

		::Eldritch2::Allocator&	GetFileDataAllocator() override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator	_fileDataAllocator;
	};

}	// namespace Tools
}	// namespace Eldritch2