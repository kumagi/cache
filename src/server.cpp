#include <stdint.h>
#include <pthread.h>

#include <vector>

#define EVTHREAD_USE_PTHREADS_IMPLEMENTED 1
#include <event2/event.h>

#include "server_config.hpp"
#include "hash_table.hpp"

template <typename T>
class scoped_ptr {
  // duty of delete
public:
  scoped_ptr(T* p)
    : ptr_(p){ }
  ~scoped_ptr() {
    delete ptr_;
  }
  T* get() { return ptr_; }
  const T* get() const { return ptr_; }
  T& operator*() { return *ptr_; }
  T* operator->() { return ptr_; }
  const T& operator*() const { return *ptr_; }
  const T* operator->() const { return ptr_; }
private:
  T* ptr_;
};

template <typename T>
class const_ptr {
  // duty of delete
public:
  const_ptr() :ptr_(NULL) {}
  T* operator=(const scoped_ptr<T>& rhs) {
    return ptr_ = rhs.get();
  }
  T* operator=(const T* rhs) {
    return ptr_ = rhs;
  }
  const_ptr(T* p)
    : ptr_(p){ }
  ~const_ptr() { /* do nothing! */ }
  const T& operator*() const { return *ptr_; }
  const T* operator->() const { return ptr_; }
private:
  const T* ptr_;
};

struct shared_workingset {
  event_base* const base;
  shared_workingset(event_base* const b)
    : base(b)
  {}
};

struct worker {
  worker() {}

  void start() {
    pthread_create(&thread, NULL, worker::worker_main, this);
  }

  static void* worker_main(void* ws) {
    worker* my_workingset = (worker*)ws;
    event_base_loop(my_workingset->sws->base, 0);
    return NULL;
  }

  // thread specific data
  pthread_t thread;
  const_ptr<const shared_workingset> sws;
  int tid;
};

class cache_server {
public:
  cache_server(const server_config& conf)
    :sws_(new shared_workingset(event_base_new())),
     workers_(conf.threads) {
    for (size_t i = 0; i < conf.threads; ++i) {
      workers_[i].sws = sws_;
      workers_[i].start();
    }
  }

  void start() {}
  int join() {
    return true;
  }
  hash_table table_;
  scoped_ptr<const shared_workingset> sws_;
  std::vector<worker> workers_;
};

int main(int argc, char** argv) {
  const server_config conf(argc, argv);
  cache_server serv(conf);
  serv.start();
  return serv.join();
}
