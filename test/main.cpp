#include <ulib/allocators/pager.h>
#include <ulib/perf/perf.h>
#include <ulib/allocators/mallocallocator.h>
#include <ulib/allocators/fastmemallocator.h>
#include <ulib/allocators/pmr.h>
#include <ulib/allocators/synchronizedallocator.h>
#include <ulib/allocators/standardallocator.h>
#include <ulib/allocators/slotallocator.h>
#include <ulib/allocators/growlinearallocator.h>
#include <ulib/allocators/staticallocator.h>
#include <ulib/containers/list.h>
#include <ulib/containers/queue.h>

#include <algorithm>
#include <random>
#include <memory_resource>
#include <vector>
#include <assert.h>

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
using MainAllocator = ulib::StaticAllocator<ulib::FastMemAllocator<ulib::MallocAllocator>, 0>;
using StaticFastMemAllocator = MainAllocator;

void *operator new(size_t size)
{
    return MainAllocator::Alloc(size);
}

void operator delete(void *ptr)
{
    if (ptr)
        MainAllocator::Free(ptr);
}

__declspec(noinline) void testvec()
{
    std::vector<std::string> vstr;
    for (int i = 0; i != 200; i++)
    {
        vstr.push_back("hello");
    }

    {
        ulib::perf::Test test("std.erase");

        vstr.erase(vstr.begin() + 2);
        vstr.erase(vstr.begin() + 3);
        vstr.erase(vstr.begin() + 1);
    }
}

__declspec(noinline) void testvecl()
{
    ulib::List<std::string> vstr;
    for (int i = 0; i != 200; i++)
    {
        vstr.push_back("hello");
    }

    {
        ulib::perf::Test test("ulib.erase");

        vstr.erase(vstr.begin() + 2);
        vstr.erase(vstr.begin() + 3);
        vstr.erase(vstr.begin() + 1);
    }
}

int main()
{
    try
    {


        std::vector<std::string> ky;
        std::string arr[20];
        ky.insert(ky.begin(), std::move(std::begin(arr)), std::move(std::end(arr)));

        ulib::List<std::string, ulib::MallocAllocator> strs = {"hi", "ky", "pizdec"};
        auto l2 = std::move(strs);
        auto l3 = ulib::List<std::string, ulib::MallocAllocator>({"1", "2"});
        l3 = std::move(l2);

        system("pause");
        return 0;

        /*
        regar::vector<int> vec({});

        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);

        */

        srand(time(0));

        /*
            ulib::FastMemAllocator<ulib::MallocAllocator> fastmem;
        RepeatTestAllocator("ulib.fastmem", fastmem, 2000);



        ulib::StaticAllocator<ulib::SlotAllocator<char[kMinAlloc], ulib::MallocAllocator>, 2> slotAllocator;
        RepeatTestAllocator("ulib.slot", slotAllocator, 2000);

        ulib::StaticPointerAllocator<ulib::SlotAllocator<char[kMinAlloc], ulib::MallocAllocator>, 2, ulib::MallocAllocator> slot2Allocator;
        RepeatTestAllocator("ulib.slot2", slot2Allocator, 2000);
        */

        ulib::MallocAllocator mallocAllocator;
        RepeatTestAllocator("std.malloc", mallocAllocator, 2000);

        ulib::FastMemAllocator<ulib::MallocAllocator> fastmem;
        RepeatTestAllocator("ulib.fastmem", fastmem, 2000);

        ulib::GrowLinearAllocator<ulib::MallocAllocator> grow;
        RepeatTestLinearAllocator("ulib.grow", grow, 2000);

        PerfDump("std.malloc.alloc");
        PerfDump("std.malloc.free");

        PerfDump("ulib.fastmem.alloc");
        PerfDump("ulib.fastmem.free");

        PerfDump("ulib.slot.alloc");
        PerfDump("ulib.slot.free");

        PerfDump("ulib.slot2.alloc");
        PerfDump("ulib.slot2.free");

        PerfDump("ulib.grow.alloc");
        PerfDump("ulib.grow.free");
    }
    catch (const std::exception &e)
    {
        printf("exception: %s\n", e.what());
    }

    return 0;
}