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
#include "./lcontroller.h"
#include <evhtp.h>
#include <ctemplate/template.h>
#include <exception>
#include <string>
#include <vector>
#include <queue>
#include "./lserver.h"
#include "./lrouter.h"

namespace loopy {

LController::LController(pReq req)
    : _thread(getRequestThread(req)),
      _threadLocal(_thread ? static_cast<ThreadLocal *>(evthr_get_aux(_thread))
                           : nullptr),
      _req(req), _res(req, _threadLocal),
      _evbase(_thread ? evthr_get_base(_thread) : nullptr), _promise(nullptr) {}

void LController::next(const char *method, const char *next,
                       const char *templateName) {
  LServer &server = LServer::getInstance();
  auto ctrlHandler = server.getCtrlHandlerStrict(method, next);

  std::string route = std::string(method) + " " + next;

  if (invalidControlHandler(ctrlHandler)) {
    std::string reason =
        std::string("Invalid route:") + route + "\nRoute Stack:\n";

    // populate the routing stack so when an error occurs, the user will be
    // able to see it
    auto callStack = _req.callStack();
    for (auto it = callStack.rbegin(); it != callStack.rend(); it++) {
      reason += *it + "\n";
    }

    throw std::invalid_argument(reason);
  }

  // now we know the controller is valid, serve the request
  ctemplate::TemplateDictionary *tParams = _res.templateParams();
  _req.addToCallStack(route);

  ctemplate::TemplateDictionary *dict =
      tParams->AddIncludeDictionary(templateName);

  std::string templateFilename =
      LServer::serveSubRequest(ctrlHandler, _req.rawReq(), dict);

  dict->SetFilename(templateFilename);
}

bool LController::invalidControlHandler(LCtrlHandler ctrlHandler) {
  return (std::get<0>(ctrlHandler) == nullptr) ||
         (std::get<1>(ctrlHandler) == nullptr);
}

LRes &LController::res() { return _res; }

pReq LController::rawReq() { return _req.rawReq(); }

void LController::execPromise() { _promise->initTrigger(); }

bool LController::isAsync() const { return _promise.get() != nullptr; }

void free_dummy_request(evhtp_request_t *req) {
  if (req == nullptr) {
    return;
  }
  if (req->headers_in != nullptr) {
    delete req->headers_in;
  }
  if (req->headers_out != nullptr) {
    delete req->headers_out;
  }

  evbuffer_free(req->buffer_in);
  evbuffer_free(req->buffer_out);

  // free_dummy_conn(req->conn);
  event_base_free(req->htp->evbase);
  evhtp_free(req->htp);
  delete[] req->uri->path->full;
  delete[] req->uri->path->file;
  delete[] req->uri->path->path;
  delete req->uri->path;
  delete req->uri;
  delete req;
}

} // namespace loopy
