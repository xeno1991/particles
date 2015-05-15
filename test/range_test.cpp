#include "particles/range.hpp"
#include "particles/vec.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>

using namespace particles;
using namespace particles::range;


TEST(RangeTest, sum) {
  std::vector<int> u = {1, 2, 3};
  EXPECT_EQ(6, sum(u.begin(), u.end()));

  std::vector<Vec<double, 2>> v = {{1, 2},{3, 4}};
  auto s = sum(v.begin(), v.end());
  EXPECT_DOUBLE_EQ(4, s[0]);
  EXPECT_DOUBLE_EQ(6, s[1]);
}

TEST(RangeTest, average) {
  std::vector<Vec<double, 2>> v = {{1, 2},{3, 4}};
  auto ave = average(v.begin(), v.end());
  EXPECT_DOUBLE_EQ(2, ave[0]);
  EXPECT_DOUBLE_EQ(3, ave[1]);
}

TEST(RangeTest, average2) {
  std::vector<int> v {1, 2};
  // double
  auto ave = average(v.begin(), v.end());
  EXPECT_DOUBLE_EQ(1.5, ave);
}


TEST(RangeTest, suite_last_test) {
  EXPECT_EQ( 3, suite_last(0,  3, 1));
  EXPECT_EQ( 6, suite_last(0,  5, 3));
  EXPECT_EQ( 6, suite_last(0,  6, 3));
  EXPECT_EQ( 9, suite_last(0,  7, 3));
  EXPECT_EQ( 9, suite_last(0,  8, 3));
  EXPECT_EQ( 9, suite_last(0,  9, 3));
  EXPECT_EQ(12, suite_last(0, 10, 3));
}


class ConvertIteratorTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    v = {{1, 2}, {3, 4}, {5, 6}};  
  }
  std::vector<std::pair<int, int>> v;
};

TEST_F(ConvertIteratorTest, increments) {
  auto it = transform_iterator(v.begin(), [](auto& p) { return p.first; });
  EXPECT_EQ(1, *it); it++;
  EXPECT_EQ(3, *it++);
  EXPECT_EQ(5, *it);
}

TEST_F(ConvertIteratorTest, compare) {
  auto it1 = transform_iterator(v.begin(), [](auto& p) { return p.first; });
  auto it2 = transform_iterator(v.begin(), [](auto&) { return 0; });
  auto it3 = transform_iterator(v.end(),   [](auto&) { return 0; });

  EXPECT_TRUE (it1 == it2);
  EXPECT_FALSE(it1 != it2);
  EXPECT_FALSE(it1 == it3);
  EXPECT_TRUE (it1 != it3);
  EXPECT_FALSE(it2 == it3);
  EXPECT_TRUE (it2 != it3);
}

TEST_F(ConvertIteratorTest, reference) {
  auto it1 = transform_iterator(v.begin(),
                              [](auto& p) { return std::ref(p.first); });
  (*it1)--;
  EXPECT_EQ(0, v.begin()->first);
  EXPECT_EQ(2, v.begin()->second);
}

TEST_F(ConvertIteratorTest, copy) {
  std::vector<int> result(3);
  auto iter =
      transform_iterator(v.begin(), v.end(), [](auto& p) {return p.first;});
  std::copy(iter.first, iter.second, result.begin());
  EXPECT_EQ(1, result[0]);
  EXPECT_EQ(3, result[1]);
  EXPECT_EQ(5, result[2]);
}

class JoinTest : public ::testing::Test {
 protected:
  typedef std::vector<int> vi;
  typedef vi::iterator actual_iterator;
  vi u, v, w;

  auto make_join_iterator(std::size_t pos = 0) {
    JoinedIterator<actual_iterator, 3> it (
      {u.begin(), v.begin(), w.begin()},
      {u.end(), v.end(), w.end()},
      pos);
    return it;
  }

  virtual void SetUp() {
    u = {1, 2, 3};
    v = {4, 5, 6};
    w = {7, 8, 9};
  }
};

TEST_F(JoinTest, iterator_increment) {
  auto it = make_join_iterator();
  EXPECT_EQ(1, *it);
  ++it; ++it; ++it;   // 2 3 4
  EXPECT_EQ(4, *it);
  it++; it++; it++;
  EXPECT_EQ(7, *it);
}

TEST_F(JoinTest, iterator_compare) {
  auto i = make_join_iterator();
  auto j = make_join_iterator();
  EXPECT_TRUE(i == j);
  EXPECT_FALSE(i != i);
  EXPECT_FALSE(++i == j);
  EXPECT_TRUE(i != j);
}

TEST_F(JoinTest, begin_end) {
  auto r = make_joined(u, v);
  auto it = r.begin();
  EXPECT_EQ(1, *it);
  for (int i=0; i<6; i++) ++it;
  EXPECT_EQ(it, r.end());
}

TEST_F(JoinTest, for) {
  std::vector<int> result;
  for (int n : make_joined(u, v, w)) {
    result.push_back(n);
  }
  for (int i=1; i<=9; i++) {
    EXPECT_EQ(i, result[i-1]);
  }
}

TEST_F(JoinTest, with_stl) {
  std::vector<int> result;
  auto joined = make_joined(u, v, w);
  std::copy(std::begin(joined), std::end(joined), std::back_inserter(result));
  for (int i=1; i<=9; i++) {
    EXPECT_EQ(i, result[i-1]);
  }
}
