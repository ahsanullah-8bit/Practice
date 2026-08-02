/*
    Thread‑Safe Task Queue

    Features: std::thread, std::mutex, std::condition_variable, std::function, std::packaged_task, std::future, move semantics.  
    Goal: Build a multi‑producer, multi‑consumer task queue.
    Plan:

    * Design the queue interface: enqueue(F&& f), run(), stop().
    * Implement the queue using a std::deque of std::packaged_task, a mutex, and a condition variable. Use std::function<void()> for type erasure.
    * Test with several threads pushing lambdas and waiting on futures.

    Notes:
        * This task is worded wrong. It's talking about a thread-pool, not a "queue".
        * Some things related to the ImprovedTaskQueue
            - Variadic Templates
                https://en.cppreference.com/cpp/language/pack
                https://en.cppreference.com/cpp/language/variadic_arguments
            - Universal and Forwarding References
                https://en.cppreference.com/cpp/language/reference
            - Callable Binding
                https://en.cppreference.com/cpp/utility/functional/bind
            - result_of, instead of decltype
                https://en.cppreference.com/cpp/types/result_of
            
*/


#include <atomic>
#include <cassert>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

/*
    TaskQueue
    Takes basic callables with no arguements, as per the requirements.
*/
class TaskQueue {
    using task_t = std::function<void()>;
public:

    void enqueue(task_t &&t) {
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_tasks.emplace_back(std::move(t));
        }
        m_cond.notify_one();
    }

    template<typename F>
    auto enqueue_with_future(F &&f) -> std::future<decltype(f())> {
        using R = decltype(f());
        auto task = std::make_shared<std::packaged_task<R()>>(std::forward<F>(f));
        auto fut = task->get_future();
        enqueue([task] { (*task)(); });
        return fut;
    }

    void run(int num_of_threads) {
        m_running = true;
        for (int i = 0; i < num_of_threads; ++i)
            m_threads.emplace_back(&TaskQueue::worker, this);
    }
    void stop() {
        m_running = false;
        m_cond.notify_all();
        for (auto &t : m_threads)
            if (t.joinable())
                t.join();
    }

private:
    void worker() {
        while (true) {
            task_t t;
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                m_cond.wait(lock, [this] { return !m_running || !m_tasks.empty(); });
                if (!m_running && m_tasks.empty())
                    break;

                t = std::move(m_tasks.front());
                m_tasks.pop_front();
            }
            t();
        }
    }

private:
    std::mutex m_mtx;
    std::condition_variable m_cond;
    std::atomic_bool m_running;
    std::vector<std::thread> m_threads;
    std::deque<std::function<void()>> m_tasks;
};


/*
    ImprovedTaskQueue
    Takes callables with arguements as well
*/
class ImprovedTaskQueue {
    using task_t = std::function<void()>;
public:
    void enqueue(task_t &&t) {
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_tasks.emplace_back(std::move(t));
        }
        m_cond.notify_one();
    }

    template<typename F, typename ...Args>
    auto enqueue_with_future(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using R = typename std::result_of<F(Args...)>::type;
        auto bound_f = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task = std::make_shared<std::packaged_task<R()>>(bound_f);
        auto fut = task->get_future();
        enqueue([task] { (*task)(); });
        return fut;
    }

    void run(int num_of_threads) {
        m_running = true;
        for (int i = 0; i < num_of_threads; ++i)
            m_threads.emplace_back(&ImprovedTaskQueue::worker, this);
    }

    void stop() {
        m_running = false;
        m_cond.notify_all();
        for (auto &t : m_threads)
            if (t.joinable())
                t.join();
    }

private:
    void worker() {
        while (true) {
            task_t t;
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                m_cond.wait(lock, [this] { return !m_running || !m_tasks.empty(); });
                if (!m_running && m_tasks.empty())
                    break;

                t = std::move(m_tasks.front());
                m_tasks.pop_front();
            }
            t();
        }
    }

private:
    std::mutex m_mtx;
    std::atomic_bool m_running;
    std::deque<task_t> m_tasks;
    std::condition_variable m_cond;
    std::vector<std::thread> m_threads;
};

// All tests are written by AI. My mind is a bit slow, when it comes to thinking of all the invalid scenarios.
// ============================================================================
// TEST MACRO AND SUITE FOR BOTH QUEUE CONFIGURATIONS
// ============================================================================

#define RUN_TEST(test, queue_type) \
    do { \
        std::cout << "Using " #queue_type " on " #test "... "; \
        test<queue_type>(); \
        std::cout << "OK\n"; \
    } while(0)

