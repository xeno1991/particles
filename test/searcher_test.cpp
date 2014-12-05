#include "searcher.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

typedef particles::Particle<double, 2> P2;
typedef particles::Particle<double, 3> P3;

std::vector<P3> read_particles3(
    const std::string& filename) {
  std::ifstream ifs(filename);
  std::vector<P3> res;
  double x,y,z;

  while(ifs >> x>>y>>z) {
    particles::Vec<double, 3> pos {x,y,z};   
    particles::Vec<double, 3> vel;   
    res.emplace_back(pos,vel);
  }
  return res;
}

TEST(SearchTest, SimpleRangeSearch) {
  particles::search::SimpleRangeSearch<double, 2> searcher(1.001);
  typename decltype(searcher)::adjacency_list_type adjacency_list;

  std::vector<P2> particles;
  particles.push_back(P2{0,0});
  particles.push_back(P2{1,0});
  particles.push_back(P2{0,1});
  particles.push_back(P2{0.1,0});

  searcher.search(adjacency_list, particles);
  EXPECT_EQ(4, adjacency_list[0].size());
  EXPECT_EQ(3, adjacency_list[1].size());
  EXPECT_EQ(2, adjacency_list[2].size());
  EXPECT_EQ(3, adjacency_list[3].size());
}

TEST(SearchTest, delaunay) {
  // Assume that cwd is (project root)/build/test
  auto particles = read_particles3("../../test/data/3d.xyz");
  ASSERT_TRUE(particles.size()>0);
  
  particles::search::DelaunaySearcher<double,3> searcher;
  typename decltype(searcher)::adjacency_list_type adjacency_list;
  searcher.search(adjacency_list, particles);

  EXPECT_EQ(4, adjacency_list[0].size());
  EXPECT_EQ(6, adjacency_list[1].size());
  EXPECT_EQ(5, adjacency_list[2].size());
  EXPECT_EQ(6, adjacency_list[3].size());
  EXPECT_EQ(4, adjacency_list[4].size());
  EXPECT_EQ(4, adjacency_list[5].size());
  EXPECT_EQ(5, adjacency_list[6].size());
}
