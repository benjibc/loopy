#ifndef LIBRARY_SYS_LDRIVER_H_
#define LIBRARY_SYS_LDRIVER_H_

#include <stdexcept>
#include <string>

namespace loopy {

/// One important note for LDriver: Drivers are not initialized per request; they
/// are initialized per thread. In this case, we will only call this function
class LDriver {
 public:
  explicit LDriver(evthr_t* thread)
    : thread_(thread)
  {}

  virtual void DBConnect() = 0;
  virtual void DBDisconnect() = 0;
  /// FIXME: naming
  virtual std::string DriverName() const = 0;

 protected:
  /// thread related variables that a thread can register to
  evthr_t* thread_;
  /// event base library that driver can register to
  evbase_t* evbase_;
  /// thread local data associated with the thread
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
