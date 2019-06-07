/*==================================================================*\
  FlatBufferPackageProvider.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/ContentDatabase.hpp>
#include <Logging/FileLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace FlatBuffers {

	class FlatBufferContentDatabase : public Core::ContentDatabase {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class DeserializationThread : public Thread {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref DeserializationThread instance.
			DeserializationThread(Core::ContentDatabase& content) ETNoexceptHint;
			//!	Disable copy construction.
			DeserializationThread(const DeserializationThread&) = delete;

			~DeserializationThread();

			// ---------------------------------------------------

		public:
			bool ShouldRun(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

			void SetShouldShutDown() ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			Result EnterOnCaller() ETNoexceptHint override;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			DeserializationThread& operator=(const DeserializationThread&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			Atomic<RunBehavior>    _runBehavior;
			Logging::FileLog       _log;
			Core::ContentDatabase* _content;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		FlatBufferContentDatabase(const FlatBufferContentDatabase&) = delete;
		//!	Constructs this @ref FlatBufferPackageProvider instance.
		FlatBufferContentDatabase() ETNoexceptHint;

		~FlatBufferContentDatabase() = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Core::AssetApiBuilder api);

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		FlatBufferContentDatabase& operator=(const FlatBufferContentDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		DeserializationThread _deserializer;
	};

}} // namespace Eldritch2::FlatBuffers

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <FlatBuffers/FlatBufferContentDatabase.inl>
//------------------------------------------------------------------//
