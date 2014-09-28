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
  void execPromise();

  /// returns a reference to the response object
  LRes& res();
  /// get the raw libevhtp request object
  pReq  rawReq();

  /// set a controller to be a subroutine
  void injectSubTemplate(ctemplate::TemplateDictionary* dict) {
    _res.subroutine(true);
    // dict is now swapped with the old template inside res. delete
    // dict would free the old template
    _res.swapTemplateParams(dict);
    delete dict;
  }

  std::string getSubtemplateFilename() const {
    return _res.getIncludedTemplateFilename();
  }

 protected:
  /// thread variable
  evthr_t* _thread;
  /// thread local variable
  ThreadLocal* _threadLocal;
  /// request variable for the controller
  LReq _req;
  /// response variable for the controller
  LRes _res;

  /// event base
  evbase_t* _evbase;

 private:
  /// an array of promises initiated by the user
  std::shared_ptr<LPromiseBase> _promise;
};

/// just takes the promise and returns it
template<typename PromisePtrTypes, typename LambdaType>
void
LController::async(PromisePtrTypes promise, LambdaType callback) {
  promise->then(callback);
  _promise = promise;
}

// helper function to make and free dummy requests
template<typename T>
evhtp_request_t * new_dummy_request(LController* ctrllr) {

  evbase_t * evbase = event_base_new();
  evhtp_t  * htp    = evhtp_new(evbase, NULL);

  evhtp_request_t * req;

  evhtp_connection_t* conn = new_dummy_conn(htp);
  conn->thread = evthr_new(
    dummyInitializeThread,
    static_cast<void*>(T::initThread)
  );
  // int result = evthr_start(conn->thread);
  // FIXME: it seems to me that the compiler reordered the place where
  // pthread_create is called, so yield control here and trick the OS into
  // creating the thread. Not the right way of doing things. But why does
  // the callback for pthread_create not get called
  std::cout << "Sleeping to wait for the thread. FIXME!!" << std::endl;
  sleep(1);

  evhtp_uri_t * uri = new evhtp_uri_t;
  uri->path = new evhtp_path_t;
  uri->path->full = new char[2];
  uri->path->full[0] = '/';
  uri->path->full[1] = '\0';
  uri->path->file = new char[2];
  uri->path->file[0] = '/';
  uri->path->file[1] = '\0';
  uri->path->path = new char[2];
  uri->path->path[0] = '/';
  uri->path->path[1] = '\0';


  if (!(req = new evhtp_request_t)) {
    return nullptr;
  }

  req->conn = conn;
  req->htp = htp;
  req->uri = uri;
  req->status = EVHTP_RES_OK;
  req->buffer_in = evbuffer_new();
  req->buffer_out = evbuffer_new();
  req->headers_in = new evhtp_headers_t;
  req->headers_out = new evhtp_headers_t;
  req->cbarg = static_cast<void*>(ctrllr);
  TAILQ_INIT(req->headers_in);
  TAILQ_INIT(req->headers_out);
  return req;
}

void free_dummy_request(evhtp_request_t* req);

} // namespace loopy

#endif  // LIBRARY_SYS_LCONTROLLER_H_
