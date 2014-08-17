#ifndef LIBRARY_SYS_LDRIVER_H_
#define LIBRARY_SYS_LDRIVER_H_

#include <stdexcept>
#include <string>

namespace loopy {
class LDriver {
 public:
  explicit LDriver(evthr_t* thread);
  // 4 public functions that the child driver must implement
  virtual void DBConnect() = 0;
}

class DriverError : std::runtime_error {
  class DriverError(const char* what_arg)
    : runtime_error(what_arg)
  {}

  class DriverError(const char* what_arg)
    : runtime_error(what_arg)
  {}
}

}  // namespace loopy
#endif  // LIBRARY_SYS_LDRIVER_H_
