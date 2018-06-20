/*==================================================================*\
  Precompiled.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
//	Meta-programming library
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/Compiler.hpp>
#include <Common/Mpl/EnableIf.hpp>
#include <Common/Mpl/FloatTypes.hpp>
#include <Common/Mpl/FunctionTraits.hpp>
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Mpl/Matrices.hpp>
#include <Common/Mpl/Platform.hpp>
#include <Common/Mpl/TypeTraits.hpp>
#include <Common/Mpl/Vectors.hpp>

//	Containers library
#include <Common/Containers/ArrayBvh.hpp>
#include <Common/Containers/ArrayList.hpp>
#include <Common/Containers/ArrayMap.hpp>
#include <Common/Containers/ArraySet.hpp>
#include <Common/Containers/Dequeue.hpp>
#include <Common/Containers/HashMap.hpp>
#include <Common/Containers/HashMultiMap.hpp>
#include <Common/Containers/HashSet.hpp>
#include <Common/Containers/IntrusiveConcurrentQueue.hpp>
#include <Common/Containers/IntrusiveForwardList.hpp>
#include <Common/Containers/IntrusiveLinkedList.hpp>
#include <Common/Containers/LinkedList.hpp>
#include <Common/Containers/PlatformString.hpp>
#include <Common/Containers/PriorityQueue.hpp>
#include <Common/Containers/Queue.hpp>
#include <Common/Containers/Range.hpp>
#include <Common/Containers/Stack.hpp>
#include <Common/Containers/String.hpp>
#include <Common/Containers/TreeMap.hpp>
#include <Common/Containers/TreeSet.hpp>

// Memory/Allocator library
#include <Common/Memory/Allocator.hpp>
#include <Common/Memory/ArenaAllocator.hpp>
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Memory/InstanceDeleter.hpp>
#include <Common/Memory/InstanceNew.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Memory/NullAllocator.hpp>
#include <Common/Memory/Prefetch.hpp>
#include <Common/Memory/UsageMixin.hpp>
#include <Common/Memory/Win32HeapAllocator.hpp>

// Common objects library
#include <Common/Algorithms.hpp>
#include <Common/Angle.hpp>
#include <Common/Assert.hpp>
#include <Common/Atomic.hpp>
#include <Common/Blackboard.hpp>
#include <Common/ComPointer.hpp>
#include <Common/CountedPointer.hpp>
#include <Common/EqualTo.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/FileAppender.hpp>
#include <Common/FileReader.hpp>
#include <Common/Filesystem.hpp>
#include <Common/FileWriter.hpp>
#include <Common/Function.hpp>
#include <Common/GoapPlanner.hpp>
#include <Common/GreaterThan.hpp>
#include <Common/Hash.hpp>
#include <Common/IdentifierPool.hpp>
#include <Common/LessThan.hpp>
#include <Common/MappedFile.hpp>
#include <Common/Math.hpp>
#include <Common/Memory.hpp>
#include <Common/Mutex.hpp>
#include <Common/Pair.hpp>
#include <Common/Platform.hpp>
#include <Common/Ratio.hpp>
#include <Common/Result.hpp>
#include <Common/RuleSet.hpp>
#include <Common/ScopeGuard.hpp>
#include <Common/Semaphore.hpp>
#include <Common/Sort.hpp>
#include <Common/Stopwatch.hpp>
#include <Common/StringLiteral.hpp>
#include <Common/SymbolTable.hpp>
#include <Common/Tuple.hpp>
#include <Common/Type.hpp>
#include <Common/UniquePointer.hpp>
#include <Common/WaitableEvent.hpp>
#include <Common/Win32Tools.hpp>
#include <Common/Window.hpp>
//------------------------------------------------------------------//