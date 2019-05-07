## WSGI

**Web服务器网关接口**（**Python Web Server Gateway Interface**，缩写为WSGI）是为Python语言定义的Web服务器和Web应用程序或框架之间的一种简单而通用的接口。

WSGI区分为两个部分：一为“服务器”或“网关”，另一为“应用程序”或“应用框架”。在处理一个WSGI请求时，服务器会为应用程序提供环境信息及一个回调函数（Callback Function）。当应用程序完成处理请求后，通过前述的回调函数，将结果回传给服务器。

例如Gunicorn就是服务器，它负责socket链接维护、各种服务端模型实现等偏底层的功能；Flask就是应用框架，它负责请求的构建、处理以及响应；两者通过WSGI协议进行通信，同时由于WSGI协议的存在，不同的服务器和应用框架可以自由组合，例如UWSGI + Flask 或者 Gunicorn + Django

