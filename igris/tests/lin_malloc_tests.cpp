#include "doctest/doctest.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "../compat/mem/lin_malloc.h"

extern "C" {
void *lin_malloc_malloc(size_t len);
void lin_malloc_free(void *p);
void *lin_malloc_realloc(void *ptr, size_t len);

extern char *__malloc_heap_start;
extern char *__brkval;
extern __freelist *__flp;
extern int __allocation_counter;
}

// Provide the linker symbol that the allocator expects as the start of SRAM.
char _heap_start;

namespace
{
    constexpr size_t kFakeHeapSize = 2048;
    alignas(std::max_align_t) std::array<unsigned char, kFakeHeapSize>
        fake_heap;

    void reset_fake_heap()
    {
        std::fill(fake_heap.begin(), fake_heap.end(), 0);
        __malloc_heap_start = reinterpret_cast<char *>(fake_heap.data());
        __brkval = nullptr;
        __flp = nullptr;
        __allocation_counter = 0;
    }

    bool pointer_in_heap(void *ptr, size_t size)
    {
        auto *begin = fake_heap.data();
        auto *end = fake_heap.data() + fake_heap.size();
        auto *cptr = static_cast<unsigned char *>(ptr);
        return cptr >= begin && (cptr + size) <= end;
    }
}

TEST_CASE("lin_malloc allocates inside the fake heap buffer")
{
    reset_fake_heap();

    auto *ptr = static_cast<unsigned char *>(lin_malloc_malloc(32));
    REQUIRE(ptr != nullptr);
    CHECK(pointer_in_heap(ptr, 32));
    CHECK(__allocation_counter == 1);
}

TEST_CASE("lin_malloc_free returns blocks to the freelist")
{
    reset_fake_heap();

    auto *ptr = lin_malloc_malloc(48);
    auto *ptr2 = lin_malloc_malloc(16);
    REQUIRE(ptr != nullptr);
    REQUIRE(ptr2 != nullptr);

    lin_malloc_free(ptr);
    auto *reused = lin_malloc_malloc(40);
    CHECK(reused == ptr);

    lin_malloc_free(ptr2);
    lin_malloc_free(reused);

    auto *merged = lin_malloc_malloc(80);
    CHECK(merged != nullptr);
    CHECK(pointer_in_heap(merged, 80));
}

TEST_CASE("lin_malloc_realloc can grow within the fake heap")
{
    reset_fake_heap();

    auto *ptr = static_cast<unsigned char *>(lin_malloc_malloc(32));
    REQUIRE(pointer_in_heap(ptr, 32));

    auto *grown = static_cast<unsigned char *>(lin_malloc_realloc(ptr, 64));
    REQUIRE(grown != nullptr);
    CHECK(pointer_in_heap(grown, 64));
}
