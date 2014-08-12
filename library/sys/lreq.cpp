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
#include "./lreq.h"
#include <string>
#include <vector>

namespace loopy {

LReq::LReq(evhtp_request_t* request, ThreadLocal* threadLocal)
  :_request(request), _threadLocal(threadLocal)
{}

const char* LReq::path() const {
  if (_request) {
    return _request->uri->path->full;
  } else {
    return nullptr;
  }
}

const char* LReq::filename() const {
  if (_request) {
    return _request->uri->path->file;
  } else {
    return nullptr;
  }
}

htp_scheme LReq::scheme() const {
  return _request->uri->scheme;
}

LReq::stringKV& LReq::queryParams() {
  if (_queryParams.empty()) {
    _queryParams = evhtpKVsToMap(_request->uri->query);
  }
  return _queryParams;
}

LReq::stringKV& LReq::httpHeaders() {
  if (_httpHeaders.empty()) {
    _httpHeaders = evhtpKVsToMap(_request->headers_in);
  }
  return _httpHeaders;
}

LReq::stringKV LReq::evhtpKVsToMap(evhtp_kvs_t* kv) {
  LReq::stringKV map;

  // it is possible that there is no query parameter
  if (kv == nullptr) {
    return map;
  }

  evhtp_kv_s* head = TAILQ_FIRST(kv);

  while (head != NULL) {
    map[std::string(head->key, head->klen)] =
      std::string(head->val, head->vlen);

    head = TAILQ_NEXT(head, next);
  }
  return map;
}


void LReq::addToCallStack(std::string nextPath) {
  _requestStack.push_back(nextPath);
}

const std::vector<std::string>& LReq::callStack() const {
  return _requestStack;
}

} // namespace loopy
