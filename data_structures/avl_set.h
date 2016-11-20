#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "data_structures/avl_tree.h"

namespace lib {

template <typename ValueType, typename Comparator = std::less<ValueType>>
class AvlSet {
public:
  using value_type = ValueType;
  using size_type = uint32;

private:

};

} // namespace lib
