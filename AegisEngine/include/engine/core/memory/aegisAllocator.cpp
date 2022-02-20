#include "aegisAllocator.h"
#include "../aegisContainer.h"
#include <tlsf.h>

#pragma comment(lib, "external/tlsf/lib/tlsf.lib")

namespace aegis
{
    namespace
    {
        static uint64 InitDataSize = 10 * 1024 * 1024;

        static tlsf_t gMemoryPools[static_cast<uint32>(memory::AllocatorType::Max)] = { nullptr };
        //static std::unique_ptr<void, Delete> gMemoryPools[static_cast<uint32>(memory::AllocatorType::Max)] = { nullptr };
        //static std::vector<pool_t> gAddtionalPools = std::vector<pool_t>(16);

        static uint64 gTotalSize[static_cast<uint32>(memory::AllocatorType::Max)] = {};
    }

    namespace memory
    {
        namespace detail
        {
            //! スマートポインタ用のカスタムデリーター(配列版)
            struct Delete_Array {
                template<class T>
                void operator()(T* p) const noexcept
                {
                    delete[] p;
                    p = nullptr;
                }
            };

            void initMemoryPool();
            void* getMemoryPool(uint32 type);
            aegis::vector<std::unique_ptr<void, Delete_Array>>& getAddtionalMemoryPool();

            void* allocate(const uint64 num, uint32 type)
            {
                void* ptr = tlsf_malloc(gMemoryPools[type], num);

                if (nullptr == ptr)
                {
                    const uint64 size = num * 2;
                    aegis::vector<std::unique_ptr<void, Delete_Array>>& addtionalPools = getAddtionalMemoryPool();
                    addtionalPools.emplace_back(tlsf_add_pool(gMemoryPools[type], ::new uint8[size], size));

                    ptr = tlsf_malloc(gMemoryPools[type], num);

                    gTotalSize[type] += size;
                }

                return ptr;
            }

            void deallocate(void* p, uint32 type)
            {
                tlsf_free(gMemoryPools[type], p);
            }

            void initAllocate()
            {
                const uint64 dataSize = 10 * 1024 * 1024;
                const uint64 size = static_cast<int>(aegis::memory::AllocatorType::Max);

                for (int i = 0; i < size; i++)
                {
                    //gMemoryPools[i] = ( tlsf_create_with_pool(::new uint8[dataSize], dataSize) );
                    gMemoryPools[i] = (tlsf_create_with_pool(getMemoryPool(i), dataSize));
                    gTotalSize[i] = InitDataSize;
                }
            }

            void uninitAllocate()
            {
                const uint64 size = static_cast<int>(aegis::memory::AllocatorType::Max);

                //for (auto& ptr : gAddtionalPools)
                //{
                //    if (nullptr != ptr)
                //    {
                //        ::delete ptr;
                //        ptr = nullptr;
                //    }
                //}

                for (int i = 0; i < size; i++)
                {
                    //::delete gMemoryPools[i];
                    //gMemoryPools[i] = nullptr;
                }
            }

            void* getMemoryPool(uint32 type)
            {
                static std::unique_ptr<uint8[], Delete_Array> MemoryPools[static_cast<uint32>(memory::AllocatorType::Max)] =
                {
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[InitDataSize]),
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[InitDataSize]),
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[InitDataSize]),
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[InitDataSize]),
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[InitDataSize])
                };

                return MemoryPools[type].get();
            }

            void initMemoryPool()
            {
                getAddtionalMemoryPool();
            }

            aegis::vector<std::unique_ptr<void, Delete_Array>>& getAddtionalMemoryPool()
            {
                static bool once = true;
                static aegis::vector<std::unique_ptr<void, Delete_Array>> AddtionalPools;
                if (once)
                {
                    once = false;
                    AddtionalPools.reserve(16);
                }

                return AddtionalPools;
            }
        }

        uint64 AegisAllocator::mTotalSize[static_cast<uint32>(AllocatorType::Max)] = { InitDataSize };
        std::shared_mutex AegisAllocator::mLockObjs[static_cast<uint32>(AllocatorType::Max)];
        std::once_flag AegisAllocator::mOnce[2];
        bool AegisAllocator::mIsInit = false;


        void* AegisAllocator::allocate(const uint64 num, AllocatorType type)
        {
            std::lock_guard lock(mLockObjs[static_cast<uint32>(type)]);

            const uint32 typeNum = static_cast<uint32>(type);

            //void* p = ::operator new(num);
            void* p = detail::allocate(num, typeNum);

            mTotalSize[typeNum] += num;
            return p;
        }

        void* AegisAllocator::allocate(const uint64 num, std::align_val_t alignment, AllocatorType type)
        {
            std::lock_guard lock(mLockObjs[static_cast<uint32>(type)]);

            const uint32 typeNum = static_cast<uint32>(type);

            //void* p = ::operator new(num);
            void* p = detail::allocate(num, typeNum);

            mTotalSize[typeNum] += num;
            return p;
        }

        void AegisAllocator::deallocate(void* p, const uint64 num, AllocatorType type)
        {
            std::lock_guard lock(mLockObjs[static_cast<uint32>(type)]);

            const uint32 typeNum = static_cast<uint32>(type);

            mTotalSize[typeNum] -= num;

            //::operator delete(p);
            detail::deallocate(p, typeNum);
        }

        void AegisAllocator::deallocate(void* p, std::align_val_t alignment, AllocatorType type)
        {
            std::lock_guard lock(mLockObjs[static_cast<uint32>(type)]);

            const uint32 typeNum = static_cast<uint32>(type);

            mTotalSize[typeNum] -= static_cast<uint64>(alignment);

            //::operator delete(p);
            detail::deallocate(p, typeNum);
        }

        void AegisAllocator::deallocate(void* p, const uint64 num, std::align_val_t alignment, AllocatorType type)
        {
            std::lock_guard lock(mLockObjs[static_cast<uint32>(type)]);

            const uint32 typeNum = static_cast<uint32>(type);

            mTotalSize[typeNum] -= num;

            //::operator delete(p);
            detail::deallocate(p, typeNum);
        }

        void AegisAllocator::addMemoey(const uint64 num, AllocatorType type)
        {
            std::lock_guard lock(mLockObjs[static_cast<uint32>(type)]);

            mTotalSize[static_cast<uint32>(type)] += num;
        }

        void AegisAllocator::subMemoey(const uint64 num, AllocatorType type)
        {
            std::lock_guard lock(mLockObjs[static_cast<uint32>(type)]);

            mTotalSize[static_cast<uint32>(type)] -= num;
        }

        uint64 AegisAllocator::getTotalSize(AllocatorType type)
        {
            std::shared_lock lock(mLockObjs[static_cast<uint32>(type)]);

            //return mTotalSize[static_cast<uint32>(type)];
            return gTotalSize[static_cast<uint32>(type)];
        }

        void AegisAllocator::initAllocator()
        {
            std::call_once(mOnce[0], detail::initAllocate);
        }

        void AegisAllocator::initMemoryPool()
        {
            //std::call_once(mOnce[1], detail::initMemoryPool);
            static bool once = true;
            if (once)
            {
                once = false;
                detail::getAddtionalMemoryPool();
            }
        }

        void AegisAllocator::uninitAllocator()
        {
            detail::uninitAllocate();
        }
    }
}
