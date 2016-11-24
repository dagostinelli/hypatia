CONTRIBUTING
============

All contributors are required to sign the CLA before any pull requests or patches will be accepted.

To get started, <a href="https://www.clahub.com/agreements/dagostinelli/hypatia">sign the Contributor License Agreement</a>. 

Unit Tests
--------------------------------
Mathematical bug fixes should have a unit test with 1 or more test cases indicating
what the problem was and now ensuring that it is fixed.

Unit tests can be executed as follows
```
mkdir build
cd build
cmake ..
make && ctest
```

Building Packages and Installers
--------------------------------
There is a built in packager using CPack. To build a tar.gz file, do as
follows.

```
mkdir build
cd build
cmake ..
make && ctest && cpack -G "TGZ"
```

In addition, CPack comes with additional packagers.  See the CPack documentation for more information.
