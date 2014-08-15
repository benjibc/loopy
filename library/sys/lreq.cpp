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

LReq::LReq(pReq request)
  :request_(request)
{}

const char* LReq::path() const {
  if (request_) {
    return request_->uri->path->full;
  } else {
    return nullptr;
  }
}

const char* LReq::filename() const {
  if (request_) {
    return request_->uri->path->file;
  } else {
    return nullptr;
  }
}

htp_scheme LReq::scheme() const {
  return request_->uri->scheme;
}

LReq::stringKV& LReq::queryParams() {
  if (query_params_.empty()) {
    query_params_= evhtpKVsToMap(request_->uri->query);
  }
  return query_params_;
}

LReq::stringKV& LReq::httpHeaders() {
  if (http_headers_.empty()) {
    http_headers_= evhtpKVsToMap(request_->headers_in);
  }
  return http_headers_;
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

pReq LReq::rawReq() {
  return request_;
}

void LReq::addToCallStack(std::string next_path) {
  request_stack_.push_back(next_path);
}

const std::vector<std::string>& LReq::callStack() const {
  return request_stack_;
}

} // namespace loopy
