#ifndef UTIL_BACKGROUND_THREAD_MANAGER_H_
#define UTIL_BACKGROUND_THREAD_MANAGER_H_

#include <atomic>
#include <functional>
#include <thread>
#include <vector>

namespace ms {

class BackgroundThreadManager {
 public:
  using Callback = std::function<std::chrono::milliseconds()>;

  BackgroundThreadManager();
  virtual ~BackgroundThreadManager();

  void Stop();
  void StopAndWarn(const std::string& destructor);
  void Add(Callback cb,
           std::chrono::milliseconds initial_sleep_amount = std::chrono::milliseconds(0));
  size_t size() const;

 private:
  void ExecuteInLoop(Callback cb,
                     std::chrono::milliseconds initial_sleep_amount) const;
  void IncrementSleep(std::chrono::milliseconds amount) const;

  std::atomic<bool> keep_running_;
  std::vector<std::thread> threads_;

};

} // namespace ms
#endif // UTIL_BACKGROUND_THREAD_MANAGERH_
