## 重复include
#### 处理方法有两种
1. 使用ifndef #define #endif
2. 使用#pragma once

#### 两种方法的区别
方式一依赖于宏名字不能冲突，这不光可以保证同一个文件不会被包含多次，也能保证内容完全相同的两个文件不会被不小心同时包含。当然，缺点就是如果不同头文件的宏名不小心“撞车”，可能就会导致头文件明明存在，编译器却硬说找不到声明的状况。

方式二则由编译器提供保证：同一个文件不会被编译多次。注意这里所说的“同一个文件”是指物理上的一个文件，而不是指内容相同的两个文件。带来的好处是，你不必再费劲想个宏名了，当然也就不会出现宏名碰撞引发的奇怪问题。对应的缺点就是如果某个头文件有多份拷贝，本方法不能保证他们不被重复包含。当然，相比宏名碰撞引发的“找不到声明”的问题，重复包含更容易被发现并修正。
方式一由语言支持所以移植性好，方式二 可以避免名字冲突
#pragma once方式产生于#ifndef之后，因此很多人可能甚至没有听说过。目前看来#ifndef更受到推崇。因为#ifndef受语言天生的支持，不受编译器的任何限制；而#pragma once方式却不受一些较老版本的编译器支持，换言之，它的兼容性不够好。也许，再过几年等旧的编译器死绝了，这就不是什么问题了。
我还看到一种用法是把两者放在一起的：
#pragma once
#ifndef __SOMEFILE_H__
#define __SOMEFILE_H__
... ... // 一些声明语句
#endif
看起来似乎是想兼有两者的优点。不过只要使用了#ifndef就会有宏名冲突的危险，所以混用两种方法似乎不能带来更多的好处，倒是会让一些不熟悉的人感到困惑。


## 前向声明:

可以声明一个类而不定义它。这个声明，有时候被称为前向声明(forward declaration)。在声明之后，定义之前，类Screen是一个不完全类型(incompete type)，即已知Screen是一个类型，但不知道包含哪些成员。不完全类型只能以有限方式使用，不能定义该类型的对象，不完全类型只能用于定义指向该类型的指针及引用，或者用于声明(而不是定义)使用该类型作为形参类型或返回类型的函数。类的前向声明之适用于指针和引用的定义，如果是普通类类型就得使用include了。

	
```
class A;  
class B  
{  
  public:  
  A* m_a; //（不能A m_a）  
}  
  
  
#include "A.h"  
  
class B  
{  
  public:  
  A* m_a; //(或者A m_a)  
}  
```

> 前向声明好处是: 
减少不必要的#include会增加编译时间. 混乱随意的#include可能导致循环#include,可能出现编译错误.


## pimpl idiom
- pimpl idiom主要运用在库的接口设计中。如果团队人数达100，有一个维护基础库的小组。原则上除了接口升级，基础库的任何升级都不应该触发应用层模块的重编译。
- 将库文件中的接口放在接口h头文件中，其余头文件是库内头文件，实现文件分别包含接口头文件和库内头文件并实现，应用只需要包含接口h头文件即可。
- 这样，只要接口头文件定义不进行改变，库内其他发生改变，也不会影响应用的编译和使用。

原理图如下
![](http://mmbiz.qpic.cn/mmbiz/pldYwMfYJpjz6L19006FFhUjtyqykj3OscIOwprWjpFh6yicxoKMuUtMxcJibLaesAb5jj1hPcF3unkmtxXtgbrg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
改成
![](http://mmbiz.qpic.cn/mmbiz/pldYwMfYJpjz6L19006FFhUjtyqykj3OxeSMXiaCt8Fjr7bSlNnV5iahZyhVuy9LgoEvvv0trRwmhoAHAvVIWh2A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
