#pragma once

#ifndef INCLUDE_STLALLOCATOR_H
#define INCLUDE_STLALLOCATOR_H

#include "../utility/TypeDefine.h"
#include "aegisAllocator.h"

namespace aegis
{
    template <typename T>
    class stl_allocatorWrapper {
    public:
        // 確保する型
        using value_type = T;

        // デフォルトコンストラクタ、コピーコンストラクタ、ムーブコンストラクタ
        stl_allocatorWrapper();
        stl_allocatorWrapper(const stl_allocatorWrapper&) {}
        stl_allocatorWrapper(stl_allocatorWrapper&&) {}

        // 別のテンプレート実引数から生成するためのコンストラクタ
        template <typename U>
        stl_allocatorWrapper(const stl_allocatorWrapper<U>&) {}

        // メモリ領域を確保
        T* allocate(aegis::uint64 num)
        {
            const aegis::uint64 size = num * sizeof(T);
            return static_cast<T*>(memory::AegisAllocator::allocate(size, memory::AllocatorType::Default));
        }

        // メモリ領域を解放
        void deallocate(T* p, aegis::uint64 num)
        {
            memory::AegisAllocator::deallocate(p, num, memory::AllocatorType::Default);
        }

        template <typename T, typename... Args>
        void construct(T* p, Args&&... args)
        {
            ::new(p) T(std::forward<Args>(args)...);
        }

        template <typename T>
        void destroy(T* p)
        {
            p->~T();
        }

    private:
        static std::once_flag mOnce[2];
    };

    template<typename T>
    std::once_flag stl_allocatorWrapper<T>::mOnce[2];

    template <typename T1, typename T2>
    bool operator==(const stl_allocatorWrapper<T1>&, const stl_allocatorWrapper<T2>&)
    {
        return true;
    }

    template <typename T1, typename T2>
    bool operator!=(const stl_allocatorWrapper<T1>&, const stl_allocatorWrapper<T2>&)
    {
        return false;
    }
    template<typename T>
    inline stl_allocatorWrapper<T>::stl_allocatorWrapper()
    {
        aegis::memory::AegisAllocator::initAllocator();
        aegis::memory::AegisAllocator::initMemoryPool();
    }
}

#endif // !INCLUDE_STLALLOCATOR_H
