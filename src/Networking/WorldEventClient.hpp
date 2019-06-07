/*==================================================================*\
  WorldEventClient.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking {

	class ETPureAbstractHint WorldEventClient {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldEventClient instance.
		WorldEventClient(const WorldEventClient&) ETNoexceptHint = default;
		//!	Constructs this @ref WorldEventClient instance.
		WorldEventClient() ETNoexceptHint = default;

		~WorldEventClient() ETNoexceptHint = default;

		// ---------------------------------------------------

	public:
		virtual Result AcceptPacket(Span<const byte*> bytes);

		virtual Result AcceptPlayerJoinRequest();

		virtual Result AcceptPlayerLeaveRequest();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		WorldEventClient& operator=(const WorldEventClient&) = delete;
	};

}} // namespace Eldritch2::Networking
