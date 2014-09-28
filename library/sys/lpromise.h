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
#ifndef LIBRARY_SYS_LPROMISE_H_
#define LIBRARY_SYS_LPROMISE_H_

#include "library/sys/utils.h"

namespace loopy {

class LPromiseBase {

 public:
  typedef std::function<void(void*)> TriggerType;
  typedef std::function<void(void*)> CallbackType;

 public:
  explicit LPromiseBase(TriggerType trigger)
    : trigger_(trigger)
  {}

  virtual ~LPromiseBase() = default;

  virtual void initTrigger() = 0;

 protected:
  TriggerType trigger_;
  CallbackType callback_;
};

/// LPromise takes a trigger, that would take a callback, register with libevent
/// and make sure the callback is executed once the DB call finishes
template<typename DriverType>
class LPromise : public LPromiseBase {

 private:
  typedef typename DriverType::ReturnType DBReturnType;

 public:
  explicit LPromise(LPromiseBase::TriggerType trigger)
    : LPromiseBase(trigger)
  {}

  void initTrigger() {
    trigger_(static_cast<void*>(&callback_));
  }

  template<typename nextLambdaType>
  void then(nextLambdaType onEnd) {
    callback_ = std::function<void(void* data)>(
      [onEnd](void* data) {
        onEnd(static_cast<DBReturnType*>(data));
      }
    );
  }

  void execute() {
    callback_();
  }
};

}  // namespace loopy

#endif  // LIBRARY_SYS_LPROMISE_H_
