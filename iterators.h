#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "operators.h"

namespace lib {

template<typename Iterator>
struct is_iterator {
private:
  template<typename I> static constexpr auto test(int)
  -> decltype(
      *std::declval<const I>(),
      std::declval<const I>() == std::declval<const I>(),
      std::declval<const I>() != std::declval<const I>(),
      ++ std::declval<I&>(),
      std::declval<I&>() ++,
      std::iterator_traits<I>(),
      bool()) { return
        std::is_destructible<I>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
};

template<typename T>
struct is_iterable {
  template <typename C>
  using begin_t = decltype(std::begin(std::declval<C&>()));

  template <typename C>
  using end_t = decltype(std::end(std::declval<C&>()));

  template<typename I> static constexpr auto test(int)
  -> decltype(
      std::declval<begin_t<I>>(),
      std::declval<end_t<I>>(),
      bool()) { return
        std::is_same<begin_t<I>, end_t<I>>::value &&
        is_iterator<begin_t<I>>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<T>(0);
};

template <typename Integral>
class counting_iterator {
public:
  static_assert(std::is_integral<Integral>::value, "counting_iterator's template argument should be integral");
  using self_type = counting_iterator;
  using value_type = Integral;
  using reference = const value_type&;
  using pointer = const value_type*;
  using difference_type = typename std::make_signed<Integral>::type;
  using iterator_category = std::random_access_iterator_tag;

  counting_iterator(): value_() { }

  explicit counting_iterator(Integral n): value_(n) { }

  reference operator*() const {
    return value_;
  }

  self_type& operator++() {
    ++value_;
    return *this;
  }

  self_type& operator--() {
    --value_;
    return *this;
  }

  self_type& operator+=(difference_type n) {
    value_ += n;
    return *this;
  }

  friend self_type operator+(const self_type& obj, difference_type n) {
    return self_type(obj.value_ + n);
  }

  friend difference_type operator-(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ - rhs.value_;
  }

  value_type operator[](difference_type n) {
    return value_type(value_ + n);
  }

  friend bool operator<(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ < rhs.value_;
  }

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ == rhs.value_;
  }

private:
  Integral value_;
};

template <typename Integral>
auto make_counting_iterator(Integral n) -> counting_iterator<Integral> {
  return counting_iterator<Integral>(n);
}

template <typename Integral>
class reverse_counting_iterator {
public:
  static_assert(std::is_integral<Integral>::value, "reverse_counting_iterator's template argument should be integral");
  using self_type = reverse_counting_iterator;
  using value_type = Integral;
  using reference = const value_type&;
  using pointer = const value_type*;
  using difference_type = typename std::make_signed<Integral>::type;
  using iterator_category = std::random_access_iterator_tag;

  reverse_counting_iterator(): value_() { }

  explicit reverse_counting_iterator(Integral n): value_(n) { }

  reference operator*() const {
    return value_;
  }

  self_type& operator++() {
    --value_;
    return *this;
  }

  self_type& operator--() {
    ++value_;
    return *this;
  }

  self_type& operator+=(difference_type n) {
    value_ -= n;
    return *this;
  }

  friend self_type operator+(const self_type& obj, difference_type n) {
    return self_type(obj.value_ - n);
  }

  friend difference_type operator-(const self_type& lhs, const self_type& rhs) {
    return rhs.value_ - lhs.value_;
  }

  value_type operator[](difference_type n) {
    return value_type(value_ - n);
  }

  friend bool operator<(const self_type& lhs, const self_type& rhs) {
    return rhs.value_ < lhs.value_;
  }

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.value_ == rhs.value_;
  }

private:
  Integral value_;
};

template <typename Integral>
auto make_reverse_counting_iterator(Integral n) -> reverse_counting_iterator<Integral> {
  return reverse_counting_iterator<Integral>(n);
}

template <typename Iterator>
class iterator_range {
public:
  using iterator_type = Iterator;
  using reference = const Iterator&;

  static_assert(is_iterator<Iterator>::value, "iterator_range's template argument should be iterator!");

