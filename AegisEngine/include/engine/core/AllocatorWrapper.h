#pragma once

#ifndef INCLUDE_ALLOCATORWRAPPER_H
#define INCLUDE_ALLOCATORWRAPPER_H

#include "utility\TypeDefine.h"
#include "memory\aegisAllocator.h"

namespace aegis
{
	//template <typename T>
	class allocatorWrapper
	{
	public:
		//! メモリ領域を確保
		template <typename T, typename... Args>
		static T* allocate(aegis::uint64 num = 1, Args&&... args)
		{
			const aegis::uint64 size = num * sizeof(T);
			T* ptr = static_cast<T*>(memory::AegisAllocator::allocate(size, memory::AllocatorType::Default));

			construct(ptr, std::forward<Args>(args)...);

			return ptr;
		}

		//! メモリ領域を解放
		template <typename T>
		static void deallocate(T* p, aegis::uint64 num = 1)
		{
			//destroy(p);
			memory::AegisAllocator::deallocate(p, num, memory::AllocatorType::Default);
		}

		template <typename T, typename... Args>
		static void construct(T* p, Args&&... args)
		{
			::new(p) T(std::forward<Args>(args)...);
		}

		template <typename T>
		static void destroy(T* p)
		{
			p->~T();
		}
	};
}

#endif // !INCLUDE_ALLOCATORWRAPPER_H
