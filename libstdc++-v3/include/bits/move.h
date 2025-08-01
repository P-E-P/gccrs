// Move, forward and identity for C++11 + swap -*- C++ -*-

// Copyright (C) 2007-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/move.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 */

#ifndef _MOVE_H
#define _MOVE_H 1

#include <bits/c++config.h>
#if __cplusplus < 201103L
# include <bits/concept_check.h>
#else
# include <type_traits> // Brings in std::declval too.
#endif

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Used, in C++03 mode too, by allocators, etc.
  /**
   *  @brief Same as C++11 std::addressof
   *  @ingroup utilities
   */
  template<typename _Tp>
    __attribute__((__always_inline__))
    inline _GLIBCXX_CONSTEXPR _Tp*
    __addressof(_Tp& __r) _GLIBCXX_NOEXCEPT
    { return __builtin_addressof(__r); }

#if __cplusplus >= 201103L

  /**
   *  @addtogroup utilities
   *  @{
   */

  /**
   *  @brief  Forward an lvalue.
   *  @return The parameter cast to the specified type.
   *
   *  This function is used to implement "perfect forwarding".
   *  @since C++11
   */
  template<typename _Tp>
    [[__nodiscard__,__gnu__::__always_inline__]]
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    { return static_cast<_Tp&&>(__t); }

  /**
   *  @brief  Forward an rvalue.
   *  @return The parameter cast to the specified type.
   *
   *  This function is used to implement "perfect forwarding".
   *  @since C++11
   */
  template<typename _Tp>
    [[__nodiscard__,__gnu__::__always_inline__]]
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
    {
      static_assert(!std::is_lvalue_reference<_Tp>::value,
	  "std::forward must not be used to convert an rvalue to an lvalue");
      return static_cast<_Tp&&>(__t);
    }

#if __glibcxx_forward_like // C++ >= 23
  template<typename _Tp, typename _Up>
  struct __like_impl; // _Tp must be a reference and _Up an lvalue reference

  template<typename _Tp, typename _Up>
  struct __like_impl<_Tp&, _Up&>
  { using type = _Up&; };

  template<typename _Tp, typename _Up>
  struct __like_impl<const _Tp&, _Up&>
  { using type = const _Up&; };

  template<typename _Tp, typename _Up>
  struct __like_impl<_Tp&&, _Up&>
  { using type = _Up&&; };

  template<typename _Tp, typename _Up>
  struct __like_impl<const _Tp&&, _Up&>
  { using type = const _Up&&; };

  template<typename _Tp, typename _Up>
    using __like_t = typename __like_impl<_Tp&&, _Up&>::type;

  /** @brief Forward with the cv-qualifiers and value category of another type.
   *  @tparam _Tp An lvalue reference or rvalue reference.
   *  @tparam _Up An lvalue reference type deduced from the function argument.
   *  @param __x An lvalue.
   *  @return `__x` converted to match the qualifiers of `_Tp`.
   *  @since C++23
   */
  template<typename _Tp, typename _Up>
  [[nodiscard,__gnu__::__always_inline__]]
  constexpr __like_t<_Tp, _Up>
  forward_like(_Up&& __x) noexcept
  { return static_cast<__like_t<_Tp, _Up>>(__x); }
