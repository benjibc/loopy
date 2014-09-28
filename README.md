loopy
=====

Event based C++ HTTP server with MVC framework. Check main.cpp and app/controllers and app/views to see 

You will need clang or the latest GCC(4.9) because of this bug in the g++ compiler
* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47226

Get started
-----

``````````````````
sh init_repo.sh
make run
``````````````````

and check localhost:8000/hello, /complex/hello?id=123&username=foo etc.

For more example endpoints, check main.cpp, and navigate to the application specific code inside app/controllers and app/views

TODO:
* Fix the installation of hiredis library
* Use folly Arena to allocate all the smart pointers in order to reduce
  cost of memory allocation
* Follow doxygen and generate the doc
* Refactor all the code that generates the key inside the router
