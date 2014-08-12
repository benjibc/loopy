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
#include "./lserver.h"
#include "./lrouter.h"

namespace loopy {

LController::LController() {}
void LController::next(
  const char* method,
  const char* next,
  const char* templateName,
  LReq& req,
  LRes& res
) const {
  LServer& server = LServer::getInstance();
  auto ctrlHandler = server.getCtrlHandlerStrict(method, next);

  std::string route = std::string(method) + " " + next;

  if (invalidControlHandler(ctrlHandler)) {
    std::string reason = std::string("Invalid route:")
                         + route
                         + "\nRoute Stack:\n";

    // populate the routing stack so when an error occurs, the user will be
    // able to see it
    auto callStack = req.callStack();
    for (auto it = callStack.rbegin(); it != callStack.rend(); it++) {
      reason += *it + "\n";
    }

    throw std::invalid_argument(reason);
  }

  // now we know the controller is valid, serve the request
  ctemplate::TemplateDictionary* tParams = res.templateParams();
  req.addToCallStack(route);

  res.subroutine(true);
  ctemplate::TemplateDictionary* dict = tParams->AddIncludeDictionary(templateName);

  res.swapTemplateParams(dict);
  LServer::serveRequest(ctrlHandler, req, res);
  res.swapTemplateParams(dict);

  res.subroutine(false);
  std::string templateFilename = res.getIncludedTemplateFilename();
  dict->SetFilename(templateFilename);
}

bool LController::invalidControlHandler(LCtrlHandler ctrlHandler) {
  return (std::get<0>(ctrlHandler) == nullptr)
         || (std::get<1>(ctrlHandler) == nullptr);
}

} // namespace loopy
