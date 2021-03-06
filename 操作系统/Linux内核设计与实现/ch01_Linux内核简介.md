# Linux 内核简介

- 内核被称为是管理者和操作系统核心

- 内核的组成

  - 负责响应中断的中断服务程序

  - 负责管理多个进程从而分享处理器时间的调度程序

  - 负责管理进程地址空间的内存管理程序

  - 网络、进程间通信等系统服务

- 内核独立于普通应用程序，一般处于系统态，拥有受保护的内存空间和访问硬件设备的所有权限。这种系统态和被保护起来的内存空间，统称为内核空间

- 应用程序在用户空间执行，他们只能看到允许他们使用的部分系统资源，且只能使用某些特定系统功能，不能直接访问硬件，不能访问内核划给别人的内存范围。

- 当内核运行的时候，系统以内核态进入内核空间执行，反之执行一个普通应用程序时，系统以用户态进入以用户空间执行

- 系统中运行的程序通过系统调用来与内核通信。当一个应用程序执行一条系统调用，我们说内核正在代替其运行。这种情况下：

  - 应用程序被称为：通过系统调用在内核空间运行

  - 内核被称为：运行于进程上下文中

- 应用程序、内核和硬件的关系
![img](https://api2.mubu.com/v3/document_image/2cfedc69-6ca1-46ea-a21f-bc57f01c6387-5127810.jpg)

- Unix 内核往往需要操作系统提供页机制（MMU）以管理内存。这种页机制可以加强对内存空间的保护，并保证每个进程都可以运行于不同的虚地址空间上。

- Linux内核和Unix之间存在显著的差异

  - Linux支持动态加载内核模块，虽然linux是但内核，但是支持动态安装和卸载部分内核代码

  - Linux支持对称多处理（SMP）机制，但是传统的 Unix 不支持这种机制。

  - Linux 内核可以抢占（Preemptive），与传统 Unix 变体不同，Linux具有允许在内核运行的任务优先执行的能力。大多数 Unix 内核不支持抢占。

  - Linux对线程的支持：内核并不区分线程和其他的一般进程，对于内核来说，所有的进程都一样，都只是一些共享资源而已。

- Linux版本号规则，比如：[2.6.26.1](http://2.6.26.1/) 其中 2 是主版本号，6 是从版本号，26是修订版本号，如果修订版本号是偶数，则证明这是一个稳定版本，1是稳定版本号