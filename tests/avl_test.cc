// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "iterators.h"
#include "data_structures/avl_tree.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(avl_suite)


BOOST_AUTO_TEST_CASE(insert_lower_bound_test) {
  std::set<int64> set;
  avl::ValueNode::node_pointer tree = nullptr;

  for (auto i: range(0, 100000)) {
    auto value1 = Random32() + 1;
    auto value2 = Random32() + 1;
    set.insert(value1);
    set.insert(value2);
    tree = avl::insert(tree, value1);
    tree = avl::insert(tree, value2);

    auto value3 = Random32() % std::max(value1, value2);

    BOOST_CHECK_EQUAL(*set.begin(), avl::first(tree)->value);
    BOOST_CHECK_EQUAL(*set.lower_bound(value3), avl::lower_bound(tree, value3)->value);
    BOOST_CHECK_EQUAL(set.size(), avl::size(tree));

    set.erase(set.begin());
    tree = avl::erase(avl::first(tree));
  }

  destroy_tree(tree);
}

BOOST_AUTO_TEST_CASE(insert_erase_test) {
  std::set<int64> set;
  avl::ValueNode::node_pointer tree = nullptr;

  for (auto i: range(0, 100000)) {
    auto value1 = Random32() + 1;
    auto value2 = Random32() + 1;
    set.insert(value1);
    set.insert(value2);
    tree = avl::insert(tree, value1);
    tree = avl::insert(tree, value2);

    auto value3 = Random32() % std::max(value1, value2);

    auto l1 = set.lower_bound(value3);
    auto l2 = avl::lower_bound(tree, value3);

    BOOST_CHECK_EQUAL(set.size(), avl::size(tree));
    BOOST_CHECK_EQUAL(*l1, l2->value);

    set.erase(l1);
    tree = avl::erase(l2);
  }

  destroy_tree(tree);
}

BOOST_AUTO_TEST_CASE(destroy_test) {
  {
    avl::ValueNode::node_pointer tree = nullptr;
    tree = avl::insert(tree, 1);
    destroy_tree(tree);
  }

  {
    avl::ValueNode::node_pointer tree = nullptr;
    tree = avl::insert(tree, 1);
    tree = avl::insert(tree, 2);
    destroy_tree(tree);
  }

  {
    avl::ValueNode::node_pointer tree = nullptr;
    tree = avl::insert(tree, 1);
    tree = avl::insert(tree, 2);
    tree = avl::insert(tree, 3);
    destroy_tree(tree);
  }

  {
    avl::ValueNode::node_pointer tree = nullptr;
    tree = avl::insert(tree, 1);
    tree = avl::insert(tree, 2);
    tree = avl::insert(tree, 3);
    tree = avl::insert(tree, 4);
    destroy_tree(tree);
  }
}

BOOST_AUTO_TEST_CASE(next_inorder_test) {
  avl::ValueNode::node_pointer tree = nullptr;
  tree = avl::insert(tree, 1);
  tree = avl::insert(tree, 2);
  tree = avl::insert(tree, 3);
  tree = avl::insert(tree, 4);
  tree = avl::insert(tree, 5);
  tree = avl::insert(tree, 6);

  auto node = first(tree);
  BOOST_CHECK_EQUAL(node->value, 1);
  node = next_inorder(node);
  BOOST_CHECK_EQUAL(node->value, 2);
  node = next_inorder(node);
  BOOST_CHECK_EQUAL(node->value, 3);
  node = next_inorder(node);
  BOOST_CHECK_EQUAL(node->value, 4);
  node = next_inorder(node);
  BOOST_CHECK_EQUAL(node->value, 5);
  node = next_inorder(node);
  BOOST_CHECK_EQUAL(node->value, 6);
  node = next_inorder(node);
  BOOST_CHECK(node == nullptr);

  destroy_tree(tree);
}

BOOST_AUTO_TEST_CASE(prev_inorder_test) {
  avl::ValueNode::node_pointer tree = nullptr;
  tree = avl::insert(tree, 1);
  tree = avl::insert(tree, 2);
  tree = avl::insert(tree, 3);
  tree = avl::insert(tree, 4);
  tree = avl::insert(tree, 5);
  tree = avl::insert(tree, 6);

  auto node = last(tree);
  BOOST_REQUIRE(node != nullptr);
  BOOST_CHECK_EQUAL(node->value, 6);
  node = prev_inorder(node);
  BOOST_REQUIRE(node != nullptr);
  BOOST_CHECK_EQUAL(node->value, 5);
  node = prev_inorder(node);
  BOOST_REQUIRE(node != nullptr);
  BOOST_CHECK_EQUAL(node->value, 4);
  node = prev_inorder(node);
  BOOST_REQUIRE(node != nullptr);
  BOOST_CHECK_EQUAL(node->value, 3);
  node = prev_inorder(node);
  BOOST_REQUIRE(node != nullptr);
  BOOST_CHECK_EQUAL(node->value, 2);
  node = prev_inorder(node);
  BOOST_REQUIRE(node != nullptr);
  BOOST_CHECK_EQUAL(node->value, 1);
  node = prev_inorder(node);
  BOOST_REQUIRE(node == nullptr);

  destroy_tree(tree);

  std::cerr << "Size = " << sizeof(avl::ValueNode) << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
