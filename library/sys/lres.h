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
#ifndef LIBRARY_SYS_LRES_H_
#define LIBRARY_SYS_LRES_H_

#include <ctemplate/template.h>
#include <evhtp.h>
#include <sstream>
#include <string>
#include "../threadlocal.h"
#include "./utils.h"

namespace loopy {

/**
 * Response class for Loopy
 */
class LRes{
 private:
  typedef ctemplate::TemplateDictionary TemplateParams;

 public:
  LRes(pReq request, const ThreadLocal* threadLocal);
  ~LRes();

  // once_off writing with a string content; suitable for all the HTTP
  // application with template view code
  void send(std::string& content);
  void send(HTTP_STATUS_CODE code, std::string& content);

  // getter and setter for status
  HTTP_STATUS_CODE status() const;
  void status(HTTP_STATUS_CODE code);

  const char* getContent() const;
  int contentSize() const;

  int isModified() { return !!_output.size(); }

  // renderer for the template
  void render(std::string& filename);
  void render(const char* filenameStr);
  void render(HTTP_STATUS_CODE code, std::string& filename);

  // get the template params
  TemplateParams* templateParams();
  void subroutine(bool subroutine);
  const std::string& getIncludedTemplateFilename() const;

  void swapTemplateParams(TemplateParams* &dict);

 private:
  std::string       _output;
  HTTP_STATUS_CODE  _code;

  // parameter used if res was rendering a subtemplate
  bool              _subroutine;    // if this res was passed on
  std::string       _includeFilename; // name for the file included

  // check if the string content was ever modified
  pReq              _request;
  const ThreadLocal*      _threadLocal;
  TemplateParams*   _templateParams;
};

} // namespace loopy

#endif  // LIBRARY_SYS_LRES_H_
