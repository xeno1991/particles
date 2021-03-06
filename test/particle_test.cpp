#include "particles/particle.hpp"

#include <gtest/gtest.h>

using particles::Particle;

TEST(ParticleTest, constructor) {
  Particle<double, 2> p0;
  EXPECT_DOUBLE_EQ(0, p0.position(0)); 
  EXPECT_DOUBLE_EQ(0, p0.position(1)); 
  EXPECT_DOUBLE_EQ(0, p0.velocity(0)); 
  EXPECT_DOUBLE_EQ(0, p0.velocity(1)); 

  Particle<double, 2> p1({1,2});
  EXPECT_DOUBLE_EQ(1, p1.position(0)); 
  EXPECT_DOUBLE_EQ(2, p1.position(1)); 
  EXPECT_DOUBLE_EQ(0, p1.velocity(0)); 
  EXPECT_DOUBLE_EQ(0, p1.velocity(1)); 

  Particle<double, 2> p2({1,2},{3,4});
  EXPECT_DOUBLE_EQ(1, p2.position(0)); 
  EXPECT_DOUBLE_EQ(2, p2.position(1)); 
  EXPECT_DOUBLE_EQ(3, p2.velocity(0)); 
  EXPECT_DOUBLE_EQ(4, p2.velocity(1)); 
}

TEST(ParticleInfoTest, info) {
  Particle<int, 2, int> p;

  p.info() = 2;
  EXPECT_EQ(2, p.info());
}
