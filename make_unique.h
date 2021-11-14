#ifndef NoStd_make_unique_h
#define NoStd_make_unique_h

#include <memory>
#include <type_traits>
#include <utility>

#include <boost/type_traits/is_unbounded_array.hpp>

#include "uninitialized.h"

namespace nostd {

  template <typename T, typename... Args>
  typename std::enable_if<boost::is_unbounded_array<T>::value, std::unique_ptr<T>>::type
  make_unique(size_t n, Args&&... args)
  {
    using Type = typename std::remove_extent<T>::type;

    auto storage = new uninitialized_t<Type>[n];
    for (unsigned int i = 0; i < n; ++i) {
      ::new ((void *)&storage[i]) Type(std::forward<Args...>(args...));
    }
    return std::unique_ptr<Type[]>{reinterpret_cast<Type *>(storage)};
  }

}  // namespace nostd

#endif  // NoStd_make_unique_h
