#include "aegisAllocator.h"
#include "../aegisContainer.h"
#include <tlsf.h>

#pragma comment(lib, "external/tlsf/lib/tlsf.lib")

namespace aegis
{
    namespace
    {
        static tlsf_t gMemoryPools[static_cast<uint32>(memory::AllocatorType::Max)] = { nullptr };
        //static std::unique_ptr<void, Delete> gMemoryPools[static_cast<uint32>(memory::AllocatorType::Max)] = { nullptr };
        //static std::vector<pool_t> gAddtionalPools = std::vector<pool_t>(16);
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
                    aegis::vector<std::unique_ptr<void, Delete_Array>>& addtionalPools = getAddtionalMemoryPool();
                    addtionalPools.emplace_back(tlsf_add_pool(gMemoryPools[type], ::new uint8[num * 2], num * 2));

                    ptr = tlsf_malloc(gMemoryPools[type], num);
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
                constexpr uint64 dataSize = 10 * 1024 * 1024;
                static std::unique_ptr<uint8[], Delete_Array> MemoryPools[static_cast<uint32>(memory::AllocatorType::Max)] =
                {
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[dataSize]),
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[dataSize]),
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[dataSize]),
                    std::unique_ptr<uint8[], Delete_Array>(new uint8[dataSize])
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

        uint64 AegisAllocator::mTotalSize[static_cast<uint32>(AllocatorType::Max)] = { 0 };
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

        uint64 AegisAllocator::getTotalSize()
        {
            std::shared_lock lock(mLockObjs[0]);

            return mTotalSize[0];
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
