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
#ifndef LIBRARY_SYS_LREQ_H_
#define LIBRARY_SYS_LREQ_H_

#include <evhtp.h>
#include <map>
#include <vector>
#include <string>
#include "../threadlocal.h"

namespace loopy {

/**
 * Request class for Loopy. Contains all the necessary information related
 * to the current request
 */
class LReq {
 private:
  typedef std::map<std::string, std::string> stringKV;

 public:
  LReq(evhtp_request_t* request, ThreadLocal* threadLocal);

  // accessor for different properties
  const char* path() const;

  // filename for the path
  const char* filename() const;

  // ip address
  const char* IPAddr() const;

  // http scheme
  htp_scheme scheme() const;

  // lazily evaluated query parameters and body parameters
  stringKV& queryParams();
  stringKV& httpHeaders();
  stringKV& bodyParams();

  void addToCallStack(std::string nextPath);
  const std::vector<std::string>& callStack() const;

 private:
  evhtp_request_t*  _request;
  ThreadLocal*      _threadLocal;

  // these 2 parameters will not be evalauted when the request is received
  // only when the accessor for the class is called, the parameter will
  // be evaluated
  stringKV          _queryParams;
  stringKV          _bodyParams;
  stringKV          _httpHeaders;

  // for hierarchical calling
  std::string       _next;

  // helper function that turns string KV to map of key value pairs
  stringKV          evhtpKVsToMap(evhtp_kvs_t* kv);

  // function call trace
  std::vector<std::string> _requestStack;
};

} // namespace loopy

#endif  // LIBRARY_SYS_LREQ_H_
