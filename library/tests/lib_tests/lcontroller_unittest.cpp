// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// test for lserver

#include <limits.h>
#include <evhtp.h>
#include "../../third_party/googletest/include/gtest/gtest.h"
#include "../../sys/lcontroller.h"
#include "../app/controllers/default_controller.h"

using namespace loopy;

// pass in an invalid controller, make sure invalid controller can tell
// that the controller is indeed invalid
TEST(ValidControllerTest, BothNullPtr) {
  LCtrlHandler ctrlHandler = std::make_tuple(
    (ctrllerFactoryFunc) nullptr,
    (LHandler)  nullptr
  );
  EXPECT_TRUE(LController::invalidControlHandler(ctrlHandler));
}

TEST(ValidControllerTest, CtrllrAsNullPtr) {
  LCtrlHandler ctrlHandler = std::make_tuple(
    (ctrllerFactoryFunc) nullptr,
    (LHandler) &DefaultController::Hello
  );
  EXPECT_TRUE(LController::invalidControlHandler(ctrlHandler));
}

TEST(ValidControllerTest, MethodAsNullPtr) {
  LCtrlHandler ctrlHandler = std::make_tuple(
    (ctrllerFactoryFunc) nullptr,
    (LHandler) &DefaultController::Hello
  );
  EXPECT_TRUE(LController::invalidControlHandler(ctrlHandler));
}

TEST(ValidControllerTest, ValidController) {
  auto factory = [] (pReq req) -> LController* {
    return new DefaultController(req);
  };
  LCtrlHandler ctrlHandler = std::make_tuple(
    (ctrllerFactoryFunc) factory,
    (LHandler) &DefaultController::Hello
  );
  EXPECT_FALSE(LController::invalidControlHandler(ctrlHandler));
}

// make sure when requesst is a nullptr, we can still handle the result properly
TEST(ControllerInit, ControllerInitializedWithDummyRequest) {
  auto factory = [] (pReq req) -> LController* {
    return new DefaultController(req);
  };

  auto request = new_dummy_request<DefaultController>(); 

  auto controller = factory(request);
  EXPECT_TRUE(controller);
  free_dummy_request(request);
  delete controller;
}

// make sure when requesst is a nullptr, we can still handle the result properly
TEST(ControllerInit, TestAsyncResponseWithDummyRequest) {
  auto factory = [] (pReq req) -> LController* {
    return new DefaultController(req);
  };

  auto request = new_dummy_request<DefaultController>(); 

  auto controller = factory(request);
  EXPECT_TRUE(controller);
  free_dummy_request(request);
  delete controller;
}
