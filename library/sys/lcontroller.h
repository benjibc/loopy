// The MIT License (MIT)
// Copyright (c) 2014 Yufei (Benny) Chen
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef LIBRARY_SYS_LCONTROLLER_H_
#define LIBRARY_SYS_LCONTROLLER_H_

#include <evhtp.h>
#include <unordered_map>
#include <string>
#include "./lreq.h"
#include "./lres.h"

namespace loopy {
class LServer;
class LController;

typedef void (LController::*LHandler)(LReq&, LRes&) const;
typedef std::shared_ptr<LController> ctrllrPtr;
typedef std::tuple<ctrllrPtr, LHandler> LCtrlHandler;

class LController {
 public:
  LController();
  virtual ~LController() = default;
  void next(
    const char* method,
    const char* next,
    const char* templateName,
    LReq& req,
    LRes& res
  ) const;
  static bool invalidControlHandler(LCtrlHandler ctrlHandler);
};


} // namespace loopy

#endif  // LIBRARY_SYS_LCONTROLLER_H_
