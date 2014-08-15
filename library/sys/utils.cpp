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
#include "./utils.h"

namespace loopy {

const char* getMethodName(htp_method method) {
  switch (method) {
    case htp_method_GET:
      return "GET";
    case htp_method_HEAD:
      return "HEAD";
    case htp_method_POST:
      return "POST";
    case htp_method_PUT:
      return "PUT";
    case htp_method_DELETE:
      return "DELETE";
    case htp_method_MKCOL:
      return "MKCOL";
    case htp_method_COPY:
      return "COPY";
    case htp_method_MOVE:
      return "MOVE";
    case htp_method_OPTIONS:
      return "OPTIONS";
    case htp_method_PROPFIND:
      return "PROPFIND";
    case htp_method_PROPPATCH:
      return "PROPPATCH";
    case htp_method_LOCK:
      return "LOCK";
    case htp_method_UNLOCK:
      return "UNLOCK";
    case htp_method_TRACE:
      return "TRACE";
    case htp_method_CONNECT: /* RFC 2616 */
      return "CONNECT";
    case htp_method_PATCH: /* RFC 5789 */
      return "PATCH";
    case htp_method_UNKNOWN:
      return "UNKNOWN";
  }
  return "";
}

} // namespace loopy
