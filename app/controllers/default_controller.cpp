#include "./default_controller.h"
#include <ctemplate/template.h>
// #include <drivers/loopy-redis/loopy-redis.h>
#include <string>

namespace loopy {

DefaultController::DefaultController(pReq req)
  : LController(req),
    name("Benny"),
    redis(static_cast<LRedis*>(_threadLocal->getDriver("LRedis")))
{}

// a static function that you must implement. You will get a thread
// variable, and you can attach the database driver there
void DefaultController::initThread(evthr_t* thread) {
  auto* threadLocal = static_cast<ThreadLocal*>(evthr_get_aux(thread));
  threadLocal->attachDriver(new LRedis(thread, "0.0.0.0", 6379));
}

// handler that renders a file in the view, and return the result to the user
void DefaultController::Hello() {
  auto* tParams = _res.templateParams();

  // render the USER_INFO section. The example shows that an section can be
  // rendered only if you enable it explicitly
  auto curName = std::string(name);
  if (curName.size() != 0) {
    auto section_dict = tParams->AddSectionDictionary("USER_INFO");
    section_dict->SetValue("USERNAME", name);
  }

  // render the list section. The example shows that you can have repleated
  // sections
  for (int i = 0; i < 4; i++) {
    auto section_dict = tParams->AddSectionDictionary("LIST_EXAMPLE");
    section_dict->SetValue("LIST_KEY", "KEY");
    section_dict->SetValue("LIST_VALUE", "VALUE");
  }
  // render the footer section. The example shows a simple Key-Value pair
  tParams->SetValue("FOOTER", "This is the footer section");
  _res.render("default/example.tpl");
}

// default handler for file not found
void DefaultController::FileNotFound() {
  std::string notFound = "01234567890";
  _res.send(L_NOT_FOUND, notFound);
}

//////////////////////////////////////////////////////////////////////////////
// async handler that renders a master template which includes a subtemplate
// uses queryParam from the user. Request the endpoint in the following format
// /complex/hello?id=2&name=foobar
void DefaultController::AsyncHello() {
  async(redis->incr("visitor_count"), [this] (redisReply* reply) {
    std::string str = "redis has finished";
    _res.send(L_OK, str);
  });
}

// endpoint to check the number of visitors
void DefaultController::Dashboard() {
  async(redis->get("visitor_count"), [this] (redisReply* reply) {
    if (reply->type == REDIS_REPLY_NIL) {
      std::string str = "no one ha hit AsyncHello yet";
      _res.send(L_OK, str);
    } else {
      std::string str = "number of visitors ";
      str += reply->str;
      _res.send(L_OK, str);
    }
  });
}

///////////////////////////////////////////////////////////////////////////////
// handler that renders a master template which includes a subtemplate, and also
// uses queryParam from the user. Request the endpoint in the following format
// /complex/hello?id=2&name=foobar
void DefaultController::ComplexHello() {
  auto* tParams = _res.templateParams();

  auto& queryParams = _req.queryParams();
  std::string ID = queryParams["id"];

  tParams->SetValue("ID", ID);
  for (int i = 0; i < 3; i++) {
    next("GET", "/section/hello", "TEMPLATE_1");
  }
  next("INTERN", "/section/hello2", "TEMPLATE_2");

  // try and see what happens if you invoke the wrong path
  // next("INTERN", "/section2/hello", req, res);

  tParams->SetValue("CTRLLR", name);

  _res.render("default/master.tpl");
}

////////////////////////////////////////////////////////////////////////////
// handles a section of template. This handler is exposed to the public
void DefaultController::SubComplexHello() {

  auto* tParams = _res.templateParams();
  auto& queryParams = _req.queryParams();

  tParams->SetValue("USERNAME", queryParams["username"]);
  _res.render("default/section.tpl");
}

//////////////////////////////////////////////////////////////////
// handles another section of the template. This handler is not exposed to the
// public
void DefaultController::SubComplexHello2() {
  auto* tParams = _res.templateParams();

  tParams->SetValue("USERNAME", name);
  _res.render("default/section2.tpl");
}

}  // namespace loopy
