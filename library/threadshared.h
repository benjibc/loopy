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
#ifndef LIBRARY_THREADSHARED_H_
#define LIBRARY_THREADSHARED_H_

#include <evhtp.h>
#include <memory>
#include <string>

namespace loopy {

// A variable that all threads have access to. Modify the definition of this
// class if you want to have shared data structure between all threads
//
// use this class more or less as a struct; add more member variables to
// the class as you like
//
class ThreadShared {
 public:
  // add fields as you like; the system does not assume any structure of the
  // data
  std::string name;
  std::string address;


  // system defined fields
  static ThreadShared& getInstance();


 private:
    // base event loop
    ThreadShared();
};
} // namespace loopy

#endif  // LIBRARY_THREADSHARED_H_
