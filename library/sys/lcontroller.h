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
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include "./lreq.h"
#include "./lres.h"
#include "./lpromise.h"

namespace loopy {
class LServer;
class LController;

typedef void (LController::*LHandler)();
typedef LController*(*ctrllerFactoryFunc)(pReq req);
typedef std::shared_ptr<LController> CtrllrPtr;
typedef std::tuple<ctrllerFactoryFunc, LHandler> LCtrlHandler;

/// Controller class that every custom made controller should inherit from.
/// Every time a request is made, a new controller will be created, so make
/// sure if you want to do some change to the controller and want to retain
/// the change, make sure you operate on _threadData
class LController {
 public:
  /// constructor that takes libevhtp's request as a parameter
  LController() = delete;
  explicit LController(pReq req);
  virtual ~LController() = default;

  /// calls for the next controller
  void next(const char* method, const char* next, const char* templateName);

  static bool invalidControlHandler(LCtrlHandler ctrlHandler);

  /// container for the promise
  template<typename PromisePtrTypes, typename LambdaType>
  void async(PromisePtrTypes promise, LambdaType callback);

  /// check if the controller is an async controller
  bool isAsync() const; 

  /// execute the promises contained
  void execPromises();

  /// returns a reference to the response object
  LRes& res();
  /// get the raw libevhtp request object
  pReq  rawReq();

 protected:
  /// request variable for the controller
  LReq req_;
  /// response variable for the controller
  LRes res_;

  /// thread variable
  evthr_t* thread_;
  /// event base
  evbase_t* evbase_;

  /// thread local variable
  ThreadLocal* threadLocal_;

 private:
  /// an array of promises initiated by the user
  std::vector<std::shared_ptr<LPromiseBase>> promises;
};

/// just takes the promise and returns it
template<typename PromisePtrTypes, typename LambdaType>
void LController::async(PromisePtrTypes promise, LambdaType callback) {
  promise->then(callback);
  promises.push_back(promise);
}

} // namespace loopy

#endif  // LIBRARY_SYS_LCONTROLLER_H_
