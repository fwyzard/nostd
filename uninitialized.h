#ifndef NoStd_uninitialized_h
#define NoStd_uninitialized_h

#include <type_traits>

namespace nostd {

  // uninitialized_t<T> should have the same size, alignment, and "cookie" requirement as T
  template <typename T, typename Enable = void>
  struct uninitialized_t;

  template <typename T>
  struct uninitialized_t<T, typename std::enable_if<std::is_trivially_destructible<T>::value>::type>
  {
    uninitialized_t() = default;
    ~uninitialized_t() = default;

    alignas(T) char unused_[sizeof(T)];
  };

  template <typename T>
  struct uninitialized_t<T, typename std::enable_if<not std::is_trivially_destructible<T>::value>::type>
  {
    uninitialized_t() = default;
    ~uninitialized_t() noexcept(std::is_nothrow_destructible<T>::value) {}

    alignas(T) char unused_[sizeof(T)];
  };

  template <typename T>
  struct uninitialized {
    using type = uninitialized_t<T>;

    static_assert(sizeof(type) == sizeof(T), "NoStd internal error: nostd::uninitialized<T>::type has a size different than T");
    static_assert(alignof(type) == alignof(T), "NoStd internal error: nostd::uninitialized<T>::type has an alignment different than T");
  };

}  // namespace nostd

#endif  // NoStd_uninitialized_h
