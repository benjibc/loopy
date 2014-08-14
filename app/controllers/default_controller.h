#ifndef LOOPY_APP_DEFAULT_CONTROLLER
#define LOOPY_APP_DEFAULT_CONTROLLER

#include <library/sys/lcontroller.h>
#include <string>
#include <boost/coroutine/coroutine.hpp>

namespace loopy {

class DefaultController: public LController {
  public:
    DefaultController();
    virtual ~DefaultController() = default;

    // handlers for this class
    void Hello              (LReq& req, LRes& res) const;
    void FileNotFound       (LReq& req, LRes& res) const;
    void ComplexHello       (LReq& req, LRes& res) const;
    void SubComplexHello    (LReq& req, LRes& res) const;
    void SubComplexHello2   (LReq& req, LRes& res) const;

  private:
    std::string name;
};

}

#endif
