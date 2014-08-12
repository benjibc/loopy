#ifndef LIBRARY_SYS_LMIDDLEWARE_H_
#define LIBRARY_SYS_LMIDDLEWARE_H_

#include "./lreq.h"
#include "./lres.h"

class LMiddleware {
 public:
  LMiddleware();
  virtual process(LReq& req, LRes& res) = 0;
}

#endif  // LIBRARY_SYS_LMIDDLEWARE_H_
