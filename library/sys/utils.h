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
#include <iostream>
#include <unistd.h>
#include "../threadlocal.h"

#define DEBUG(msg) (std::cout << "file " << __FILE__ << " line " << __LINE__ << ":" << \
                  msg <<  std::endl);

namespace loopy {

typedef evhtp_request_t* pReq;

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
evthr_t* getRequestThread(evhtp_request_t * request);

// helper template to decide if the two types are the same
template <class T1, class T2> struct SameType {
  enum{value = false};
};
template <class T> struct SameType<T, T> {
  enum{value = true};
};

// helper function to make and free dummy connections
void free_dummy_conn(evhtp_connection_t* conn);
evhtp_connection_t * new_dummy_conn(evhtp_t* htp);

void dummyInitializeThread(evthr_t * thread, void* arg);

// using a c++11 implementation of integer_sequence
template <typename T, T... N>
struct integer_sequence
{
  typedef T value_type;
  static_assert(
    std::is_integral<T>::value,
    "std::integer_sequence can only be instantiated with an integral type" );

  static inline
  std::size_t size(
  ) {
    return (sizeof...(N));
  }
};

template <std::size_t... N>
using index_sequence = integer_sequence<std::size_t, N...>;

namespace integer_sequence_detail {

template <typename T, std::size_t ..._Extra>
struct repeat;

template <typename T, T ...N, std::size_t ..._Extra>
struct repeat<integer_sequence<T, N...>, _Extra...>
{
  typedef integer_sequence<T, N...,
    1 * sizeof...(N) + N...,
    2 * sizeof...(N) + N...,
    3 * sizeof...(N) + N...,
    4 * sizeof...(N) + N...,
    5 * sizeof...(N) + N...,
    6 * sizeof...(N) + N...,
    7 * sizeof...(N) + N...,
    _Extra...> type;
};

template <std::size_t N> struct parity;
template <std::size_t N> struct make:parity<N % 8>::template pmake<N> {};

template <> struct make<0> { typedef integer_sequence<std::size_t> type; };
template <> struct make<1> { typedef integer_sequence<std::size_t, 0> type; };
template <> struct make<2> { typedef integer_sequence<std::size_t, 0, 1> type; };
template <> struct make<3> { typedef integer_sequence<std::size_t, 0, 1, 2> type; };
template <> struct make<4> { typedef integer_sequence<std::size_t, 0, 1, 2, 3> type; };
template <> struct make<5> { typedef integer_sequence<std::size_t, 0, 1, 2, 3, 4> type; };
template <> struct make<6> { typedef integer_sequence<std::size_t, 0, 1, 2, 3, 4, 5> type; };
template <> struct make<7> { typedef integer_sequence<std::size_t, 0, 1, 2, 3, 4, 5, 6> type; };

template <> struct parity<0> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type> {}; };
template <> struct parity<1> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type, N - 1> {}; };
template <> struct parity<2> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type, N - 2, N - 1> {}; };
template <> struct parity<3> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type, N - 3, N - 2, N - 1> {}; };
template <> struct parity<4> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type, N - 4, N - 3, N - 2, N - 1> {}; };
template <> struct parity<5> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type, N - 5, N - 4, N - 3, N - 2, N - 1> {}; };
template <> struct parity<6> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type, N - 6, N - 5, N - 4, N - 3, N - 2, N - 1> {}; };
template <> struct parity<7> { template <std::size_t N> struct pmake:repeat<typename make<N / 8>::type, N - 7, N - 6, N - 5, N - 4, N - 3, N - 2, N - 1> {}; };

template <typename T, typename U>
struct convert
{
  template <typename>
  struct result;

  template <T ...N>
  struct result<integer_sequence<T, N...> >
  {
    typedef integer_sequence<U, N...> type;
  };
};

template <typename T>
struct convert<T, T>
{
  template <typename U>
  struct result
  {
    typedef U type;
  };
};

template <typename T, T N>
using make_integer_sequence_unchecked =
typename convert<std::size_t, T>::template result<typename make<N>::type>::type;

template <typename T, T N>
struct make_integer_sequence
{
  static_assert(std::is_integral<T>::value,
    "std::make_integer_sequence can only be instantiated with an integral type");
  static_assert(0 <= N,"std::make_integer_sequence input shall not be negative");

  typedef make_integer_sequence_unchecked<T, N> type;
};

} // namespace integer_sequence_detail


template <typename T, T N>
using make_integer_sequence = typename integer_sequence_detail::make_integer_sequence<T, N>::type;

template <std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template <typename... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;


// helper to print tuple
template<typename T>
void printTuple(T tuple) {
  auto& first = std::get<0>(tuple);
  auto& second = std::get<1>(tuple);
  std::cout << "=========== printing tuple ============" << std::endl;
  if (first == nullptr) {
    std::cout << "first is empty" << std::endl;
  } else {
    std::cout << "first data is " << first->str << std::endl;
  }

  if (second == nullptr) {
    std::cout << "second is empty" << std::endl;
  } else {
    std::cout << "second data is " << second->str << std::endl;
  }
  std::cout << "=========== printing tuple end =======" << std::endl;
}


}  // namespace loopy

#endif  // LIBRARY_SYS_UTILS_H_