template <typename TQueue>
void test_single_task() {
    TQueue q;
    q.run(1);
    int value = 0;
    q.enqueue([&value] { value = 42; });
    q.stop();
    assert(value == 42);
}

template <typename TQueue>
void test_future_task() {
    TQueue q;
    q.run(1);
    auto fut = q.enqueue_with_future([] { return 17; });
    assert(fut.get() == 17);
    q.stop();
}

template <typename TQueue>
void test_multiple_futures() {
    TQueue q;
    q.run(4);
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 100; ++i)
        futures.push_back(q.enqueue_with_future([i] { return i * i; }));

    for (int i = 0; i < 100; ++i)
        assert(futures[i].get() == i * i);

    q.stop();
}

template <typename TQueue>
void test_multiple_producers() {
    TQueue q;
    q.run(4);
    std::atomic<int> counter{0};
    constexpr int tasks_per_producer = 250;
    constexpr int producers = 4;

    auto producer = [&] {
        for (int i = 0; i < tasks_per_producer; ++i) {
            q.enqueue([&counter] { counter++; });
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < producers; ++i)
        threads.emplace_back(producer);
    for (auto& t : threads) t.join();

    q.stop();
    assert(counter == producers * tasks_per_producer);
}

template <typename TQueue>
void test_stop_while_waiting() {
    TQueue q;
    q.run(1);
    std::atomic<bool> started{false};
    auto fut = q.enqueue_with_future([&] {
        started = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return 10;
    });
    while (!started) { std::this_thread::yield(); }
    q.stop(); 
    assert(fut.get() == 10);
}

template <typename TQueue>
void test_stop_idle_queue() {
    TQueue q;
    q.run(2);
    q.stop();
    assert(true); 
}

template <typename TQueue>
void test_order_not_required_but_all_processed() {
    TQueue q;
    q.run(2);
    std::atomic<int> sum{0};
    constexpr int N = 200;
    for (int i = 0; i < N; ++i)
        q.enqueue([i, &sum] { sum += i; });
    q.stop();
    assert(sum == N * (N - 1) / 2);
}

template <typename TQueue>
void test_empty_queue_immediate_stop() {
    TQueue q;
    q.run(1);
    q.stop(); 
}

// Unique check for ImprovedTaskQueue to test passing multi-variable parameters
int multiply(int a, int b) { return a * b; }

void test_improved_queue_arguments() {
    ImprovedTaskQueue q;
    q.run(2);
    
    auto fut1 = q.enqueue_with_future(multiply, 6, 7);
    auto fut2 = q.enqueue_with_future([](const std::string& prefix, int val) {
        return prefix + std::to_string(val);
    }, "Value: ", 100);
    
    assert(fut1.get() == 42);
    assert(fut2.get() == "Value: 100");
    q.stop();
}

// ============================================================================
// MAIN RUNNER
// ============================================================================

int main() {
    std::cout << "--- Testing TaskQueue ---\n";
    RUN_TEST(test_single_task, TaskQueue);
    RUN_TEST(test_future_task, TaskQueue);
    RUN_TEST(test_multiple_futures, TaskQueue);
    RUN_TEST(test_multiple_producers, TaskQueue);
    RUN_TEST(test_stop_while_waiting, TaskQueue);
    RUN_TEST(test_stop_idle_queue, TaskQueue);
    RUN_TEST(test_order_not_required_but_all_processed, TaskQueue);
    RUN_TEST(test_empty_queue_immediate_stop, TaskQueue);

    std::cout << "\n--- Testing ImprovedTaskQueue ---\n";
    RUN_TEST(test_single_task, ImprovedTaskQueue);
    RUN_TEST(test_future_task, ImprovedTaskQueue);
    RUN_TEST(test_multiple_futures, ImprovedTaskQueue);
    RUN_TEST(test_multiple_producers, ImprovedTaskQueue);
    RUN_TEST(test_stop_while_waiting, ImprovedTaskQueue);
    RUN_TEST(test_stop_idle_queue, ImprovedTaskQueue);
    RUN_TEST(test_order_not_required_but_all_processed, ImprovedTaskQueue);
    RUN_TEST(test_empty_queue_immediate_stop, ImprovedTaskQueue);
    
    std::cout << "Running test_improved_queue_arguments with ImprovedTaskQueue... ";
    test_improved_queue_arguments();
    std::cout << "OK\n";

    std::cout << "\nAll test macros executed successfully.\n";
    return 0;
}