  explicit iterator_range(Iterator begin, Iterator end):
      begin_(std::move(begin)), end_(std::move(end)) { }

  reference begin() const {
    return begin_;
  }

  reference end() const {
    return end_;
  }

private:
  iterator_type begin_;
  iterator_type end_;
};

template <typename Iterator>
auto make_range(Iterator begin, Iterator end) -> iterator_range<Iterator> {
  return iterator_range<Iterator>(std::move(begin), std::move(end));
}

template <typename Integral>
auto range(Integral begin, Integral end) ->
typename std::enable_if<std::is_integral<Integral>::value, iterator_range<counting_iterator<Integral>>>::type {
  return make_range(make_counting_iterator(begin), make_counting_iterator(end));
}

template <typename Integral>
auto rrange(Integral begin, Integral end) ->
typename std::enable_if<std::is_integral<Integral>::value, iterator_range<reverse_counting_iterator<Integral>>>::type {
  return make_range(make_reverse_counting_iterator(end - 1), make_reverse_counting_iterator(begin - 1));
}

template <typename Iterator, typename Mapper>
class mapping_iterator {
public:
  using self_type = mapping_iterator;
  using underlying_value_type = decltype(*std::declval<Iterator>());
  using value_type = decltype(std::declval<Mapper>()(std::declval<underlying_value_type>()));
  using reference = value_type;
  using pointer = typename std::remove_reference<value_type>::type*;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

  mapping_iterator(const Mapper& mapper = Mapper()):
      iterator_(), mapper_(mapper) { }

  explicit mapping_iterator(Iterator iterator, const Mapper& mapper = Mapper()):
      iterator_(iterator), mapper_(mapper) { }

  reference operator*() const {
    return mapper_(*iterator_);
  }

  self_type& operator++() {
    ++iterator_;
    return *this;
  }

  self_type& operator--() {
    --iterator_;
    return *this;
  }

  self_type& operator+=(difference_type n) {
    iterator_ += n;
    return *this;
  }

  friend self_type operator+(const self_type& obj, difference_type n) {
    return self_type(obj.iterator_ + n, obj.mapper_);
  }

  friend difference_type operator-(const self_type& lhs, const self_type& rhs) {
    return lhs.iterator_ - rhs.iterator_;
  }

  value_type operator[](difference_type n) {
    return *(*this + n);
  }

  friend bool operator<(const self_type& lhs, const self_type& rhs) {
    return lhs.iterator_ < rhs.iterator_;
  }

  friend bool operator==(const self_type& lhs, const self_type& rhs) {
    return lhs.iterator_ == rhs.iterator_;
  }

private:
  Iterator iterator_;
  Mapper mapper_;
};

namespace detail {

template <typename ValuesIterator>
struct IndirectMapper {
  static_assert(is_iterator<ValuesIterator>::value, "first indirect_iterator's template argument must be iterator.");
  static_assert(
      std::is_same<typename std::iterator_traits<ValuesIterator>::iterator_category, std::random_access_iterator_tag>::value,
      "ValuesIterator must be random assess iterator."
  );

  using value_type = typename std::iterator_traits<ValuesIterator>::value_type;
  using reference = const value_type&;
  using index_type = typename std::iterator_traits<ValuesIterator>::difference_type;

  IndirectMapper() = default;

  IndirectMapper(ValuesIterator values):
      values_(values) { }

  reference operator()(index_type n) const {
    return *(values_ + n);
  }

private:
  ValuesIterator values_;
};

} // namespace detail

template <typename ValuesIterator, typename IndexesIterator>
using indirect_iterator = mapping_iterator<IndexesIterator, detail::IndirectMapper<ValuesIterator>>;

template <typename ValuesIterator, typename IndexesIterator>
auto make_indirect_iterator(ValuesIterator values, IndexesIterator indexes) ->
indirect_iterator<ValuesIterator, IndexesIterator> {
  detail::IndirectMapper<ValuesIterator> mapper(values);
  return indirect_iterator<ValuesIterator, IndexesIterator>(std::move(indexes), std::move(mapper));
};

} // namespace lib
