// A few more utilities used in the backports not corresponding to existing headers -*- C++ -*-

// Copyright (C) 2026 Free Software Foundation, Inc.
// Copyright The GNU Toolchain Authors.
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

#ifndef GCC_BACKPORT_BITS_CONDITIONALS_H
#define GCC_BACKPORT_BITS_CONDITIONALS_H 1

#include <type_traits>

#ifndef __has_builtin
# define gcc_backport_has_builtin(x) 0
#else
# define gcc_backport_has_builtin(x) __has_builtin (x)
#endif

// Helper.  Expands to 'constexpr' in C++20 and an empty string otherwise.
#if __cplusplus >= 202002L
# define GCC_BACKPORT20_CONSTEXPR constexpr
#else
# define GCC_BACKPORT20_CONSTEXPR
#endif

namespace gcc
{
  template<typename G_Tp>
    [[nodiscard]]
    constexpr std::add_const_t<G_Tp>&
    as_const(G_Tp& g_t) noexcept
    { return g_t; }

  template<typename G_Tp>
    void as_const(const G_Tp&&) = delete;
} // namespace gcc

#endif // GCC_BACKPORT_BITS_CONDITIONALS_H
