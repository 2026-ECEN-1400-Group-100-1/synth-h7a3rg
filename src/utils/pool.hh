#pragma once

#include <cstddef>

namespace utils {

// forward declare for PoolNode friend
template <typename T> class PoolManager;

template <typename T>
struct PoolNode {
    friend class PoolManager<T>;

    PoolNode() = default;

    // No move or copy for PoolNodes
    // (it would fuck up the linked list and array in PoolManager)
    PoolNode(const PoolNode& node) = delete;
    PoolNode(PoolNode&& node) = delete;

    // conversion operators
    // TODO(?) this will shit itself at runtime if T is not copy constructable
    operator T() const { return _data; }
    operator T&() { return _data; }

    T& data() { return _data; }

private:
    PoolNode* _next = nullptr;

    T _data;
};

template <typename T>
class PoolManager {
public:
    PoolManager(PoolNode<T>* pool, size_t pool_size)
        : _pool(pool), _pool_size(pool_size) { link_nodes(); }

    /// @brief Allocate one PoolNode<T> from the pool. Returns nullptr if the pool is full
    /// @note Has junk data, like malloc()
    [[nodiscard]]
    PoolNode<T>* alloc_one();

    /// @brief Returns a PoolNode to the pool
    /// @note Consumes the PoolNode, this works like free()
    void free_one(PoolNode<T>* node);

    /// @brief Returns the current number of nodes used in the pool
    [[nodiscard]]
    size_t size() const { return _size; }

    /// @brief Returns the max number of nodes that can be stored in the pool
    [[nodiscard]]
    size_t max_size() const { return _pool_size; }

private:
    void link_nodes();

    PoolNode<T>* const _pool;
    const size_t _pool_size;

    size_t _size = 0;

    PoolNode<T>* _head = nullptr;
};

} // namespace utils
