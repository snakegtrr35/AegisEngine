#pragma once

#ifndef INCLUDE_AEGISALLOCATOR_H
#define INCLUDE_AEGISALLOCATOR_H

#include "../utility/TypeDefine.h"
#include <shared_mutex>

namespace aegis
{
    namespace memory
    {
        enum class AllocatorType : uint32
        {
            Boot,
            Default,
            Resource,
            Temp,
            Develop,
            Max
        };

        class AegisAllocator
        {
        public:
            // メモリ領域を確保
            static void* allocate(const uint64 num, AllocatorType type);
            static void* allocate(const uint64 num, std::align_val_t alignment, AllocatorType type);

            // メモリ領域を解放
            static void deallocate(void* p, const uint64 num, AllocatorType type);
            static void deallocate(void* p, std::align_val_t alignment, AllocatorType type);
            static void deallocate(void* p, const uint64 num, std::align_val_t alignment, AllocatorType type);

            static void addMemoey(const uint64 num, AllocatorType type);
            static void subMemoey(const uint64 num, AllocatorType type);

            static uint64 getTotalSize(AllocatorType type);

            static void initAllocator();
            static void initMemoryPool();
            static void uninitAllocator();

        private:
            static uint64 mTotalSize[static_cast<uint32>(AllocatorType::Max)];
            static std::shared_mutex mLockObjs[static_cast<uint32>(AllocatorType::Max)];
            static std::once_flag mOnce[2];

            static bool mIsInit;
        };

        namespace detail
        {
            void* allocate(const uint64 num, uint32 type);
            void deallocate(void* p, uint32 type);
            void initAllocate();
            void uninitAllocate();
        }
    }
}

#endif // !INCLUDE_AEGISALLOCATOR_H
