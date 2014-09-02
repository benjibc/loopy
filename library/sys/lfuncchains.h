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
#ifndef LIBRARY_SYS_LFUNCCHAIN_H__
#define LIBRARY_SYS_LFUNCCHAIN_H__

#include <evhtp.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <memory>
#include "./utils.h"

// LFuncChain will be responsible for holding multiple lambdas together. It
// will always make sure the return type of the first lambda matches the parameter
// type of the second lambda. Also, it will overload a paramter type to execute
// all the data on a certain function set. You will be able to chain the lambda
// using `then`
//
namespace loopy {

class LAsyncBase {
 public:
  LAsyncBase() = default;
  virtual ~LAsyncBase() = default;
};

/// main specialization
/// dont bother converting lambda to std::function, it doesnt work
class LAsync : public LAsyncBase {
 public:
  LAsync(LQuery* query);
 private:
  LambdaType initLambda;
};

} // namespace loopy

#endif  // LIBRARY_SYS_LFUNCCHAIN_H__
