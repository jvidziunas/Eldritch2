/*==================================================================*\
  Memory.inl
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

namespace Eldritch2 {

ETConstexpr ETPureFunctionHint size_t GetHashCode(const void* pointer, size_t seed) ETNoexceptHint {
	return uintptr(pointer) + 31u * seed;
}

// ---------------------------------------------------

ETCpp14Constexpr ETPureFunctionHint size_t HashMemory(const void* memory, size_t byteLength, size_t seed) ETNoexceptHint {
	struct SizeHelper {
		static ETCpp14Constexpr ETPureFunctionHint uint64 Mix(uint64& value) ETNoexceptHint {
			value ^= value >> 23;
			value *= 0x2127599bf4325c37ULL;
			value ^= value >> 47;

			return value;
		}

		static ETCpp14Constexpr ETPureFunctionHint uint64 Hash(const void* buffer, size_t byteLength, uint64 seed) ETNoexceptHint {
			enum : uint64 { m = 0x880355f21e6d1965ull };

			// ---

			uint64 hash(seed ^ (byteLength * m));
			auto   pos(static_cast<const uint64*>(buffer));

			for (const auto end(pos + (byteLength / sizeof(pos[0]))); pos != end;) {
				uint64 v = *pos++;
				hash ^= Mix(v);
				hash *= m;
			}

			const auto pos2(reinterpret_cast<const unsigned char*>(pos));
			uint64     v(0ull);

			switch (byteLength & 7) {
			case 7: v ^= uint64(pos2[6]) << 48;
			case 6: v ^= uint64(pos2[5]) << 40;
			case 5: v ^= uint64(pos2[4]) << 32;
			case 4: v ^= uint64(pos2[3]) << 24;
			case 3: v ^= uint64(pos2[2]) << 16;
			case 2: v ^= uint64(pos2[1]) << 8;
			case 1:
				v ^= uint64(pos2[0]);
				hash ^= Mix(v);
				hash *= m;
			} // switch(lengthInBytes & 7)

			return Mix(hash);
		}

		// ---------------------------------------------------

		static ETCpp14Constexpr ETPureFunctionHint uint32 Hash(const void* buffer, size_t byteLength, uint32 seed) ETNoexceptHint {
			/*	Converts the 64-bit hash code to Fermat residue, which retains information from both the higher and
			 *	lower parts of the code. */
			const uint64 hash(Hash(buffer, byteLength, uint64(seed)));
			return uint32(hash - (hash >> 32));
		}
	};

	return SizeHelper::Hash(memory, byteLength, seed);
}

} // namespace Eldritch2
