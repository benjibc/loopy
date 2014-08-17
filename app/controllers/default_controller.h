#ifndef APP_CONTROLLERS_DEFAULT_CONTROLLER_H_
#define APP_CONTROLLERS_DEFAULT_CONTROLLER_H_

#include <library/sys/lcontroller.h>
//#include <drivers/loopy-redis/loopy-redis.h>
#include <string>

namespace loopy {

class DefaultController: public LController {
 public:
  explicit DefaultController(pReq req);
  virtual ~DefaultController() = default;

  // handlers for this class
  void Hello();
  void FileNotFound();
  void AsyncHello();
  void ComplexHello();
  void SubComplexHello();
  void SubComplexHello2();

  void initializeThread(evthr_t* thread) const;

  // static void initializeFromThread(evhtp_t* thread);

 private:
  std::string name;
  int i = 0;
};

} // namespace loopy

#endif  // APP_CONTROLLERS_DEFAULT_CONTROLLER_H_
