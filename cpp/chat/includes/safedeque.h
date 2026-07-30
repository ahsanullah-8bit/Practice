#pragma once
#include <deque>
#include <mutex>

template <typename T>
class SafeDeque {
public:
	SafeDeque() = default;
	SafeDeque(const SafeDeque<T>&) = delete;

	std::deque<T> getDeque() {
		std::scoped_lock<std::mutex> l(m_mutex);
		return m_deque;
	}

	const T& front() {
		std::scoped_lock<std::mutex> l(m_mutex);
		return m_deque.front();
	}
	
	const T& back() {
		std::scoped_lock<std::mutex> l(m_mutex);
		return m_deque.back();
	}
	
	void push_front(const T& val) {
        std::scoped_lock<std::mutex> l(m_mutex);
        m_deque.push_front(val);
	}
	
	void push_front(T&& val) {
        std::scoped_lock<std::mutex> l(m_mutex);
        m_deque.push_front(val);
	}

	void push_back(const T& val) {
        std::scoped_lock<std::mutex> l(m_mutex);
        m_deque.push_back(val);
	}

	void push_back(T&& val) {
        std::scoped_lock<std::mutex> l(m_mutex);
        m_deque.push_back(val);
	}

	bool empty() {
		std::scoped_lock<std::mutex> l(m_mutex);
		return m_deque.empty();
	}

	std::size_t size() {
		std::scoped_lock<std::mutex> l(m_mutex);
		return m_deque.size();
	}

	T pop_front() {
		std::scoped_lock<std::mutex> l(m_mutex);
		auto res = std::move(m_deque.front());
		m_deque.pop_front();
		return res;
	}

	T pop_back() {
		std::scoped_lock<std::mutex> l(m_mutex);
		auto res = std::move(m_deque.back());
		m_deque.pop_back();
		return res;
	}

	void remove(const T& val) {
		std::scoped_lock<std::mutex> l(m_mutex);
		m_deque.erase(std::remove(m_deque.begin(), m_deque.end(), val), m_deque.end());
	}

private:
	std::deque<T> m_deque;
	std::mutex m_mutex;
};
