# 有趣的轮子

一些比较有趣的开源项目链接，放在这里备忘

## 目录

### 网络相关

1. [gost](https://github.com/ginuerzh/gost)
   1. GO 语言实现的安全网络隧道
   2. 文档地址：https://docs.ginuerzh.xyz/gost/getting-started/
   3. 使用实践：
      1. [Mr.2 + gost 实现代理转发](https://runsisi.com/2019-07-01/nat-proxy)
      2. [Goole Group 的讨论](https://groups.google.com/forum/?hl=zh-TW#!topic/fqlt/ZfP-7Q3BVUw)
      3. [安全隧道利器 gost](https://blog.fengcl.com/2017/06/09/go-simple-tunnel-gost-introduction/)
2. [goproxy](https://github.com/snail007/goproxy)
   1. Proxy 是高性能全功能的 http 代理、https 代理、socks5 代理、内网穿透、内网穿透 p2p、内网穿透代理、内网穿透反向代理、内网穿透服务器、Websocket 代理、TCP 代理、UDP 代理、DNS 代理、DNS 加密代理，代理 API 认证，全能跨平台代理服务器。
   2. [中文说明](https://github.com/snail007/goproxy/blob/master/README_ZH.md)
   3. [中文手册](https://snail007.github.io/goproxy/manual/zh/#/)

### 实用工具

1. [gosseract](https://github.com/otiai10/gosseract)
   1. golang 实现的一个 OCR 识别引擎
2. [otto](https://github.com/robertkrimen/otto)
   1. golang 实现的一个`javascript`的解析器

### 项目

1. [gochat](https://github.com/LockGit/gochat)
   1. gochat 为纯 go 实现的即时通讯系统，支持私信消息与房间广播消息，各层之间通过 rpc 通讯，支持水平扩展。使用 redis 作为消息存储与投递的载体，相对 kafka 操作起来更加方便快捷，所以十分轻量。各层之间基于 etcd 服务发现，在扩容部署时将会方便很多。由于 go 的交叉编译特性，编译后可以快速在各个平台上运行，gochat 架构及目录结构清晰，并且本项目还贴心的提供了 docker 一键构建所有环境依赖，安装起来十分便捷。
2. .