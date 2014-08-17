#ifndef LIBRARY_SYS_LDRIVER_H_
#define LIBRARY_SYS_LDRIVER_H_

#include <stdexcept>
#include <string>

namespace loopy {

// composition over inheritance
class LDriver {
 public:
  explicit LDriver(evthr_t* thread)
    : thread_(thread)
  {}

  virtual void DBConnect();

 protected:
  evthr_t* thread_;
};

class DriverError : std::runtime_error {
 public:
  DriverError(const char* what_arg)
    : runtime_error(what_arg)
  {}

  DriverError(const std::string& what_arg)
    : runtime_error(what_arg)
  {}
};

}  // namespace loopy
#endif  // LIBRARY_SYS_LDRIVER_H_
