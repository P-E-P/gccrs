// Copyright (C) 2025 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#ifndef RUST_DESUGAR_TRY_BLOCK
#define RUST_DESUGAR_TRY_BLOCK

#include "rust-expr.h"

namespace Rust {
namespace AST {

// FIXME: Add documentation
class DesugarTryBlock
{
public:
  static void go (std::unique_ptr<Expr> &ptr);

private:
  DesugarTryBlock ();

  std::unique_ptr<Expr> desugar (TryExpr &);
};

} // namespace AST
} // namespace Rust

#endif // ! RUST_DESUGAR_TRY_BLOCK
