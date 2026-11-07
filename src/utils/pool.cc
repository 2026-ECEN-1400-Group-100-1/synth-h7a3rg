#include "pool.hh"

namespace utils {

template<typename T>
PoolNode<T>* PoolManager<T>::alloc_one() {
    if (_size >= _pool_size)
        return nullptr;

    ++_size;

    // pop head
    PoolNode<T>* ret = _head;
    _head = _head->_next;

    return ret;
}

template<typename T>
void PoolManager<T>::free_one(PoolNode<T>* node) {
    // add this one as head
    node->_next = _head;
    _head = node;

    --_size;
}

template<typename T>
void PoolManager<T>::link_nodes() {
    for (size_t i = 0; i < _pool_size-1; ++i) {
        _pool[i]._next = &_pool[i+1];
    }
}

} // namespace utils
