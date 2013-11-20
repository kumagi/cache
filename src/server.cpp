#include <stdint.h>
#include <pthread.h>

#include <vector>
#include <boost/shared_ptr.hpp>

#define EVTHREAD_USE_PTHREADS_IMPLEMENTED 1
#include <event2/event.h>

#include "server_config.hpp"
#include "hash_table.hpp"


struct shared_workingset {
  event_base* const base;
  shared_workingset(event_base* const b)
    : base(b)
  {}
};

struct worker {
  worker(int t)
    :tid(t) {}

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
  boost::shared_ptr<const shared_workingset> sws;
  int tid;
};

class cache_server {
public:
  cache_server(const server_config& conf)
    : workers_(conf.threads) {
    sws_ = boost::shared_ptr<shared_workingset>
        (new shared_workingset(event_base_new()));
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
  boost::shared_ptr<const shared_workingset> sws_;
  std::vector<worker> workers_;
};

int main(int argc, char** argv) {
  const server_config conf(argc, argv);
  cache_server serv(conf);
  serv.start();
  return serv.join();
}
