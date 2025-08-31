#include <gtest/gtest.h>
#include "graph_db/graph.h"
#include "graph_db/node.h"
#include "graph_db/edge.h"

#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <algorithm>
#include <atomic>
#include <chrono>
using namespace graph_db;

class GraphAdditionalTests : public ::testing::Test {
protected:
    Graph g;

    void SetUp() override {
        // Initialize graph for each test
    }
};

// Test node creation and removal
TEST_F(GraphAdditionalTests, CreateAndRemoveNode) {
    NodeID node1 = g.create_node();
    NodeID node2 = g.create_node();

    EXPECT_TRUE(g.has_node(node1));
    EXPECT_TRUE(g.has_node(node2));
    EXPECT_EQ(g.node_count(), 2);

    g.remove_node(node1);
    EXPECT_FALSE(g.has_node(node1));
    EXPECT_EQ(g.node_count(), 1);
}

// Test edge creation and removal
TEST_F(GraphAdditionalTests, CreateAndRemoveEdge) {
    NodeID node1 = g.create_node();
    NodeID node2 = g.create_node();

    EdgeID edge = g.create_edge(node1, node2, "edge_label");
    EXPECT_TRUE(g.has_edge(edge));

    g.remove_edge(edge);
    EXPECT_FALSE(g.has_edge(edge));
}

// Test neighbor retrieval
TEST_F(GraphAdditionalTests, GetNeighbors) {
    NodeID node1 = g.create_node();
    NodeID node2 = g.create_node();
    NodeID node3 = g.create_node();

    g.create_edge(node1, node2);
    g.create_edge(node1, node3);

    auto neighbors = g.get_neighbors(node1);
    EXPECT_EQ(neighbors.size(), 2);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), node2), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), node3), neighbors.end());
}

// Test property management on edges
TEST_F(GraphAdditionalTests, EdgePropertyManagement) {
    NodeID node1 = g.create_node();
    NodeID node2 = g.create_node();

    EdgeID edge = g.create_edge(node1, node2, "edge_label");
    Edge* edge_ptr = g.get_edge(edge);

    edge_ptr->set_property("weight", 5.5);
    EXPECT_TRUE(edge_ptr->has_property("weight"));
    EXPECT_EQ(std::get<double>(edge_ptr->get_property("weight")), 5.5);

    edge_ptr->remove_property("weight");
    EXPECT_FALSE(edge_ptr->has_property("weight"));
}

TEST(ThreadSafetyTests, ConcurrentEdgeCreateModifyRemove) {
    Graph g;

    // Prepare a stable set of nodes to avoid invalid node accesses.
    constexpr int kNodeCount = 100;
    for (int i = 0; i < kNodeCount; ++i) g.create_node();

    // Shared edge-id container and its mutex
    std::vector<EdgeID> edge_ids;
    std::mutex edge_mutex;

    std::atomic<bool> stop{false};
    const int creators = 4;
    const int modifiers = 4;
    const int removers = 2;
    constexpr int iterations_per_thread = 500; // constexpr → no capture needed

    std::random_device rd;
    std::mt19937_64 rng(rd());

    // Helper to pick random node id in range [1..kNodeCount]
    auto pick_node = [&](std::mt19937_64 &r) -> NodeID {
        std::uniform_int_distribution<int> dist(1, kNodeCount);
        return static_cast<NodeID>(dist(r));
    };

    // Edge creator threads
    std::vector<std::thread> threads;
    for (int t = 0; t < creators; ++t) {
        threads.emplace_back([&g, &edge_ids, &edge_mutex, &stop, &rng, pick_node]() mutable {
            std::mt19937_64 r(rng());
            for (int i = 0; i < iterations_per_thread && !stop.load(); ++i) {
                NodeID a = pick_node(r);
                NodeID b = pick_node(r);
                if (a == b) continue;
                try {
                    EdgeID e = g.create_edge(a, b, "concurrent");
                    std::lock_guard<std::mutex> lk(edge_mutex);
                    edge_ids.push_back(e);
                } catch (...) {
                    // ignore and continue
                }
            }
        });
    }

    // Edge modifier threads: set/get/remove property
    for (int t = 0; t < modifiers; ++t) {
        threads.emplace_back([&g, &edge_ids, &edge_mutex, &stop, &rng]() mutable {
            std::mt19937_64 r(rng());
            for (int i = 0; i < iterations_per_thread && !stop.load(); ++i) {
                EdgeID chosen = 0;
                {
                    std::lock_guard<std::mutex> lk(edge_mutex);
                    if (edge_ids.empty()) continue;
                    size_t idx = std::uniform_int_distribution<size_t>(0, edge_ids.size() - 1)(r);
                    chosen = edge_ids[idx];
                }
                Edge* e = g.get_edge(chosen);
                if (!e) continue;
                try {
                    e->set_property("p", 3.1415);
                    if (e->has_property("p")) {
                        auto v = e->get_property("p");
                        (void)std::get<double>(v);
                    }
                    e->remove_property("p");
                } catch (...) {
                    // ignore transient race-related errors
                }
            }
        });
    }

    // Edge remover threads
    for (int t = 0; t < removers; ++t) {
        threads.emplace_back([&g, &edge_ids, &edge_mutex, &stop, &rng]() mutable {
            std::mt19937_64 r(rng());
            for (int i = 0; i < iterations_per_thread && !stop.load(); ++i) {
                EdgeID chosen = 0;
                {
                    std::lock_guard<std::mutex> lk(edge_mutex);
                    if (edge_ids.empty()) continue;
                    size_t idx = std::uniform_int_distribution<size_t>(0, edge_ids.size() - 1)(r);
                    chosen = edge_ids[idx];
                    // optimistically remove from tracking
                    edge_ids.erase(edge_ids.begin() + idx);
                }
                g.remove_edge(chosen);
            }
        });
    }

    // Wait for threads to finish
    for (auto &th : threads) if (th.joinable()) th.join();
    stop.store(true);

    // Basic consistency checks for remaining tracked edges
    {
        std::lock_guard<std::mutex> lk(edge_mutex);
        for (EdgeID eid : edge_ids) {
            if (!g.has_edge(eid)) continue;
            Edge* e = g.get_edge(eid);
            ASSERT_NE(e, nullptr);
            Node* from = g.get_node(e->from_node());
            Node* to   = g.get_node(e->to_node());
            ASSERT_NE(from, nullptr);
            ASSERT_NE(to, nullptr);
            auto outs = from->get_out_edges();
            auto ins  = to->get_in_edges();
            ASSERT_NE(outs.find(eid), outs.end());
            ASSERT_NE(ins.find(eid), ins.end());
        }
    }

    // Final simple sanity: no crash and graph reports reasonable counts
    ASSERT_GE(g.node_count(), static_cast<size_t>(kNodeCount));
    (void)g.edge_count();
}
