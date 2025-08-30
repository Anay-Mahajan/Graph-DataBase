#include <gtest/gtest.h>
#include "../edge.h"

TEST(EdgeTest, Constructor) {
    Edge edge(1, 2);
    EXPECT_EQ(edge.getStart(), 1);
    EXPECT_EQ(edge.getEnd(), 2);
}

TEST(EdgeTest, Weight) {
    Edge edge(1, 2, 5.0);
    EXPECT_EQ(edge.getWeight(), 5.0);
    edge.setWeight(10.0);
    EXPECT_EQ(edge.getWeight(), 10.0);
}

TEST(EdgeTest, Equality) {
    Edge edge1(1, 2);
    Edge edge2(1, 2);
    EXPECT_TRUE(edge1 == edge2);
    Edge edge3(2, 1);
    EXPECT_FALSE(edge1 == edge3);
}