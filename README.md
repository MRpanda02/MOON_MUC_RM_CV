# MOON_MUC_RM_CV

> **声明**	
>
>  该工程代码由上一届视觉学长整理并注释，工程结构较整齐，将来没有必要不做大幅修改。
>
> 花费一整天的时间将工程里的缺失文件补全,编写CMakeList,使视觉代码可以在Linux和Windows下皆可运行。
>
> 该README仅阐述我在TX2和Windows系统运行视觉代码时遇到的问题和心得，让各位省去一些麻烦。

# 安装MindVision Industrial Camera

迈德威视SDK可以自行在官网下载安装。官网:[mindvision.com.cn](https://mindvision.com.cn/)

**注意**：

- 要在官网上下载对应的操作系统的SDK进行安装(并重启)。(Linux版本document文件夹内有详细的安装说明和常见错误)

# 开启摄像头

- Windows可以将MindVision工业摄像头识别为USB摄像头，即可以用opencv库中的`VideoCapture camera(1)`来读取到摄像头的数据，十分方便。在Windows系统上测试代码时可以以该方式读取摄像头。
- Linux上则无法用该方式打开摄像头，需要一些由OpenCV C++源代码封装的图像读取API。(在该工程的iamge/src文件夹下)，其使用方式在demo文件夹里有说明。

# CMake

该工程用了CMake来编译工程内的多文件。一下罗列了编写或修改CMakeLists.txt所必须了解的cmakelist语法

**CMAKE_MINIMUM_REQUIRED(VERSION X.X)**                       指明了对cmake的最高(低)版本的要求,一般写在cmakelist的第一行

**PROJECT(PROJECTNAME)**                                                               PROJECT名字

**INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)**             加入子文件到cmake工程里

**AUX_SOURCE_DIRECTORY(./ DIR_SRCS)**                                      把文件夹下的原文件赋给变量(第二个参数)

**ADD_EXECUTABLE(PROJECTNAME ${DIR_SRCS})**                      生成可执行文件

**TARGET_LINK_LIBRARIES(PROJECTNAME 静态库名称)**             加入静态库/动态库  只能在加入到cmake工程里的文件夹里的静态库(由文件夹里的CMakeLists.txt生成),可以理解为#include,但是只会从上面添加到工程的子文件中搜索

**ADD_LIBRARY(PROJECTNAME STATIC/SHARED ${DIR_LIB_SRCS})**        参数3为AUX_SOURCE_DIRECTORY赋值后的变量

**SET(LIBRABY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)**             把生成的库放到指定的路径下

**SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)**      把可执行的文件放到指定的路径下

 **ADD_DEFINATIONS(-std=c++11)**                                          			设置C++编译器版本

> https://www.bilibili.com/video/BV16V411k7eF?p=1
>
> b站的一个CMake教程，里面有一些错误，但是可以对CMake的作用有一个具体的认识，一些语法可以之后自己去搜，仓库里也附上了很详细的CMake实践教程pdf

# 关于视觉代码的CMake

下载到自己的windows版本的电脑想要运行需要稍作修改主文件夹下的CMakeLists.txt

- 第9~11行是我加的条件,经过在两个版本的测试可知,对于第12行的`find_package`在Linux版本,可自动搜索到opencv库，但在windows系统,无法自动找到,所以需要手动给`OpenCV_DIR`赋值,地址为OpenCV文件中的静态库,我电脑中地址为E:/moon/OpenCV-MinGW-Build/x64/mingw/lib(重要)
- 13~15行需手动添加OpenCV库到工程中作为子文件(重要)
- 24~31行为定义的cmakedefine,因为在大风车识别代码中include的message.h用到了termios.h,该文件不支持windows系统,因为其用于linux系统tx2和单片机通信时获得字长,所以在windows上调试时由于不需要通信,将WINDOWS变量设置ON,进行条件编译(同理，在Linux上编译时将30行改为false即可)

- 若后续需要更多的条件编译,可以在config/config.hpp.in按第一行的格式进行宏定义,在Cmakelist只需加一个option做选项操作即可
- 各子文件里的CMakeLists.txt暂时无需做修改。

# 编译步骤

- 新建build文件夹(名字随意)
- 进入build文件夹,Windows环境下输入第一次要输入`cmake -DCMAKE_C_COMPILER=/c/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=/c/msys64/mingw64/bin/g++.exe ..`,两个参数地址为gcc.exe和g++.exe的地址
- 看到Configuring done后输入`make`
- 完成后文件夹中会出现两个可运行文件,完活。



> 如果有找不到libopencv_core.lib等错误,将CMakeLists.txt里手动找的OpenCV静态库地址复制,最后的lib改为bin,加入到系统环境变量即可
