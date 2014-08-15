#include "./default_controller.h"
#include <ctemplate/template.h>
#include <boost/coroutine/coroutine.hpp>
#include <string>

namespace loopy {

DefaultController::DefaultController(pReq req)
  :LController(req), name("Benny")
{}

// handler that renders a file in the view, and return the result to the user
void DefaultController::Hello() {
  auto* tParams = res_.templateParams();

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
  res_.render("default/example.tpl");
}

// default handler for file not found
void DefaultController::FileNotFound() {
  std::string notFound = "01234567890";
  res_.send(L_NOT_FOUND, notFound);
}

///////////////////////////////////////////////////////////////////////////////
// handler that renders a master template which includes a subtemplate, and also
// uses queryParam from the user. Request the endpoint in the following format
// /complex/hello?id=2&name=foobar
void DefaultController::ComplexHello() {
  auto* tParams = res_.templateParams();

  auto& queryParams = req_.queryParams();
  std::string ID = queryParams["id"];

  tParams->SetValue("ID", ID);
  for (int i = 0; i < 3; i++) {
    next("GET", "/section/hello", "TEMPLATE_1");
  }
  next("INTERN", "/section/hello2", "TEMPLATE_2");

  // try and see what happens if you invoke the wrong path
  // next("INTERN", "/section2/hello", req, res);

  tParams->SetValue("CTRLLR", name);

  res_.render("default/master.tpl");
}

////////////////////////////////////////////////////////////////////////////
// handles a section of template. This handler is exposed to the public
void DefaultController::SubComplexHello() {

  auto* tParams = res_.templateParams();
  auto& queryParams = req_.queryParams();

  tParams->SetValue("USERNAME", queryParams["username"]);
  res_.render("default/section.tpl");
}

//////////////////////////////////////////////////////////////////
// handles another section of the template. This handler is not exposed to the
// public
void DefaultController::SubComplexHello2() {
  auto* tParams = res_.templateParams();

  tParams->SetValue("USERNAME", name);
}

}  // namespace loopy
