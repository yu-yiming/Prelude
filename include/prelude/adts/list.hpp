#pragma once

#include "../defs.hpp"
#include "../utils/meta.hpp"

namespace prelude {

template<typename List, 
         typename CIt = typename List::const_iterator_type,
         typename CRef = typename List::const_reference_type,
         typename It = typename List::iterator_type,
         typename Ref = typename List::reference_type,
         typename Size = typename List::size_type,
         typename T = typename List::value_type>
concept list_type = requires (List list, List const clist) {
    { list.back() }                 -> same_type<Ref>;
    { clist.back() }                -> same_type<CRef>;
    { list.begin() }                -> same_type<It>;
    { clist.begin() }               -> same_type<CIt>;
    { list.cbegin() }               -> same_type<CIt>;
    { list.cend() }                 -> same_type<CIt>;
    { list.clear() };
    { list.empty() }                -> same_type<bool>;
    { list.end() }                  -> same_type<It>;
    { clist.end() }                 -> same_type<CIt>;
    { list.erase(declval<CIt>()) };
    { list.front() }                -> same_type<Ref>;
    { clist.front() }               -> same_type<CRef>;
    { list.insert(declval<CIt>()) };
    { list.pop_back() };
    { list.pop_front() };
    { list.push_back(declval<T const&>()) };
    { list.push_back(declval<T&&>()) };
    { list.push_front(declval<T const&>()) };
    { list.push_front(declval<T&&>()) };
    { list.size() }                 -> same_type<Size>;
    { list[declval<Size>()] }       -> same_type<Ref>;
    { clist[declval<Size>()] }      -> same_type<CRef>;
};

template<typename T, typename Alloc>
class array_list {

};


} // namespace prelude