#endif

  /**
   *  @brief  Convert a value to an rvalue.
   *  @param  __t  A thing of arbitrary type.
   *  @return The parameter cast to an rvalue-reference to allow moving it.
   *  @since C++11
  */
  template<typename _Tp>
    [[__nodiscard__,__gnu__::__always_inline__]]
    constexpr typename std::remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept
    { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }


  template<typename _Tp>
    struct __move_if_noexcept_cond
    : public __and_<__not_<is_nothrow_move_constructible<_Tp>>,
                    is_copy_constructible<_Tp>>::type { };

  /**
   *  @brief  Conditionally convert a value to an rvalue.
   *  @param  __x  A thing of arbitrary type.
   *  @return The parameter, possibly cast to an rvalue-reference.
   *
   *  Same as std::move unless the type's move constructor could throw and the
   *  type is copyable, in which case an lvalue-reference is returned instead.
   *  @since C++11
   */
  template<typename _Tp>
    [[__nodiscard__,__gnu__::__always_inline__]]
    constexpr
    __conditional_t<__move_if_noexcept_cond<_Tp>::value, const _Tp&, _Tp&&>
    move_if_noexcept(_Tp& __x) noexcept
    { return std::move(__x); }

  // declval, from type_traits.

  /**
   *  @brief Returns the actual address of the object or function
   *         referenced by r, even in the presence of an overloaded
   *         operator&.
   *  @param  __r  Reference to an object or function.
   *  @return   The actual address.
   *  @since C++11
  */
  template<typename _Tp>
    [[__nodiscard__,__gnu__::__always_inline__]]
    inline _GLIBCXX17_CONSTEXPR _Tp*
    addressof(_Tp& __r) noexcept
    { return __builtin_addressof(__r); }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2598. addressof works on temporaries
  template<typename _Tp>
    const _Tp* addressof(const _Tp&&) = delete;

  // C++11 version of std::exchange for internal use.
  template <typename _Tp, typename _Up = _Tp>
    _GLIBCXX20_CONSTEXPR
    inline _Tp
    __exchange(_Tp& __obj, _Up&& __new_val)
    {
      _Tp __old_val = std::move(__obj);
      __obj = std::forward<_Up>(__new_val);
      return __old_val;
    }

  /// @} group utilities

#define _GLIBCXX_FWDREF(_Tp) _Tp&&
#define _GLIBCXX_MOVE(__val) std::move(__val)
#define _GLIBCXX_FORWARD(_Tp, __val) std::forward<_Tp>(__val)
#else
#define _GLIBCXX_FWDREF(_Tp) const _Tp&
#define _GLIBCXX_MOVE(__val) (__val)
#define _GLIBCXX_FORWARD(_Tp, __val) (__val)
#endif

  /**
   *  @addtogroup utilities
   *  @{
   */

  /**
   *  @brief Swaps two values.
   *  @param  __a  A thing of arbitrary type.
   *  @param  __b  Another thing of arbitrary type.
   *  @return   Nothing.
  */
  template<typename _Tp>
#if __glibcxx_concepts // >= C++20
    requires (! __is_tuple_like<_Tp>::value)
      && is_move_constructible_v<_Tp>
      && is_move_assignable_v<_Tp>
    constexpr void
#elif __cplusplus >= 201103L
    _GLIBCXX20_CONSTEXPR inline
    __enable_if_t<__and_<__not_<__is_tuple_like<_Tp>>,
			 is_move_constructible<_Tp>,
			 is_move_assignable<_Tp>>::value>
#else
    inline void
#endif
    swap(_Tp& __a, _Tp& __b)
    _GLIBCXX_NOEXCEPT_IF(__and_<is_nothrow_move_constructible<_Tp>,
				is_nothrow_move_assignable<_Tp>>::value)
    {
#if __cplusplus < 201103L
      // concept requirements
      __glibcxx_function_requires(_SGIAssignableConcept<_Tp>)
#endif
      _Tp __tmp = _GLIBCXX_MOVE(__a);
      __a = _GLIBCXX_MOVE(__b);
      __b = _GLIBCXX_MOVE(__tmp);
    }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 809. std::swap should be overloaded for array types.
  /// Swap the contents of two arrays.
  template<typename _Tp, size_t _Nm>
#if __glibcxx_concepts // >= C++20
    requires is_swappable_v<_Tp>
    constexpr void
#elif __cplusplus >= 201103L
    _GLIBCXX20_CONSTEXPR inline
    __enable_if_t<__is_swappable<_Tp>::value>
#else
    inline void
#endif
    swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm])
    _GLIBCXX_NOEXCEPT_IF(__is_nothrow_swappable<_Tp>::value)
    {
      for (size_t __n = 0; __n < _Nm; ++__n)
	swap(__a[__n], __b[__n]);
    }

  /// @} group utilities
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif /* _MOVE_H */
