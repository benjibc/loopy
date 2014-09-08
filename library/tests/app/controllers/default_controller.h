#ifndef LOOPY_APP_DEFAULT_CONTROLLER
#define LOOPY_APP_DEFAULT_CONTROLLER

#include <library/sys/lcontroller.h>
#include <string>

namespace loopy {

class DefaultController: public LController {
  public:
    DefaultController(pReq req);
    virtual ~DefaultController() = default;

    // handlers for this class
    void Hello();
    void FileNotFound();
    void AsyncHello();
    void ComplexHello();
    void SubComplexHello();
    void SubComplexHello2();

    static void initThread(evthr_t* thread);

  private:
    std::string name;
    // store a reference to the redis server
    LRedis* redis;
};

}

#endif
