#include <gtest/gtest.h>
#include "../node.h"

TEST(NodeTest, DefaultConstructor) {
    Node node;
    EXPECT_EQ(node.getId(), 0);
    EXPECT_TRUE(node.getData().empty());
}

TEST(NodeTest, ParameterizedConstructor) {
    Node node(1, "Test Data");
    EXPECT_EQ(node.getId(), 1);
    EXPECT_EQ(node.getData(), "Test Data");
}

TEST(NodeTest, SetData) {
    Node node;
    node.setData("New Data");
    EXPECT_EQ(node.getData(), "New Data");
}