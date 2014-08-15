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
#include "./lserver.h"
#include <evhtp.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <thread>

#include <iostream>

namespace loopy {

LServer::LServer(
  const char* address,
  uint16_t    port,
  uint16_t    num_threads,
  int         backlog
)
  : _evbase(event_base_new()),
    _htp(evhtp_new(_evbase, NULL)),
    _addr(address),
    _port(port),
    _num_threads(num_threads),
    _backlog(backlog)
{}

// TODO: make error reporting
void LServer::run() {
  void (*cb)(evhtp_request_s*, void*) = &processRequest;

  // register the callback that handles all the requests
  evhtp_set_gencb(_htp.get(), cb, this);

  // initialize the threads
  evhtp_use_threads(_htp.get(), initializeThread, _num_threads, NULL);

  // bind the sockets
  if (evhtp_bind_socket(_htp.get(), _addr.c_str(), _port, _backlog) < 0) {
    std::cerr << "Could not bind socket: " << strerror(errno) << std::endl;
    std::terminate();
  }

  event_base_loop(_evbase, 0);
}


LCtrlHandler LServer::getCtrlHandler(const char* method, const char* path) {
  std::string key = std::string(method) + "::" + path;
  return _router.getHandler(key);
}

LCtrlHandler LServer::getCtrlHandlerStrict(const char* method, const char* path) {
  std::string key = std::string(method) + "::" + path;
  return _router.getHandlerStrict(key);
}


// get the thread that is handling the current request
evthr_t* get_request_thr(evhtp_request_t * request) {
  evhtp_connection_t * htpconn;
  evthr_t * thread;

  htpconn = evhtp_request_get_connection(request);
  thread = htpconn->thread;

  return thread;
}

void processRequest(evhtp_request_t* request, void* arg) {
  struct sockaddr_in* sin;
  auto pServer = static_cast<LServer*>(arg);
  // evthr_t * thread;
  evhtp_connection_t * conn;
  char tmp[1024];

  // auto thread = get_request_thr(request);
  conn = evhtp_request_get_connection(request);

  // auto threadLocal = static_cast<ThreadLocal*>(evthr_get_aux(thread));

  sin = (struct sockaddr_in *)conn->saddr;

  evutil_inet_ntop(AF_INET, &sin->sin_addr, tmp, sizeof(tmp));

  // route the request to the appropriate handler
  evhtp_uri_t* URI = request->uri;
  auto method = request->method;
  std::string methodName = getMethodName(method);

  // init Req and Res object
  auto ctrlHandler = pServer->getCtrlHandler(methodName.c_str(), URI->path->full);

  evhtp_request_pause(request);

    LServer::serveRequest(ctrlHandler, request);


  evhtp_request_resume(request);
}

/**
 * Function call to initialize each of the thread
 */
void initializeThread(evhtp_t * htp, evthr_t * thread, void* arg) {

  ThreadShared* threadShared = static_cast<ThreadShared*>(arg);

  // populate the system related information required for the thread, and
  // initialize the ThreadLocal storage
  auto evbase = evthr_get_base(thread);
  SysThreadInfo sysInfo(thread, evbase);

  // TODO: memory leak when the thread is stopped. But when the thread is
  // stopped, server shouldve died anyways
  ThreadLocal* threadLocal = new ThreadLocal(threadShared, sysInfo);
  evthr_set_aux(thread, threadLocal);
}

LServer& LServer::getInstance(
  const char* address,
  uint16_t    port,
  uint16_t    num_threads,
  int         backlog
) {
  static LServer instance(address, port, num_threads, backlog);
  return instance;
}

unsigned LServer::getNumCPU() {
  return std::thread::hardware_concurrency();
}

void LServer::serveRequest(LCtrlHandler ctrlHandler, pReq request) {
  try {

    ctrllerFactoryFunc factory = std::get<0>(ctrlHandler);
    std::shared_ptr<LController> pCtrl((*factory)(request));
    LHandler pHandler = std::get<1>(ctrlHandler);

    // execute the request
    ((*pCtrl).*pHandler)();

    // put the output of res into the request object and finish off
    auto& res = pCtrl->res();
    if (res.isModified()) {

      evbuffer_add(request->buffer_out, res.getContent(), res.contentSize());

      evhtp_headers_add_header(
        request->headers_out,
        evhtp_header_new("Content-Type", "text/html", 0, 0)
      );
    }

    evhtp_send_reply(request, res.status());

  // caught some runtime error, probably invalid routes
  // TODO: have a separate exceptiont type
  } catch (const std::runtime_error & e) {

    // in case of a path exception, remove all the remaining data in the buffer
    // and send back all the data with regards to exception
    std::string message = e.what();
    evbuffer_add(request->buffer_out, message.c_str(), message.size());
    evhtp_send_reply(request, L_SERVER_ERROR);
  }
}


} // namespace loopy
