loopy
=====

Event based C++ HTTP server with MVC framework

You will need the latest GCC(4.9) because of this bug in the g++ compiler
* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47226

TODO:
* Fix the installation of hiredis library
* Use folly Arena to allocate all the smart pointers in order to reduce
  cost of memory allocation
