#include <ulib/allocators/fastmemallocator.h>
#include <ulib/allocators/mallocallocator.h>
#include <ulib/allocators/growlinearallocator.h>
#include <ulib/allocators/staticallocator.h>
#include <ulib/allocators/handledstaticallocator.h>
#include <ulib/allocators/tempallocator.h>

#include <ulib/containers/list.h>
#include <ulib/random/uniquegenerator.h>
#include <ulib/perf/perf.h>

#include <string>
#include <vector>
#include <random>

void StatDescDump(const ulib::perf::StatisticsDescription &desc)
{
    printf("- min: %.10fs\n- max: %.10fs\n- avg: %.10fs\n- sum: %.10fs\n- calls: %llu\n- minCall: %llu\n- maxCall: %llu\n",
           ulib::perf::ToSeconds(desc.min), ulib::perf::ToSeconds(desc.max), ulib::perf::ToSeconds(desc.avg.Value()), ulib::perf::ToSeconds(desc.sum), desc.avg.Count(),
           desc.minCallIndex, desc.maxCallIndex);
}

void PerfDump(const std::string &name)
{
    if (ulib::perf::Statistics *pStatistics = ulib::perf::Status(name))
    {
        if (!pStatistics->ready)
        {
            printf("perf %s is not ready\n", name.c_str());
            return;
        }

        printf("\n--- perf dump start ---\n");

        if (pStatistics->mDescs.size() > 1)
        {
            printf("\n[%s]: (all)\n", name.c_str());
            StatDescDump(*pStatistics);
        }

        for (auto &obj : pStatistics->mDescs)
        {
            printf("\n[%s]: (%s)\n", name.c_str(), obj.first.c_str());
            StatDescDump(obj.second);
        }

        printf("\n--- perf dump end --- \n\n");
    }
    else
    {
        printf("perf %s not found\n", name.c_str());
    }
}

const size_t kBlockSize = 0x200000;
const size_t kAllocationCount = 4000;
const size_t kMinAlloc = 5000;
const size_t kMaxAlloc = 100;

template <class AllocatorT>
void TestAllocator(const std::string &name, AllocatorT &alloc)
{
    std::vector<void *> ptrs;
    ptrs.resize(kAllocationCount);

    std::random_device rd;
    std::mt19937 g(rd());

    {
        {
            ulib::perf::Test test(name + ".alloc");

            for (size_t i = 0; i != kAllocationCount; i++)
            {
                ptrs[i] = alloc.Alloc(kMinAlloc + rand() % 0x100);
            }
        }

        size_t index = rand() % kAllocationCount;
        alloc.Free(ptrs[index]);
        ptrs[index] = alloc.Alloc(0x10000);

        std::shuffle(ptrs.begin(), ptrs.end(), g);

        {
            ulib::perf::Test test(name + ".free");

            for (size_t i = 0; i != kAllocationCount; i++)
            {
                if (ptrs[i])
                    alloc.Free(ptrs[i]);
            }
        }
    }
}

template <class AllocatorT>
__declspec(noinline) void RepeatTestAllocator(const std::string &name, AllocatorT &alloc, size_t repeatCount)
{
    for (size_t i = 0; i != repeatCount; i++)
        TestAllocator<AllocatorT>(name, alloc);
}

template <class LinearAllocatorT>
__declspec(noinline) void RepeatTestLinearAllocator(const std::string &name, LinearAllocatorT &alloc, size_t repeatCount)
{
    for (size_t i = 0; i != repeatCount; i++)
    {
        TestAllocator<LinearAllocatorT>(name, alloc);
        alloc.Clear();
    }
}

/*
namespace regar
{
    template <class T>
    using MyAllocator = ulib::StandardAllocator<T, ulib::StaticAllocator<ulib::FastMemAllocator<ulib::MallocAllocator>, 0>>;

    template <class T>
    using vector = std::vector<T, MyAllocator<T>>;
}
*/

// using MainAllocator = ulib::StaticAllocator<ulib::GrowLinearAllocator<ulib::MallocAllocator>, 0>;
using MainAllocator = ulib::StaticAllocator<ulib::FastMemAllocator<ulib::MallocAllocator>>;
using StaticFastMemAllocator = MainAllocator;

/*
void *operator new(size_t size)
{
    return MainAllocator::Alloc(size);
}

void operator delete(void *ptr)
{
    if (ptr)
        MainAllocator::Free(ptr);
}
*/

void AllocatorsMain()
{
    srand(time(0));

    std::mutex mt;
    

    /*
        ulib::FastMemAllocator<ulib::MallocAllocator> fastmem;
    RepeatTestAllocator("ulib.fastmem", fastmem, 2000);

    ulib::StaticAllocator<ulib::SlotAllocator<char[kMinAlloc], ulib::MallocAllocator>, 2> slotAllocator;
    RepeatTestAllocator("ulib.slot", slotAllocator, 2000);

    ulib::StaticPointerAllocator<ulib::SlotAllocator<char[kMinAlloc], ulib::MallocAllocator>, 2, ulib::MallocAllocator> slot2Allocator;
    RepeatTestAllocator("ulib.slot2", slot2Allocator, 2000);
    */

    // ulib::MallocAllocator mallocAllocator;
    //RepeatTestAllocator("std.malloc", mallocAllocator, 2000);

    ulib::FastMemAllocator<ulib::MallocAllocator> fastmem;
    RepeatTestAllocator("ulib.fastmem", fastmem, 2000);

     ulib::TempAllocatorT tempalloc;
    RepeatTestAllocator("ulib.tempalloc", tempalloc, 2000);

    // ulib::GrowLinearAllocator<ulib::MallocAllocator> grow;
    //RepeatTestLinearAllocator("ulib.grow", grow, 2000);

    //PerfDump("std.malloc.alloc");
    //PerfDump("std.malloc.free");

    PerfDump("ulib.fastmem.alloc");
    PerfDump("ulib.fastmem.free");

    PerfDump("ulib.tempalloc.alloc");
    PerfDump("ulib.tempalloc.free");

    //PerfDump("ulib.grow.alloc");
    //PerfDump("ulib.grow.free");

    //PerfDump("ulib.slot.alloc");
    //PerfDump("ulib.slot.free");

    //PerfDump("ulib.slot2.alloc");
    //PerfDump("ulib.slot2.free");
}