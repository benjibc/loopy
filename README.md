loopy
=====

Event based C++ HTTP server with MVC framework. Check main.cpp and app/controllers and app/views to see 

You will need clang or the latest GCC(4.9) because of this bug in the g++ compiler
* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47226

Why I am doing this project
----

Look at the following syntax:

``````````````````
//////////////////////////////////////////////////////////////////////////////
// example async handler for parallel db request 
void DefaultController::ParallelHello() {
  parallel(make_tuple(
    redis->get("visitor_count"),
    redis->get("dash_count")
  ),
  [this] (std::shared_ptr<std::tuple<LRedisData*, LRedisData*>> reply ) {
    std::cout << "inside user callback" << std::endl;
    std::string str = "two redis requests have finished ";
    str += "visitor count ";
    str += std::get<0>(*reply)->str;
    str += "dash count ";
    str += std::get<1>(*reply)->str;
    _res.send(L_OK, str);
  });
}
``````````````````
This is the example code for a controller.
* The syntax looks like node.js, and the callback patterns saves the day.
* You get the type safety and the performance of C++
* Concerned about callback hell? Check out the hierarchical controller routing example.


Get started
-----

``````````````````
sh init_repo.sh
make run
``````````````````

and check localhost:8000/hello, /complex/hello?id=123&username=foo etc.

For more example endpoints, check main.cpp, and navigate to the application specific code inside app/controllers and app/views

TODO:
* Fix all the current FIXME that is related to memory leaks, and move everything into unique pointers
* Clean up all the code!
* Fix all the docs for the header file!
* Move all the patterns from the javascript async libraries to the current implementation
* Make a plugin system for all the drivers (maybe modify npm a little)
* Use folly Arena to allocate all the smart pointers in order to reduce
  cost of memory allocation
* Follow doxygen and generate the doc
* Refactor all the code that generates the key inside the router
