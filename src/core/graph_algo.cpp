#include"../../include/graph_db/graph_algo.h"
#include"../../include/graph_db/graph.h"

#include <limits>
#include <queue>
#include <unordered_map>
namespace graph_db{
    std::vector<NodeID> bfs(Graph& g, NodeID start) {
        std::vector<NodeID> visited;
        std::queue<NodeID> q;
        std::unordered_map<NodeID, bool> in_queue;

        q.push(start);
        in_queue[start] = true;

        while (!q.empty()) {
            NodeID current = q.front();
            q.pop();
            visited.push_back(current);

            for (const auto& neighbor : g.get_neighbors(current)) {
                if (in_queue.find(neighbor) == in_queue.end()) {
                    q.push(neighbor);
                    in_queue[neighbor] = true;
                }
            }
        }

        return visited;
    }

    std::vector<NodeID> bfs_level(Graph& g,NodeID start , int level){
        std::vector<NodeID> visited;
        std::queue<std::pair<NodeID,int>> q; // Pair of NodeID and its level
        std::unordered_map<NodeID, bool> in_queue;

        q.push({start, 0});
        in_queue[start] = true;

        while (!q.empty()) {
            auto [current, curr_level] = q.front();
            q.pop();

            if(curr_level > level) break; // Stop if we exceed the desired level

            visited.push_back(current);

            for (const auto& neighbor : g.get_neighbors(current)) {
                if (in_queue.find(neighbor) == in_queue.end()) {
                    q.push({neighbor, curr_level + 1});
                    in_queue[neighbor] = true;
                }
            }
        }

        return visited;
    }

    std::vector<NodeID> dfs(Graph& g, NodeID start) {
        std::vector<NodeID> visited;
        std::stack<NodeID> s;
        std::unordered_map<NodeID, bool> in_stack;

        s.push(start);
        in_stack[start] = true;

        while (!s.empty()) {
            NodeID current = s.top();
            s.pop();
            visited.push_back(current);

            for (const auto& neighbor : g.get_neighbors(current)) {
                if (in_stack.find(neighbor) == in_stack.end()) {
                    s.push(neighbor);
                    in_stack[neighbor] = true;
                }
            }
        }

        return visited;
    }
    
    std::unordered_map<NodeID, int64_t> dijkstra(Graph& g, NodeID start) {
        std::unordered_map<NodeID, int64_t> distances;
        auto cmp = [&distances](NodeID left, NodeID right) { return distances[left] > distances[right]; };
        std::priority_queue<NodeID, std::vector<NodeID>, decltype(cmp)> pq(cmp);

        for (const auto& [node_id, _] : g.get_all_nodes()) {
            distances[node_id] = std::numeric_limits<int64_t>::max();
        }
        distances[start] = 0;
        pq.push(start);

        while (!pq.empty()) {
            NodeID current = pq.top();
            pq.pop();

            for (const auto& neighbor : g.get_neighbors(current)) {
                Edge* edge = nullptr;
                // Find the edge between current and neighbor
                for (const auto& edge_id : g.get_node_unlocked(current)->get_out_edges()) {
                    Edge* e = g.get_edge(edge_id);
                    if (e && e->to_node() == neighbor) {
                        edge = e;
                        break;
                    }
                }
                if (!edge) continue; // No edge found

                int64_t new_dist = distances[current] + edge->get_weight();
                if (new_dist < distances[neighbor]) {
                    distances[neighbor] = new_dist;
                    pq.push(neighbor);
                }
            }
        }

        return distances;
    }
}