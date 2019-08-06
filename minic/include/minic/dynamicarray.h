#pragma once
#include "minic/allocator.h"
#include "minic/int.h"

template <typename T>
struct DynamicArray
{
    explicit DynamicArray(Allocator &allocator) : allocator(allocator)
    {
    }

	DynamicArray(Allocator &allocator, usize capacity) : allocator(allocator)
	{

	}

	DynamicArray(const DynamicArray<T> &copy) : allocator(copy.allocator)
	{

	}

    DynamicArray(DynamicArray<T> &&move) : allocator(move.allocator)

private:
    T *data;

    usize count;
    usize capacity;

    Allocator &allocator;
};
