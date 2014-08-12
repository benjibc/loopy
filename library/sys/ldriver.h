#ifndef LIBRARY_SYS_LDRIVER_H_
#define LIBRARY_SYS_LDRIVER_H_

namespace loopy {
class LDriver {
 public:
  LDriver();
 private:
  // 4 private functions that the child driver must implement
  virtual void DBConnect() = 0;
  virtual void DBStart() = 0;
  virtual void DBContinue() = 0;
  virtual void DBEnd() = 0;
}
}  // namespace loopy
#endif  // LIBRARY_SYS_LDRIVER_H_
