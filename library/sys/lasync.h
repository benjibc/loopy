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
#ifndef LIBRARY_SYS_LASYNC_H_
#define LIBRARY_SYS_LASYNC_H_

#include <evhtp.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <memory>
#include "./utils.h"

namespace loopy {

class LAsyncBase;

typedef std::shared_ptr<LAsyncBase> LAsyncBasePtr;

// dummy base class so I can have a pointer to it
class LAsyncBase {
 public:
  LAsyncBase()
    : nextAsync_(nullptr)
  {}


  virtual ~LAsyncBase() = default;
  virtual void operator()() = 0;


 private:
  LAsyncBasePtr nextAsync_;
};

/// different specialization
template <typename LambdaType>
class LAsync : public LAsyncBase {
 public:
  explicit LAsync(LambdaType cb)
    : callback_(cb)
  {}
  typedef LambdaType Lambda;

 public:
  /// execute the next callback on stack if there is any
  void operator() () {
    callback_();
  }

 private:
  /// libevhtp thread variable
  LambdaType    callback_;
};

/// Chain of Asyncs to call
class LAsyncChain {
 public:
  /// start with the first element of the async queue
  template<typename NextLambdaType>
  explicit LAsyncChain(NextLambdaType lambda) {

    auto ptr = LAsyncBasePtr(new LAsync<NextLambdaType>(lambda));
    callbacks_.push_back(ptr);
  }

  /// add another element in the async queue
  template<typename NextLambdaType>
  LAsyncChain& next(NextLambdaType lambda) {

    auto ptr = LAsyncBasePtr(new LAsync<NextLambdaType>(lambda));
    callbacks_.push_back(ptr);
    return *this;
  }

  bool operator()() {
    std::for_each(callbacks_.begin(), callbacks_.end(), [] (LAsyncBasePtr asc) {
      (*asc)();
    });
    return true;
  }

 private:
  std::vector<LAsyncBasePtr> callbacks_;
};

} // namespace loopy
#endif  // LIBRARY_SYS_LASYNC_H_
