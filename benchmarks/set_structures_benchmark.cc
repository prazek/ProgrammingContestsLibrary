// Jakub Staroń, 2016
#include <celero/Celero.h>

#include "numeric.h"
#include "data_structures/van_emde_boas_set.h"
#include "data_structures/avl_tree.h"

CELERO_MAIN

using namespace lib;

constexpr size_t samples = 10;
constexpr size_t iterations = 1;

class QueriesFixture : public celero::TestFixture
{
public:
  std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override
  {
    return {
        {100 * 1000, 0},
        {1000 * 1000, 0}
    };
  }

  void setUp(int64_t experimentValue) override
  {
    for (auto i: range<int32>(0, experimentValue)) {
      using lib::Random32;

      auto value1 = Random32() % Million;
      auto value2 = Random32() % Million;
      queries.push_back(value1);
      queries.push_back(value2);
    }
  }

  std::vector<int32> queries;
};


BASELINE_F(InsertPopMin, StdSet, QueriesFixture, samples, iterations)
{
  std::set<int64> set;

  for (auto it = queries.begin(); it != queries.end(); ) {
    auto value1 = *it++;
    auto value2 = *it++;
    set.insert(value1);
    set.insert(value2);

    set.erase(set.begin());
  }
}

BENCHMARK_F(InsertPopMin, VanEmdeBoas, QueriesFixture, samples, iterations)
{
  VanEmdeBoasSet<20> set;
  for (auto it = queries.begin(); it != queries.end(); ) {
    auto value1 = *it++;
    auto value2 = *it++;

    set.insert(value1);
    set.insert(value2);

    set.erase(set.first());
  }
}

BENCHMARK_F(InsertPopMin, avl_tree, QueriesFixture, samples, iterations)
{
  avl::ValueNode::node_pointer tree = nullptr;
  for (auto it = queries.begin(); it != queries.end(); ) {
    auto value1 = *it++;
    auto value2 = *it++;

    tree = avl::insert(tree, value1);
    tree = avl::insert(tree, value2);

    tree = avl::erase(avl::first(tree));
  }
}
