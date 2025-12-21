# Multi-Order Type Orderbook

A high-performance order matching engine implemented in modern C++20





| Operation | Time Complexity |
|-----------|-----------------|
| Add Order | O(log n) |
| Cancel Order | O(1) lookup + O(log n) removal |
| Best Bid/Ask | O(1) |
| Match Orders | O(k) where k = orders matched |
| Order Lookup | O(1) |

---

### Prerequisites
- GCC 11+ or Clang 14+ with C++20 support
- Linux / macOS / Windows (with WSL)





