#pragma once

#ifndef INCLUDE_AEGISCONTAINER_H
#define INCLUDE_AEGISCONTAINER_H

#include "utility\TypeDefine.h"
#include "memory\stlAllocator.h"


#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <deque>
#include <stack>
#include <variant>
#include <string>

#include <memory>

#include "../../../common.h"

namespace aegis
{
    template <class T, class D = AllocatorDelete<T>>
    using uniquePtr = std::unique_ptr<T, D>;

    template <class T, uint64 Size>
    using array = std::array<T, Size>;

    template <class T>
    using vector = std::vector<T, stl_allocatorWrapper<T>>;

    template <class T>
    using list = std::list<T, stl_allocatorWrapper<T>>;

    template <class Key,
        class T,
        class Compare = std::less<Key>>
    using map = std::map<Key, T, Compare, stl_allocatorWrapper<std::pair<const Key, T>>>;

    template <class Key,
        class T,
        class Hash = std::hash<Key>,
        class Pred = std::equal_to<Key>>
    using unordered_map = std::unordered_map<Key, T, Hash, Pred, stl_allocatorWrapper<std::pair<const Key, T>>>;

    template <class Key,
        class Compare = std::less<Key>>
    using set = std::set<Key, Compare, stl_allocatorWrapper<Key>>;

    template <class Key,
        class Hash = std::hash<Key>,
        class Pred = std::equal_to<Key>>
    using unordered_set = std::unordered_set<Key, Hash, Pred, stl_allocatorWrapper<Key>>;

    template <class T>
    using deque = std::deque<T, stl_allocatorWrapper<T>>;

    template <class T>
    using stack = std::stack<T, stl_allocatorWrapper<T>>;

#if _MSVC_LANG > 201703L
    // c++20
#elif _MSVC_LANG > 201402L
    // c++17
    template <class... Types>
    using variant = std::variant<Types...>;
#else
    // c++14以下
#endif

    using string = std::basic_string<char8, std::char_traits<char8>, stl_allocatorWrapper<char8>>;
    using wstring = std::basic_string<char16, std::char_traits<char16>, stl_allocatorWrapper<char16>>;
}

#endif // !INCLUDE_AEGISCONTAINER_H
