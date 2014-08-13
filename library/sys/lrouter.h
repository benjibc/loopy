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
#ifndef LIBRARY_SYS_LROUTER_H_
#define LIBRARY_SYS_LROUTER_H_

#include <unordered_map>
#include <tuple>
#include <typeinfo>
#include <string>
#include <typeindex>
#include <memory>
#include "./lcontroller.h"

namespace loopy {

// typedefs to make code easier to read

class LRouter{
 private:
  typedef std::unordered_map<std::string, LCtrlHandler> RoutingMap;

 public:
  LRouter();

  /// using the key, either returns the correct handler, or returns a
  /// handler registered for 404
  LCtrlHandler getHandler(std::string key);

  /// return the correct handler or return a handler that is initialized to
  /// nullptr on both the handler pointer and the class pointer
  LCtrlHandler getHandlerStrict(std::string key);

  template<class T>
  void add404Ctrllr(void (T::*handler)(LReq&, LRes&) const) {
    auto pController = getOrAddController(handler);
    _404CtrlHandler = std::make_tuple(pController, (LHandler) handler);
  }

  template<class T>
  void add500Ctrllr(void (T::*handler)(LReq&, LRes&) const) {
    auto pController = getOrAddController(handler);
    _500CtrlHandler = std::make_tuple(pController, (LHandler) handler);
  }

  template<class T>
  ctrllrPtr getOrAddController(void (T::*handler)(LReq&, LRes&) const) {

    if (handler == nullptr) {
      throw std::runtime_error("handler is a nullptr");
    }

    auto typeKey = std::type_index(typeid(T));
    auto ctrlIter = _controllers.find(typeKey);

    // if the controller does not exist in the map, initialize a controller
    // and store the pointer; else retrieve the controller pointer
    std::shared_ptr<LController> pController(nullptr);

    if (ctrlIter == _controllers.end()) {
      pController = std::shared_ptr<LController>(new T());
      _controllers[typeKey] = pController;

    } else { // key does exist
      pController = ctrlIter->second;
    }

    return pController;
  }

  template<class T>
  void addRoute(
    const char* method,
    const char* path,
    void (T::*handler)(LReq&, LRes&) const
  ) {
    auto pController = getOrAddController(handler);

    std::string key = method;
    key += "::";
    key += path;
    _routes[key] = std::make_tuple(pController, (LHandler) handler);
  }

 private:
  // typedefs for more readability

  RoutingMap _routes;

  // use of type_index; refer to
  // http://en.cppreference.com/w/cpp/types/type_index
  std::unordered_map<std::type_index, ctrllrPtr> _controllers;

  // special handlers
  LCtrlHandler  _404CtrlHandler;
  LCtrlHandler  _500CtrlHandler;
};

}  // namespace loopy

#endif  // LIBRARY_SYS_LROUTER_H_
