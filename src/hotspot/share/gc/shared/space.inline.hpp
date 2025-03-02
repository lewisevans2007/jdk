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

#ifndef SHARE_GC_SHARED_SPACE_INLINE_HPP
#define SHARE_GC_SHARED_SPACE_INLINE_HPP

#include "gc/shared/space.hpp"

#include "gc/shared/collectedHeap.hpp"
#include "gc/shared/spaceDecorator.hpp"
#include "oops/oop.inline.hpp"
#include "oops/oopsHierarchy.hpp"
#include "runtime/prefetch.inline.hpp"
#include "runtime/safepoint.hpp"

inline HeapWord* Space::block_start(const void* p) {
  return block_start_const(p);
}

#if INCLUDE_SERIALGC
inline HeapWord* TenuredSpace::allocate(size_t size) {
  HeapWord* res = ContiguousSpace::allocate(size);
  if (res != nullptr) {
    _offsets.update_for_block(res, res + size);
  }
  return res;
}

inline HeapWord* TenuredSpace::par_allocate(size_t size) {
  HeapWord* res = ContiguousSpace::par_allocate(size);
  if (res != nullptr) {
    _offsets.update_for_block(res, res + size);
  }
  return res;
}

inline void TenuredSpace::update_for_block(HeapWord* start, HeapWord* end) {
  _offsets.update_for_block(start, end);
}
#endif // INCLUDE_SERIALGC

template <typename OopClosureType>
void ContiguousSpace::oop_since_save_marks_iterate(OopClosureType* blk) {
  HeapWord* t;
  HeapWord* p = saved_mark_word();
  assert(p != nullptr, "expected saved mark");

  const intx interval = PrefetchScanIntervalInBytes;
  do {
    t = top();
    while (p < t) {
      Prefetch::write(p, interval);
      oop m = cast_to_oop(p);
      p += m->oop_iterate_size(blk);
    }
  } while (t < top());

  set_saved_mark();
}

#endif // SHARE_GC_SHARED_SPACE_INLINE_HPP
