#pragma once

namespace ray::core::memory
{
	
class IAllocator
{
public:
	virtual ~IAllocator() = default;

	/**
	 * \brief Allocate a block of memory with allocator.
	 * \param size the size of allocation.
	 * \param alignment the alignment of memory block.
	 * \return the pointer to allocated memory block.
	 */
	virtual void* Alloc(size_t size, size_t alignment) = 0;

	/**
	 * \brief 
	 * \param memory 
	 */
	virtual void Free(void* memory) = 0;
	
	/**
	 * \brief 
	 * \param memory 
	 * \param size 
	 * \param alignment 
	 * \return 
	 */
	virtual void* Realloc(void* memory, size_t size, size_t alignment) = 0;

	/**
	 * \brief 
	 * \return 
	 */
	virtual size_t GetAllocationSize(void*) = 0;
};

}
