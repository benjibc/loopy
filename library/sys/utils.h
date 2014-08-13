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
#ifndef LIBRARY_SYS_UTILS_H_
#define LIBRARY_SYS_UTILS_H_

#include <evhtp.h>

enum HTTP_STATUS_CODE {
  L_OK   = EVHTP_RES_200,
  L_300  = EVHTP_RES_300,
  L_400  = EVHTP_RES_400,
  L_NOT_FOUND = EVHTP_RES_NOTFOUND,
  L_SERVER_ERROR = EVHTP_RES_SERVERR,
  L_CONTINUE = EVHTP_RES_CONTINUE,
  L_FORBIDDEN = EVHTP_RES_FORBIDDEN,
  L_SWITCH_PROTO = EVHTP_RES_SWITCH_PROTO,
  L_MOVEDPERM = EVHTP_RES_MOVEDPERM,
  L_PROCESSING = EVHTP_RES_PROCESSING,
  L_URL_TOO_LONG = EVHTP_RES_URI_TOOLONG,
  L_CREATED = EVHTP_RES_CREATED,
  L_ACCEPTED = EVHTP_RES_ACCEPTED,
  L_NO_AUTH_INFO = EVHTP_RES_NAUTHINFO,
  L_NO_CONTENT = EVHTP_RES_NOCONTENT,
  L_RST_CONTENT = EVHTP_RES_RSTCONTENT,
  L_PARTIAL_CONTENT = EVHTP_RES_PARTIAL
// TODO: implement the rest
// case EVHTP_RES_MSTATUS:
// return "Multi-Status";
// case EVHTP_RES_IMUSED:
// return "IM Used";
// case EVHTP_RES_FOUND:
// return "Found";
// case EVHTP_RES_SEEOTHER:
// return "See Other";
// case EVHTP_RES_NOTMOD:
// return "Not Modified";
// case EVHTP_RES_USEPROXY:
// return "Use Proxy";
// case EVHTP_RES_SWITCHPROXY:
// return "Switch Proxy";
// case EVHTP_RES_TMPREDIR:
// return "Temporary Redirect";
// case EVHTP_RES_UNAUTH:
// return "Unauthorized";
// case EVHTP_RES_PAYREQ:
// return "Payment Required";
// case EVHTP_RES_METHNALLOWED:
// return "Not Allowed";
// case EVHTP_RES_NACCEPTABLE:
// return "Not Acceptable";
// case EVHTP_RES_PROXYAUTHREQ:
// return "Proxy Authentication Required";
// case EVHTP_RES_TIMEOUT:
// return "Request Timeout";
// case EVHTP_RES_CONFLICT:
// return "Conflict";
// case EVHTP_RES_GONE:
// return "Gone";
// case EVHTP_RES_LENREQ:
// return "Length Required";
// case EVHTP_RES_PRECONDFAIL:
// return "Precondition Failed";
// case EVHTP_RES_ENTOOLARGE:
// return "Entity Too Large";
// case EVHTP_RES_URITOOLARGE:
// return "Request-URI Too Long";
// case EVHTP_RES_UNSUPPORTED:
// return "Unsupported Media Type";
// case EVHTP_RES_RANGENOTSC:
// return "Requested Range Not Satisfiable";
// case EVHTP_RES_EXPECTFAIL:
// return "Expectation Failed";
// case EVHTP_RES_IAMATEAPOT:
// return "I'm a teapot";
// case EVHTP_RES_NOTIMPL:
// return "Not Implemented";
// case EVHTP_RES_BADGATEWAY:
// return "Bad Gateway";
// case EVHTP_RES_SERVUNAVAIL:
// return "Service Unavailable";
// case EVHTP_RES_GWTIMEOUT:
// return "Gateway Timeout";
// case EVHTP_RES_VERNSUPPORT:
// return "HTTP Version Not Supported";
// case EVHTP_RES_BWEXEED:
// return "Bandwidth Limit Exceeded";
};
const char* getMethodName(htp_method method);

#endif  // LIBRARY_SYS_UTILS_H_