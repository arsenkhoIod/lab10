//Copyright [2021] <nickgeo.winner@gmail.com>

#ifndef INCLUDE_SAFE_QUEUE_HPP_
#define INCLUDE_SAFE_QUEUE_HPP_
#include <iostream>
#include <queue>
#include <mutex>
#include <utility>

template <typename T>
class safe_queue {
 public:
  T front();
  T pop();
  void push(const T& obj);
  void push(T&& obj);
  bool is_empty();

 private:
  std::queue<T> _queue;
  std::mutex _mutex;
};

template <typename T>
T safe_queue<T>::front() {
  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.front();
}

template <typename T>
T safe_queue<T>::pop() {
  std::lock_guard<std::mutex> lock(_mutex);
  if (!_queue.empty()){
    T tmp = _queue.front();
    _queue.pop();
    return tmp;
  } else {
    throw std::runtime_error("Queue is empty");
  }
}

template <typename T>
void safe_queue<T>::push(const T& obj) {
  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.push(obj);
}

template <typename T>
void safe_queue<T>::push(T&& obj) {
  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.push(std::forward<T>(obj));
}

template <typename T>
bool safe_queue<T>::is_empty() {
  return _queue.empty();
}

#endif  // INCLUDE_SAFE_QUEUE_HPP_
