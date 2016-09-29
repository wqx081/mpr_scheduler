#include "util/background_thread_manager.h"

#include <glog/logging.h>

namespace ms {

namespace {

static const int kIncrementSleepMS = 1000;

} // namespace

BackgroundThreadManager::BackgroundThreadManager() 
    : keep_running_(true) {
}

BackgroundThreadManager::~BackgroundThreadManager() {
  StopAndWarn("BackgroundThreadManager");
}

void BackgroundThreadManager::StopAndWarn(const std::string& msg) {
  if (keep_running_.load()) {
    LOG(ERROR) << msg;
    Stop();
  }
}

void BackgroundThreadManager::Stop() {
  if (keep_running_.exchange(false)) {
    for (auto& t : threads_) {
      t.join();
    }
  }
}

void BackgroundThreadManager::Add(Callback cb, std::chrono::milliseconds amount) {
  // Create new Thread
  threads_.emplace_back(&BackgroundThreadManager::ExecuteInLoop, this, cb, amount);
}

void BackgroundThreadManager::ExecuteInLoop(Callback cb,
                                            std::chrono::milliseconds sleep_amount) const {
  IncrementSleep(sleep_amount);
  while (keep_running_.load()) {
    IncrementSleep(cb());
  }
}

void BackgroundThreadManager::IncrementSleep(std::chrono::milliseconds amount) const {
  auto s = amount > std::chrono::milliseconds(kIncrementSleepMS) ?
                    std::chrono::milliseconds(amount) : amount;
  std::this_thread::sleep_for(s);
  amount -= s;
}

} // namespace ms
