#pragma once

#include "minic/allocator.h"

struct CAllocator : public Allocator
{
public:
    CAllocator();

protected:
    virtual void *alloc(usize bytes) override;
    virtual void *realloc(void *ptr, usize bytes) override;
    virtual void free(void *ptr) override;

private:
    void *handle;

    // Inherited via Allocator
};
