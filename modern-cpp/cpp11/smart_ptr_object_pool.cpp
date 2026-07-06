/*
    Smart‑Pointer Object Pool

    Features: std::unique_ptr, std::shared_ptr, std::make_shared, custom deleters, move semantics.
    Goal: A fixed‑size pool that lends unique_ptr or shared_ptr handles; returning the pointer recycles the object.

    Plan:
    * Define a pool of pre‑allocated objects.
    * Implement acquire() returning a unique_ptr with a custom deleter that marks the slot free.
    * Test with multiple threads acquiring and releasing.
*/

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

template<typename T>
class ObjectPool {
public:
    explicit ObjectPool(size_t capacity) 
        : m_acquired(capacity, false)
    {
        for (size_t i = 0; i < capacity; ++i)
            m_pool.emplace_back(new T);
    }

    size_t count() {
        std::lock_guard<std::mutex> lock(m_mtx);
        return std::count_if(m_acquired.cbegin(), m_acquired.cend(), [] (uint8_t a) { return !a; });
    }

    size_t capacity() {
        std::lock_guard<std::mutex> lock(m_mtx);
        return m_acquired.size();
    }

    std::unique_ptr<T> acquireUnique() {
        std::lock_guard<std::mutex> lock(m_mtx);
        for (size_t i = 0; i < m_acquired.size(); ++i) {
            if (m_acquired.at(i))
                continue;

            m_acquired[i] = true;
            return std::unique_ptr<T, std::function<void(T*)>> (m_pool.at(i).get(),
                [this, i] (T *p) {
                    std::lock_guard<std::mutex> lock(m_mtx);
                    m_acquired[i] = false;
                    // Leave the p
                }
            );
        }

        return nullptr;
    }
    std::shared_ptr<T> acquireShared() {
        std::lock_guard<std::mutex> lock(m_mtx);
        for (size_t i = 0; i < m_acquired.size(); ++i) {
            if (m_acquired.at(i))
                continue;

            m_acquired[i] = true;
            return std::shared_ptr<T> (m_pool.at(i).get(),
                [this, i] (T *p) {
                    std::lock_guard<std::mutex> lock(m_mtx);
                    m_acquired[i] = false;
                    // Leave the p
                }
            );
        }

        return nullptr;
    }

private:
    std::mutex m_mtx;
    std::vector<uint8_t> m_acquired;
    std::vector<std::unique_ptr<T>> m_pool;
};

#define NUM_OF_THREADS 4
#define MAX_OBJECTS 50

int main() {
    typedef struct Type {
        int hits = 0;
        std::string s;
    } Type;
    ObjectPool<Type> pool(MAX_OBJECTS);
    std::vector<std::thread> threads;

    for (size_t t = 0; t < NUM_OF_THREADS; ++t) {
        threads.emplace_back([](ObjectPool<Type> &pool, size_t t) {
            for (size_t i = 0; i < MAX_OBJECTS / 2; ++i) {
                std::shared_ptr<Type> s = pool.acquireShared();
                if (!s) continue;

                s->s.append(std::to_string(t) + ", ");
                s->hits++;
                // std::this_thread::yield();
            }
        }, std::ref(pool), t);
    }

    for (auto &t : threads) {
        if (t.joinable())
            t.join();
    }

    assert(pool.count() == pool.capacity()); // Are all freed?
    // See all the modified ones
    for (size_t i = 0; i < pool.capacity(); ++i) {
        auto s = pool.acquireShared();
        std::cout << std::to_string(i) << ": Hits " << s->hits << std::endl;
    }
}