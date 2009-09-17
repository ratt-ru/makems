//#  lofar_algorithm.h:
//#
//#  Copyright (C) 2002
//#  ASTRON (Netherlands Foundation for Research in Astronomy)
//#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//#  This program is free software; you can redistribute it and/or modify
//#  it under the terms of the GNU General Public License as published by
//#  the Free Software Foundation; either version 2 of the License, or
//#  (at your option) any later version.
//#
//#  This program is distributed in the hope that it will be useful,
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#  GNU General Public License for more details.
//#
//#  You should have received a copy of the GNU General Public License
//#  along with this program; if not, write to the Free Software
//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//#  $Id: lofar_algorithm.h 9745 2006-12-15 10:08:46Z loose $

#ifndef LOFAR_COMMON_ALGORITHM_H
#define LOFAR_COMMON_ALGORITHM_H

// \file

#include <algorithm>

namespace LOFAR
{
  using std::adjacent_find;
  using std::binary_search;
  using std::copy;
  using std::copy_backward;
  using std::count;
  using std::count_if;
  using std::equal;
  using std::equal_range;
  using std::fill;
  using std::fill_n;
  using std::find;
  using std::find_end;
  using std::find_first_of;
  using std::find_if;
  using std::for_each;
  using std::generate;
  using std::generate_n;
  using std::includes;
  using std::inplace_merge;
  using std::iter_swap;
  using std::lexicographical_compare;
  using std::lower_bound;
  using std::make_heap;
  using std::max;
  using std::max_element;
  using std::merge;
  using std::min;
  using std::min_element;
  using std::mismatch;
  using std::next_permutation;
  using std::nth_element;
  using std::partial_sort;
  using std::partial_sort_copy;
  using std::partition;
  using std::pop_heap;
  using std::prev_permutation;
  using std::push_heap;
  using std::random_shuffle;
  using std::remove;
  using std::remove_copy;
  using std::remove_copy_if;
  using std::remove_if;
  using std::replace;
  using std::replace_copy;
  using std::replace_copy_if;
  using std::replace_if;
  using std::reverse;
  using std::reverse_copy;
  using std::rotate;
  using std::rotate_copy;
  using std::search;
  using std::search_n;
  using std::set_difference;
  using std::set_intersection;
  using std::set_symmetric_difference;
  using std::set_union;
  using std::sort;
  using std::sort_heap;
  using std::stable_partition;
  using std::stable_sort;
  using std::swap;
  using std::swap_ranges;
  using std::transform;
  using std::unique;
  using std::unique_copy;
  using std::upper_bound;
}

#endif

