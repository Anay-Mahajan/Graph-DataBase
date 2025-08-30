#include <gtest/gtest.h>
#include "../graph.cpp"

TEST(GraphTest, AddNode) {
    Graph g;
    g.addNode(1);
    EXPECT_TRUE(g.hasNode(1));
}

TEST(GraphTest, AddEdge) {
    Graph g;
    g.addNode(1);
    g.addNode(2);
    g.addEdge(1, 2);
    EXPECT_TRUE(g.hasEdge(1, 2));
}

TEST(GraphTest, RemoveNode) {
    Graph g;
    g.addNode(1);
    g.removeNode(1);
    EXPECT_FALSE(g.hasNode(1));
}

TEST(GraphTest, RemoveEdge) {
    Graph g;
    g.addNode(1);
    g.addNode(2);
    g.addEdge(1, 2);
    g.removeEdge(1, 2);
    EXPECT_FALSE(g.hasEdge(1, 2));
}