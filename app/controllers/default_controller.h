#ifndef APP_CONTROLLERS_DEFAULT_CONTROLLER_H_
#define APP_CONTROLLERS_DEFAULT_CONTROLLER_H_

#include <library/sys/lcontroller.h>
#include <string>

namespace loopy {

class DefaultController: public LController {
 public:
  explicit DefaultController(pReq req);
  virtual ~DefaultController() = default;

  // handlers for this class
  void Hello();
  void FileNotFound();
  void ComplexHello();
  void SubComplexHello();
  void SubComplexHello2();

 private:
  std::string name;
};

} // namespace loopy

#endif  // APP_CONTROLLERS_DEFAULT_CONTROLLER_H_
