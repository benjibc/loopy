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
#ifndef LIBRARY_SYS_LSERVER_H_
#define LIBRARY_SYS_LSERVER_H_

#include <evhtp.h>
#include <event.h>
#include <memory>
#include <string>
#include <cstdint>
#include <typeinfo>
#include "../threadshared.h"
#include "../threadlocal.h"
#include "./lrouter.h"
#include "./utils.h"

namespace loopy {

// I don't want this class to be extended, so the destructor is not virtual
class LServer {
 public:
  static unsigned getNumCPU();
  static LServer& getInstance(
    const char* address     = "127.0.0.1",
    uint16_t    port        = 80,
    uint16_t    num_threads = getNumCPU(),
    int         backlog     = 1024
  );

  // main entry function to call when you have setup everything and want to
  // make the server up and running
  void run();

  LCtrlHandler getCtrlHandler(const char* method, const char* path);
  LCtrlHandler getCtrlHandlerStrict(const char* method, const char* path);

  static void serveRequest(LCtrlHandler ctrlHandler, LReq& req, LRes& res);
  static void invalidControlHandler(LCtrlHandler ctrlHandler);

  // list of HTTP Methods to use as routing functions
  template<class T>
  void post(const char* path, void (T::*handler)(LReq&, LRes&) const);

  template<class T>
  void get(const char* path, void (T::*handler)(LReq&, LRes&) const);

  template<class T>
  void custom(
    const char* method,
    const char* path,
    void (T::*handler)(LReq&, LRes&) const
  );

  // intern is not a valid HTTP request method. It is used for dedicated
  // internal handlers
  template<class T>
  void intern(const char* path, void (T::*handler)(LReq&, LRes&) const);

  // setters for the special handlers when something goes wrong
  // these handlers are used when File Not Found occurs or when there
  // is a server error
  template<class T>
  void setHandlerFor404(void (T::*handler)(LReq&, LRes&) const);

  template<class T>
  void setHandlerFor500(void (T::*handler)(LReq&, LRes&) const);

 private:
  // lock the constructor to make sure the server is a singleton
  LServer(
    const char* address,
    uint16_t    port,
    uint16_t    num_threads,
    int         backlog
  );

  // disable copy constructor and assignment operator
  LServer(LServer const&) = delete;
  LServer& operator=(LServer const&) = delete;

  // members of the class
  evbase_t* _evbase;
  std::unique_ptr<evhtp_t>      _htp;
  std::string                   _addr;
  uint16_t                      _port;
  uint16_t                      _num_threads;
  int                           _backlog;

  // router to route all the requests
  LRouter                       _router;
};

void processRequest(evhtp_request_t* request, void* arg);
void initializeThread(evhtp_t* htp, evthr_t* thread, void* arg);

template<class T>
void LServer::post(const char* path, void (T::*handler)(LReq&, LRes&) const) {
  _router.addRoute("POST", path, handler);
}

template<class T>
void LServer::get(const char* path, void (T::*handler)(LReq&, LRes&) const) {
  _router.addRoute("GET", path, handler);
}

template<class T>
void LServer::custom(
  const char* method,
  const char* path,
  void (T::*handler)(LReq&, LRes&) const
) {
  _router.addRoute(method, path, handler);
}

template<class T>
void LServer::intern(const char* path, void (T::*handler)(LReq&, LRes&) const) {
  _router.addRoute("INTERN", path, handler);
}

template<class T>
void LServer::setHandlerFor404(void (T::*handler)(LReq&, LRes&) const) {
  _router.add404Ctrllr(handler);
}

template<class T>
void LServer::setHandlerFor500(void (T::*handler)(LReq&, LRes&) const) {
  _router.add500Ctrllr(handler);
}

} // namespace loopy

#endif  // LIBRARY_SYS_LSERVER_H_
