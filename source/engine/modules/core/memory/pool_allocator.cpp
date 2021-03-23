#include "pool_allocator.hpp"

#pragma clang diagnostic ignored "-Wbraced-scalar-init"

size_t PoolAllocator::Allocate(size_t size) noexcept
{
	auto smallestFragmentBySize = _fragmentsBySize.lower_bound(size);
        if (smallestFragmentBySize == _fragmentsBySize.end())
            return POOL_ALLOCATOR_ERROR;

	auto smallestFragmentByOffset = smallestFragmentBySize->second;
	size_t offset = smallestFragmentByOffset->first;

	size_t newOffset = offset + size;
	size_t newSize = smallestFragmentByOffset->second.Size - size;

    _fragmentsBySize.erase(smallestFragmentBySize);
    _fragmentsByOffset.erase(smallestFragmentByOffset);

    if (newSize > 0)
		AddNewFragment(newOffset, newSize);

    auto lastElement = _fragmentsBySize.end();
    _maxAvailableSpace = _fragmentsBySize.size() != 0 ? 
       (--lastElement)->first : 0;

	return offset;
}

void PoolAllocator::Free(size_t offset, size_t size) noexcept
{
	MemoryFragmentsByOffsetMap::iterator nextBlockIt, prevBlockIt;
   nextBlockIt = _fragmentsByOffset.upper_bound(offset);
        prevBlockIt = nextBlockIt;
        if (prevBlockIt != _fragmentsByOffset.begin())
            --prevBlockIt;
        else
            prevBlockIt = _fragmentsByOffset.end(); 

    size_t newSize, newOffset;
    if (prevBlockIt != _fragmentsByOffset.end() && offset == prevBlockIt->first + prevBlockIt->second.Size)
    {
        // PrevBlock.offset           offset
        // |                          |
        // |<-----PrevBlock.Size----->|<------Size-------->|
        //
        newSize = prevBlockIt->second.Size + size;
        newOffset = prevBlockIt->first;

        if (nextBlockIt != _fragmentsByOffset.end() && offset + size == nextBlockIt->first)
        {
            // PrevBlock.offset           offset               NextBlock.offset 
            // |                          |                    |
            // |<-----PrevBlock.Size----->|<------Size-------->|<-----NextBlock.Size----->|
            //
            newSize += nextBlockIt->second.Size;
            _fragmentsBySize.erase(prevBlockIt->second.OrderBySizeIt);
            _fragmentsBySize.erase(nextBlockIt->second.OrderBySizeIt);
            // Delete the range of two blocks
            ++nextBlockIt;
            _fragmentsByOffset.erase(prevBlockIt, nextBlockIt);
        }
        else
        {
            // PrevBlock.offset           offset                       NextBlock.offset 
            // |                          |                            |
            // |<-----PrevBlock.Size----->|<------Size-------->| ~ ~ ~ |<-----NextBlock.Size----->|
            //
            _fragmentsBySize.erase(prevBlockIt->second.OrderBySizeIt);
            _fragmentsByOffset.erase(prevBlockIt);
        }
    }
    else if (nextBlockIt != _fragmentsByOffset.end() && offset + size == nextBlockIt->first)
    {
        // PrevBlock.offset                   offset               NextBlock.offset 
        // |                                  |                    |
        // |<-----PrevBlock.Size----->| ~ ~ ~ |<------Size-------->|<-----NextBlock.Size----->|
        //
        newSize = size + nextBlockIt->second.Size;
        newOffset = offset;
        _fragmentsBySize.erase(nextBlockIt->second.OrderBySizeIt);
        _fragmentsByOffset.erase(nextBlockIt);
    }
    else
    {
        // PrevBlock.offset                   offset                       NextBlock.offset 
        // |                                  |                            |
        // |<-----PrevBlock.Size----->| ~ ~ ~ |<------Size-------->| ~ ~ ~ |<-----NextBlock.Size----->|
        //
        newSize = size;
        newOffset = offset;
    }

    AddNewFragment(newOffset, newSize);

    auto lastElement = _fragmentsBySize.end();
    _maxAvailableSpace = (--lastElement)->first;
}

void PoolAllocator::AddNewFragment(size_t offset, size_t size) noexcept
{
    auto newfragmentIt = _fragmentsByOffset.emplace(offset, size);
	auto orderIt = _fragmentsBySize.emplace(size, newfragmentIt.first);
	newfragmentIt.first->second.OrderBySizeIt = orderIt;
}