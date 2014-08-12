#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <evhtp.h>
#include <string>
#include "./library/sys/lserver.h"
#include "./library/threadshared.h"

// list of files to include
#include "./app/controllers/default_controller.h"

using loopy::LServer;
using loopy::ThreadShared;
using loopy::DefaultController;

int
main() {

  // do your initialization for the shared thread object here
  std::string str = "Example app";
  auto sharedObj = ThreadShared::getInstance();
  sharedObj.name = str;

  // now do your initialization for the thread server
  LServer& server = LServer::getInstance("127.0.0.1", 8000, 2);

  // default example. Request with /hello
  server.get("/hello", &DefaultController::Hello);

  // complex example. Request with /complex/hello?id=4&username=baz
  server.get("/complex/hello", &DefaultController::ComplexHello);

  // small example, but is also a section for complex hello
  server.get("/section/hello", &DefaultController::SubComplexHello);

  // a dedicated section for complex hello
  server.intern("/section/hello2", &DefaultController::SubComplexHello2);

  // default handler when there is no response to be found
  server.setHandlerFor404(&DefaultController::FileNotFound);

  // run the server!
  server.run();
  return 0;
}
