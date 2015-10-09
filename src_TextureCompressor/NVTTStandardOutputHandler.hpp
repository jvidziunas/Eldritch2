/*==================================================================*\
  NVTTStandardOutputHandler.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
#include <nvtt/nvtt.h>
//------------------------------------------------------------------//

namespace Eldritch2
{
	namespace FileSystem
	{
		class FileWriteAccessStrategy;
	}
}

namespace Eldritch2
{

	namespace Tools
	{

		namespace Tools	= ::Eldritch2::Tools;

	// ---------------------------------------------------

		class NVTTStandardOutputHandler : public ::nvtt::OutputHandler
		{
		public:
			 NVTTStandardOutputHandler( FileSystem::FileWriteAccessStrategy& fileAccessor );
			~NVTTStandardOutputHandler();

		// ---------------------------------------------------
			
			void	beginImage( int size, int width, int height, int depth, int face, int miplevel );

			void	endImage();

		// ---------------------------------------------------

			bool	writeData( const void* data, int size );

			template <typename T>
			bool	WriteObject( const T& object )
			{
				return this->writeData( &object, sizeof( object ) );
			}

		// ---------------------------------------------------

		private:
			const Tools::NVTTStandardOutputHandler&	operator=( const Tools::NVTTStandardOutputHandler& handler );

		// ---------------------------------------------------

			FileSystem::FileWriteAccessStrategy&	_accessor;
		};

	}	// namespace Tools

}	// namespace Eldritch2