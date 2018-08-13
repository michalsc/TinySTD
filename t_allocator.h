//

#ifndef _T_ALLOCATOR_H
#define _T_ALLOCATOR_H

#include <stdio.h>
#include <typeinfo>
#include "support.h"

namespace t_std {

template <class T>
class allocator {

public:
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef const value_type*   const_pointer;
    typedef uintptr_t           size_type;
    typedef ptrdiff_t           difference_type;

    allocator() : construct_count(0), deconstruct_count(0), allocate_count(0), deallocate_count(0) { printf("Allocator @ %p for type %s\n", (void*)this, typeid(T).name()); };
    allocator(const allocator&) : allocator() {};
    ~allocator() { printf("Destroying allocator @ %p for type %s\n", (void*)this, typeid(T).name()); 
        if (construct_count != deconstruct_count || allocate_count != deallocate_count) printf("Constructed: %d, Deconstructed: %d\nAllocated: %d, Deallocated: %d\n", construct_count, deconstruct_count, allocate_count, deallocate_count); }
    pointer address(reference x) { return &x; }
    const_pointer address(const_reference x) { return &x; }
    pointer allocate(size_type n) { pointer p = (pointer)AllocMem(n * sizeof(value_type), MEMF_CLEAR); printf("t_std::allocate(%p, %d)=%p\n", (void*)this, (int)n, (void*)p); allocate_count++; return p; }
    void deallocate(pointer p, size_type n) { printf("t_std::deallocate(%p, %p, %d)\n", (void*)this, (void*)p, (int)n); if (n>0 && p!=nullptr) FreeMem((void*)p, n * sizeof(T)); deallocate_count++; }
    size_type max_size() { return (size_type)-1 / sizeof(value_type); }
    void construct(pointer p, const_reference val) { new((void*)p) value_type(val); construct_count++;}
    void destroy(pointer p) { p->~value_type(); deconstruct_count++; }

private:
    int construct_count;
    int deconstruct_count;
    int allocate_count;
    int deallocate_count;
};

}

#endif // _T_ALLOCATOR_H