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
#include "lres.h"
#include <iostream>

namespace loopy {

LRes::LRes(evhtp_request_t* request, ThreadLocal* threadLocal)
  :_subroutine(false),
   _request(request),
   _threadLocal(threadLocal),
   _templateParams(new TemplateParams(request->uri->path->full)),
   _code(200)
{}

void LRes::send(HTTP_STATUS_CODE code, std::string& content) {
  std::swap(_output, content);
  _code = code; 
};

void LRes::send(std::string& content) {
  send(L_OK, content);
};

const char* LRes::getContent() const {
  return _output.c_str();
}

int LRes::contentSize() const {
  return _output.size();
}

void LRes::status(HTTP_STATUS_CODE code) {
  _code = code; 
};

HTTP_STATUS_CODE LRes::status() const {
  return _code;
};

void LRes::render(std::string& filename) {
  render(L_OK, filename);
}

void LRes::render(const char* filenameStr) {
  std::string filename = filenameStr;
  render(L_OK, filename);
}

void LRes::render(HTTP_STATUS_CODE code, std::string& filename) {
  if (!_subroutine) {
    std::string filepath = "./app/views/" + filename;
    try {
      ctemplate::ExpandTemplate(
        filepath,
        ctemplate::DO_NOT_STRIP,
        _templateParams,
        &_output
      ); 
      _code = code;
    } catch (std::string& e) {
      // clear output, and store e as the output data
      _output.clear();
      _output = e;
      _code = L_SERVER_ERROR;
    }
  } else {
    _includeFilename = "./app/views/" + filename;
  }
}

LRes::TemplateParams* LRes::templateParams() {
  return _templateParams;
}

void LRes::subroutine(bool subroutine) {
  _subroutine = subroutine;
}

LRes::~LRes() {
  delete _templateParams;
};

void LRes::swapTemplateParams(LRes::TemplateParams* &dict) {
  std::swap(dict, _templateParams);
}

const std::string& LRes::getIncludedTemplateFilename() const {
  return _includeFilename;
}

}


