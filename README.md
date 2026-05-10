# Graph Database Engine

A high-performance, concurrent Graph Database Engine built from scratch in modern C++17. This project provides a robust foundation for storing, querying, and analyzing graph-structured data with support for node/edge properties, disk serialization, and thread-safe operations.

## ✨ Features

- **Core Graph Data Structures**: Full support for Nodes and directed Edges with integer weights and string labels.
- **Dynamic Properties**: Attach arbitrary key-value properties to both nodes and edges. Supported types include `bool`, `int64_t`, `double`, and `std::string`.
- **High Concurrency**: Built with `std::shared_mutex` to allow concurrent multi-threaded reads while safely managing exclusive locks for writes/modifications.
- **Indexing Engine**: B+ Tree-based index manager allows `CREATE INDEX` on specific node properties for `O(log N)` rapid querying without scanning the entire graph.
- **Storage & Buffer Pool Management**: Includes a custom serialization format to save and load graphs to disk, backed by a Buffer Pool Manager utilizing an LRU (Least Recently Used) caching policy for out-of-core graph processing.
- **Graph Algorithms**: Built-in implementations of core graph traversals and pathfinding:
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - Dijkstra's Algorithm (Shortest Path)
- **Interactive CLI**: A fully featured REPL (Read-Eval-Print Loop) command-line interface for interacting with the database.

## 🛠️ Tech Stack

- **Language**: C++17
- **Build System**: CMake (Minimum 3.16)
- **Testing**: GoogleTest (CTest)

## 🚀 Getting Started

### Prerequisites
- A C++17 compatible compiler (Clang, GCC, or MSVC)
- CMake 3.16 or higher

### Building the Project

Clone the repository and build using CMake:

```bash
mkdir -p build
cd build
cmake ..
make -j4
```

### Running the CLI

Start the interactive command-line interface:

```bash
./src/graph_cli
```

### Running Tests

The project includes a comprehensive test suite to verify core logic, indexing, serialization, and thread safety. To run the tests:

```bash
cd build
ctest --output-on-failure
# Or run the executable directly for detailed output:
./tests/runTests
```

## 💻 CLI Commands Usage

Once inside the `graph_cli`, you can use the following commands to interact with your graph:

**Entity Creation & Deletion**
- `CREATE NODE`
- `CREATE EDGE FROM <from_id> TO <to_id> LABEL <label> [WEIGHT <weight>]`
- `REMOVE NODE <id>`
- `REMOVE EDGE <id>`

**Properties & Indexing**
- `SET PROPERTY ON NODE <id> KEY <key> VALUE <value>`
- `SET PROPERTY ON EDGE <id> KEY <key> VALUE <value>`
- `CREATE INDEX ON <property_key>`

**Querying & Traversal**
- `GET NODE <id>`
- `GET EDGE <id>`
- `PRINT GRAPH`
- `BFS FROM <start_node_id>`
- `DFS FROM <start_node_id>`
- `SHORTEST PATH FROM <start_node_id> TO <end_node_id>`

**Disk Storage**
- `SAVE <filename>.db`
- `LOAD <filename>.db`

## 📂 Project Architecture

- `src/core/`: Contains the fundamental Graph, Node, and Edge entities, plus the algorithmic implementations.
- `src/Index/`: Handles the B+ Tree structures for property indexing.
- `src/storage/`: Manages disk serialization and raw block reading/writing.
- `src/buffer/`: Implements the Buffer Pool and LRU caching mechanisms.
- `src/query/`: Parses string queries from the CLI into executable internal commands.
- `tests/`: Contains the GoogleTest suite validating database integrity and thread-safety.
