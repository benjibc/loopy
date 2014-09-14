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
#include <iostream>
#include "./lres.h"
#include "./lcontroller.h"

namespace loopy {

LRes::LRes(pReq request, const ThreadLocal* threadLocal, bool subroutine)
  : _code(L_OK),
    _subroutine(subroutine),
    _request(request),
    _threadLocal(threadLocal),
    _templateParams(new TemplateParams(request->uri->path->full))
{}

void LRes::send(HTTP_STATUS_CODE code, std::string& content) {
  std::swap(_output, content);
  _code = code; 
  if (this->isModified()) {

    evbuffer_add(_request->buffer_out, getContent(), contentSize());
    evhtp_headers_add_header(
      _request->headers_out,
      evhtp_header_new("Content-Type", "text/html", 0, 0)
    );
  }
  evhtp_send_reply(_request, status());
  evhtp_request_resume(_request);
  // request reaches end of life
  delete static_cast<LController*>(_request->cbarg);
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
    send(L_OK, _output);
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
  // only delete subtemplate that you own
  if (!_subroutine) {
    delete _templateParams;
  }
};

void LRes::swapTemplateParams(LRes::TemplateParams* &dict) {
  std::swap(dict, _templateParams);
}

const std::string& LRes::getIncludedTemplateFilename() const {
  return _includeFilename;
}

}
