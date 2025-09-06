#ifndef BPLUSTREE_HPP
#define BPLUSTREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <variant>

namespace bplustree {

template<typename Key, typename Value>
class BPlusTree {
public:
    // --- CHANGE HERE: Changed 'int' to 'size_t' ---
    BPlusTree(size_t degree = 3) : degree_(degree), root_(new Node(true)) {}

    void insert(const Key& key, const Value& value) {
        // This comparison is now safe
        if (root_->keys.size() == (2 * degree_ - 1)) {
            Node* new_root = new Node(false);
            new_root->children.push_back(root_);
            split_child(new_root, 0);
            root_ = new_root;
        }
        insert_non_full(root_, key, value);
    }

    std::vector<Value> find(const Key& key) const {
        Node* leaf = find_leaf(root_, key);
        for (size_t i = 0; i < leaf->keys.size(); ++i) {
            if (leaf->keys[i] == key) {
                return leaf->values[i];
            }
        }
        return {};
    }

    void remove(const Key& key, const Value& value) {
         Node* leaf = find_leaf(root_, key);
        for (size_t i = 0; i < leaf->keys.size(); ++i) {
            if (leaf->keys[i] == key) {
                auto& values = leaf->values[i];
                values.erase(std::remove(values.begin(), values.end(), value), values.end());
                if (values.empty()) {
                    leaf->keys.erase(leaf->keys.begin() + i);
                    leaf->values.erase(leaf->values.begin() + i);
                }
                return;
            }
        }
    }

private:
    struct Node {
        bool is_leaf;
        std::vector<Key> keys;
        std::vector<std::vector<Value>> values; // For leaf nodes
        std::vector<Node*> children;

        Node(bool leaf) : is_leaf(leaf) {}
    };

    // --- CHANGE HERE: Changed 'int' to 'size_t' ---
    size_t degree_; 
    Node* root_;

    Node* find_leaf(Node* node, const Key& key) const {
        if (node->is_leaf) {
            return node;
        }
        size_t i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }
        return find_leaf(node->children[i], key);
    }

    void insert_non_full(Node* node, const Key& key, const Value& value) {
        if (node->is_leaf) {
            size_t i = 0;
            while (i < node->keys.size() && key > node->keys[i]) {
                i++;
            }
            if (i < node->keys.size() && node->keys[i] == key) {
                node->values[i].push_back(value);
            } else {
                node->keys.insert(node->keys.begin() + i, key);
                node->values.insert(node->values.begin() + i, {value});
            }
        } else {
            size_t i = 0;
            while (i < node->keys.size() && key > node->keys[i]) {
                i++;
            }
            // This comparison is now safe
            if (node->children[i]->keys.size() == (2 * degree_ - 1)) {
                split_child(node, i);
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insert_non_full(node->children[i], key, value);
        }
    }

    void split_child(Node* parent, int index) {
        Node* child = parent->children[index];
        Node* new_child = new Node(child->is_leaf);
        parent->keys.insert(parent->keys.begin() + index, child->keys[degree_ - 1]);
        parent->children.insert(parent->children.begin() + index + 1, new_child);

        new_child->keys.assign(child->keys.begin() + degree_, child->keys.end());
        child->keys.resize(degree_ - 1);

        if (child->is_leaf) {
            new_child->values.assign(child->values.begin() + degree_, child->values.end());
            child->values.resize(degree_ - 1);
        } else {
            new_child->children.assign(child->children.begin() + degree_, child->children.end());
            child->children.resize(degree_);
        }
    }
};

} // namespace bplustree
#endif // BPLUSTREE_HPP