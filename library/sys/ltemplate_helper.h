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
#ifndef LIBRARY_SYS_LTEMPLATE_HELPER_H_
#define LIBRARY_SYS_LTEMPLATE_HELPER_H_

namespace loopy {

template <size_t N, typename T> void initiateTriggerForPromiseTuple(T tuple) {
  std::get<N>(tuple)->initTrigger();
}

// does nothing. Placeholder for the expansion in the parameter to take place
template <typename T>
void call_triggers_impl(T args) {
  return;
}

template <typename T,
          size_t N,
          size_t... Is>
void call_triggers_impl(T args) {
  initiateTriggerForPromiseTuple<N, T>(args);
  call_triggers_impl<T, Is...>(args);
  return;
}

template <typename UserPromiseArgsTuple, size_t... Is>
void
call_triggers(UserPromiseArgsTuple args, index_sequence<Is...>) {
  call_triggers_impl<UserPromiseArgsTuple, Is...>(args);
  return;
}

}  // namespace loopy

#endif  // LIBRARY_SYS_LTEMPLATE_HELPER_H_
