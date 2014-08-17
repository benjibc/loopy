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
#include <ctemplate/template.h>
#include <exception>
#include <string>
#include <vector>
#include "./lserver.h"
#include "./lrouter.h"

namespace loopy {

LController::LController(pReq req)
  : req_(req), res_(req)
{}
void LController::next(
  const char* method,
  const char* next,
  const char* templateName
) {
  LServer& server = LServer::getInstance();
  auto ctrlHandler = server.getCtrlHandlerStrict(method, next);

  std::string route = std::string(method) + " " + next;

  if (invalidControlHandler(ctrlHandler)) {
    std::string reason = std::string("Invalid route:")
                         + route
                         + "\nRoute Stack:\n";

    // populate the routing stack so when an error occurs, the user will be
    // able to see it
    auto callStack = req_.callStack();
    for (auto it = callStack.rbegin(); it != callStack.rend(); it++) {
      reason += *it + "\n";
    }

    throw std::invalid_argument(reason);
  }

  // now we know the controller is valid, serve the request
  ctemplate::TemplateDictionary* tParams = res_.templateParams();
  req_.addToCallStack(route);

  res_.subroutine(true);
  ctemplate::TemplateDictionary* dict = tParams->AddIncludeDictionary(templateName);

  res_.swapTemplateParams(dict);
  LServer::serveRequest(ctrlHandler, req_.rawReq());
  res_.swapTemplateParams(dict);

  res_.subroutine(false);
  std::string templateFilename = res_.getIncludedTemplateFilename();
  dict->SetFilename(templateFilename);
}

bool LController::invalidControlHandler(LCtrlHandler ctrlHandler) {
  return (std::get<0>(ctrlHandler) == nullptr)
         || (std::get<1>(ctrlHandler) == nullptr);
}

LRes& LController::res() {
  return res_;
}

pReq LController::rawReq() {
  return req_.rawReq();
}

void LController::initializeThread(evhtp_t* thread) const {
  return;
}

LAsyncChain& LController::callbacks() {
  return asyncs_;
}


} // namespace loopy
