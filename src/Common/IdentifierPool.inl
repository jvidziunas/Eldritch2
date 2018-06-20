/*==================================================================*\
  IdentifierPool.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename Identifier>
	ETInlineHint IdentifierPool<Identifier>::ValueRange::ValueRange(IdentifierType first, IdentifierType last) :
		first(first),
		last(last) {}

	// ---------------------------------------------------

	template <typename Identifier>
	ETInlineHint bool IdentifierPool<Identifier>::ValueRange::CanMergeForwardWith(const ValueRange& range) const {
		return last == range.first;
	}

	// ---------------------------------------------------

	template <typename Identifier>
	typename IdentifierPool<Identifier>::IdentifierType IdentifierPool<Identifier>::ValueRange::GetSize() const {
		return last - first;
	}

	// ---------------------------------------------------

	template <typename Identifier>
	ETInlineHint bool IdentifierPool<Identifier>::ValueRange::Contains(IdentifierType identifier) const {
		return (first <= identifier) & (identifier < last);
	}

	// ---------------------------------------------------

	template <typename Identifier>
	ETInlineHint bool IdentifierPool<Identifier>::ValueRange::IsEmpty() const {
		return first == last;
	}

} // namespace Detail

template <typename Identifier, class Allocator>
ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool(std::initializer_list<ValueRange> ranges, const AllocatorType& allocator) :
	IdentifierPool(allocator) {
	for (const auto& range : ranges) {
		ReleaseRange(range);
	}
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool(ValueRange initialRange, const AllocatorType& allocator) :
	IdentifierPool(allocator) {
	ReleaseRange(initialRange);
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool(const AllocatorType& allocator) :
	_freeRanges(allocator) {}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool(IdentifierPool<Identifier, Allocator>&& pool) :
	_freeRanges(eastl::move(pool._freeRanges)) {}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
Pair<Identifier, bool> IdentifierPool<Identifier, Allocator>::Allocate() {
	return AllocateRange(static_cast<Identifier>(1));
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
Pair<Identifier, bool> IdentifierPool<Identifier, Allocator>::AllocateRange(IdentifierType sizeInElements) {
	const auto candidate(eastl::find_if(_freeRanges.Begin(), _freeRanges.End(), [sizeInElements](const ValueRange& range) {
		return sizeInElements <= range.GetSize();
	}));

	if (candidate == _freeRanges.End()) {
		return { 0, false };
	}

	const auto firstIdentifier(candidate->first);

	candidate->first += sizeInElements;

	if (candidate->IsEmpty() && ((candidate + 1) != _freeRanges.End())) {
		//	If current range is full and there is another one, that will become the new current range.
		_freeRanges.Erase(candidate);
	}

	return { firstIdentifier, true };
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
void IdentifierPool<Identifier, Allocator>::Release(IdentifierType identifier) {
	return ReleaseRange({ identifier, identifier + static_cast<IdentifierType>(1) });
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
void IdentifierPool<Identifier, Allocator>::ReleaseRange(ValueRange releasedRange) {
	const auto candidate(eastl::lower_bound(_freeRanges.Begin(), _freeRanges.End(), releasedRange, [](const ValueRange& lhs, const ValueRange& rhs) {
		return lhs.last <= rhs.first;
	}));

	if (candidate == _freeRanges.End()) {
		_freeRanges.Append(releasedRange);
		return;
	}

	if (releasedRange.CanMergeForwardWith(*candidate)) {
		candidate->last = releasedRange.last;

		if (candidate + 1 != _freeRanges.End() && candidate->CanMergeForwardWith(candidate[1])) {
			candidate->last = candidate[1].last;
			_freeRanges.Erase(candidate + 1);
		}

		return;
	}

	if (candidate->CanMergeForwardWith(releasedRange)) {
		candidate->first = releasedRange.first;

		if (candidate != _freeRanges.Begin() && candidate[-1].CanMergeForwardWith(*candidate)) {
			candidate[-1].last = candidate->first;
			_freeRanges.Erase(candidate - 1);
		}

		return;
	}

	_freeRanges.Insert(candidate, releasedRange);

#if 0
	//	Binary search of the range list
		AvailableRangeCollection::SizeType	i0(0);
		AvailableRangeCollection::SizeType	i1(_availableRanges.Size() - 1);

		for (; ;) {
			const AvailableRangeCollection::SizeType	i((i0 + i1) / 2);

			if (releasedRange.firstIdentifier < _availableRanges[i].begin) {
			//	Before current range, check if neighboring
				if (releasedRange.lastIdentifier >= _availableRanges[i].begin) {
				//	Overlaps a range of free IDs, thus (at least partially) invalid IDs
					ETRuntimeAssert(releasedRange.lastIdentifier == _availableRanges[i].begin);

				//	Neighbor id, check if neighboring previous range too
					if (i > i0 && releasedRange.firstIdentifier - 1 == _availableRanges[i - 1].end) {
					//	Merge with previous range
						_availableRanges[i - 1].end = _availableRanges[i].end;
						_availableRanges.Erase(_availableRanges.Begin() + i);
					}
					else {
				 //	Just grow range
						_availableRanges[i].begin = releasedRange.firstIdentifier;
					}

					break;
				}
				else {
			 //	Non-neighbor id
					if (i != i0) {
					//	Cull upper half of list
						i1 = i - 1;
					}
					else {
				 //	Found our position in the list, insert the deleted range here
						_availableRanges.Insert(_availableRanges.Begin() + i, AvailableRange(releasedRange.firstIdentifier, releasedRange.lastIdentifier - 1));
						break;
					}
				}
			}
			else if (releasedRange.firstIdentifier > _availableRanges[i].end) {
		 //	After current range, check if neighboring
				if (releasedRange.firstIdentifier - 1 == _availableRanges[i].end) {
				//	Neighbor id, check if neighboring next range too
					if (i < i1 && releasedRange.lastIdentifier == _availableRanges[i + 1].begin) {
					//	Merge with next range
						_availableRanges[i].end = _availableRanges[i + 1].end;
						_availableRanges.Erase(_availableRanges.Begin() + i + 1);
					}
					else {
				 //	Just grow range
						_availableRanges[i].end += (releasedRange.lastIdentifier - releasedRange.firstIdentifier);
					}

					break;
				}
				else {
			 //	Non-neighbor id
					if (i != i1) {
					//	Cull bottom half of list
						i0 = i + 1;
					}
					else {
				 //	Found our position in the list, insert the deleted range here
						_availableRanges.Insert(_availableRanges.Begin() + i + 1, AvailableRange(releasedRange.firstIdentifier, releasedRange.lastIdentifier - 1));
						break;
					}
				}
			}
			else {
		 //	Inside a free block, not a valid ID
				ETRuntimeAssert(false);
			}
		}
#endif
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint void IdentifierPool<Identifier, Allocator>::Clear() {
	_freeRanges.Clear();
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint bool IdentifierPool<Identifier, Allocator>::IsEmpty() const {
	return _freeRanges.IsEmpty();
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint typename IdentifierPool<Identifier, Allocator>::IdentifierType IdentifierPool<Identifier, Allocator>::GetLargestSpanLength() const {
	IdentifierType longestRangeLength(0);

	for (const ValueRange& range : _freeRanges) {
		longestRangeLength = Max(longestRangeLength, range.GetSize());
	}

	return longestRangeLength;
}

} // namespace Eldritch2
