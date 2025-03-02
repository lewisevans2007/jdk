/*
 * Copyright (c) 2000, 2023, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_GC_SERIAL_TENUREDGENERATION_INLINE_HPP
#define SHARE_GC_SERIAL_TENUREDGENERATION_INLINE_HPP

#include "gc/serial/tenuredGeneration.hpp"

#include "gc/shared/space.inline.hpp"

inline size_t TenuredGeneration::capacity() const {
  return space()->capacity();
}

inline size_t TenuredGeneration::used() const {
  return space()->used();
}

inline size_t TenuredGeneration::free() const {
  return space()->free();
}

inline MemRegion TenuredGeneration::used_region() const {
  return space()->used_region();
}

inline bool TenuredGeneration::is_in(const void* p) const {
  return space()->is_in(p);
}

HeapWord* TenuredGeneration::allocate(size_t word_size,
                                                 bool is_tlab) {
  assert(!is_tlab, "TenuredGeneration does not support TLAB allocation");
  return _the_space->allocate(word_size);
}

HeapWord* TenuredGeneration::par_allocate(size_t word_size,
                                                     bool is_tlab) {
  assert(!is_tlab, "TenuredGeneration does not support TLAB allocation");
  return _the_space->par_allocate(word_size);
}

bool TenuredGeneration::block_is_obj(const HeapWord* addr) const {
  return addr < _the_space  ->top();
}

template <typename OopClosureType>
void TenuredGeneration::oop_since_save_marks_iterate(OopClosureType* blk) {
  _the_space->oop_since_save_marks_iterate(blk);
}

#endif // SHARE_GC_SERIAL_TENUREDGENERATION_INLINE_HPP
