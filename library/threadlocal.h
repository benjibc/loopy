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
#ifndef LIBRARY_THREADLOCAL_H_
#define LIBRARY_THREADLOCAL_H_

#include <evhtp.h>
#include <map>
#include <string>
#include "./sys/threadlocalbase.h"
#include "./threadshared.h"


namespace loopy {

/**
 * System required thread related information
 */
struct SysThreadInfo {

  // Pointer to the individual thread
  evthr_t*  threadPtr;

  // this event base is thread specific; each thread has its own even base,
  // and if you want to register some non-blocking behavior, register to the
  // threadEventBase for each of the threads
  evbase_t* threadEventBase;

  SysThreadInfo(evthr_t* threadPtr, evbase_t* tEventBase)
    :threadPtr(threadPtr), threadEventBase(tEventBase)
  {}
};

/**
 * This class contains the thread local data for the server.
 */
class ThreadLocal : public ThreadLocalBase {
 public:
  ThreadLocal(ThreadShared* threadShared, SysThreadInfo sysInfo);

  // one simple example would be having a map of string with strings;
  // std::map<std::string, std::string> store;
  // modify the fields to anything that suits your application
  //
  // for example, if you want nonblocking mysql or redis connection, you
  // should specify these connections here
};

} // namespace loopy
#endif  // LIBRARY_THREADLOCAL_H_
