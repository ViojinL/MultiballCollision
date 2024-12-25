### 项目简介  
这是一个基于c++的SDL2库的多球碰撞模拟的改版，参考项目为[marcotf-git](https://github.com/marcotf-git/cpp)的项目，本项目使用cmake构建，所用到的SDL的库均来自[SDL github链接](https://github.com/libsdl-org/SDL)  
### 关于作者  
作者的网名叫Viojin。欢迎访问我的个人博客[🔗](https://www.lymzzi.xyz)
### 构建环境  
windows 11，vscode，mingw，cmake，
### 编译指令  
``` bash
mkdir build
cd build  
cmake .. -G "MinGW Makefiles" 
cmake --build . --config Debug 
```
### 涉及物理原理
&nbsp;&nbsp;&nbsp;&nbsp;项目涉及到碰撞，我采用了二维空间中的完全弹性碰撞来处理，分解速度为一维空间上的弹性碰撞
### 模拟原型
&nbsp;&nbsp;&nbsp;&nbsp;我想模拟的是在传统CSMA/CD(Carrier Sense Multiple Access with Collision Detection)机制下数据包碰撞，将球抽象为数据包，用球的大小模拟数据大小及优先级，我们知道，在传统的以太网中，如果多个设备在同一信道上同时发送数据，可能会导致冲突。因此，我用两面墙模拟一个信道，当数据包球在这个信道中发生碰撞（冲突）时，触发重传机制，优先级小的被重新发送，来模拟退避算法（Backoff Algorithm），当数据包球到达底部，就说明数据已经完成传输，然后被销毁。
&nbsp;&nbsp;在现代的网络通信中，交换机和全双工通信的应用，避免了这种碰撞机制的出现。

