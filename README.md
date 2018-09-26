# cppLog 
![c++11](https://img.shields.io/badge/c%2B%2B-11-green.svg)  
 
Log helper class written in modern C++  

```cpp  
cppLog::LogI("Hello", "World!");  
auto age = 29;  
cppLog::LogW("I'm", age, "years old.");  
auto b = true;  
cppLog::LogW("Yes, this is", b);  
cppLog::LogD("Goodbye!");  
```  
![img_short_example](https://github.com/lucadesogus/cppLog/blob/DEV_WK1839/img/example_00.png)  

### Tested Platforms  
  
- Windows  
- Unix (Linux,OSX)  
  
### Tested Compilers  
- MSVC 19.14.26428.1
- GCC 5.4.0
- CLang  
  
## How to use cppLog  
Include "cpplog.hpp" into your project and use it as shown in "example/main.cpp".  
  
## Build and run the example  
Cmake  >= 5.4

Unix:
```sh  
cd cppLog  
mkdir build  
cd build  
cmake ..
make
./bin/cpplog
```
Windows:
```sh
cd cppLog
mkdir build
cd build
cmake .. -G "NMake Makefiles"
nmake
.\bin\cpplog
```
