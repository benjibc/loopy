loopy
=====

Event based C++ HTTP server with MVC framework. Check main.cpp and app/controllers and app/views to see 

You will need clang or the latest GCC(4.9) because of this bug in the g++ compiler
* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47226

TODO:
* Fix the installation of hiredis library
* Use folly Arena to allocate all the smart pointers in order to reduce
  cost of memory allocation
* Follow doxygen and generate the doc
* Refactor all the code that generates the key inside the router
