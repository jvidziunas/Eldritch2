/*==================================================================*\
  RawInputSubscriber.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/ReferenceCountable.hpp>
//------------------------------------------------------------------//

struct	tagRAWINPUT;
using RAWINPUT	= tagRAWINPUT;

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	class ETPureAbstractHint RawInputSubscriber : public Core::ReferenceCountable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref RawInputSubscriber instance.
		RawInputSubscriber( const RawInputSubscriber& ) = default;
	//!	Constructs this @ref RawInputSubscriber instance.
		RawInputSubscriber() = default;

		~RawInputSubscriber() = default;

	// ---------------------------------------------------

	public:
	//!	Converts a generic Win32 raw input packet into HID-specific input events.
	/*!	@param[in] inputPacket Packet to parse. */
		virtual void	ReadInputPacket( const RAWINPUT& inputPacket ) abstract;
	};

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2