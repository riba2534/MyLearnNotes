# Ubuntu+Apache2配置腾讯云ssl证书实现https

## 前言

本文所用的服务器环境是 Ubuntu18.04 + Apache2 .通过部署 ssl 来达到默认使用https方式访问网站的目的。

> **超文本传输安全协议**（英语：**Hypertext Transfer Protocol Secure**，[缩写](https://zh.wikipedia.org/wiki/%E7%B8%AE%E5%AF%AB)：**HTTPS**，常称为**HTTP over TLS**，**HTTP over SSL**或**HTTP Secure**）是一种透过[计算机网络](https://zh.wikipedia.org/wiki/%E8%A8%88%E7%AE%97%E6%A9%9F%E7%B6%B2%E7%B5%A1)进行安全通信的[传输协议](https://zh.wikipedia.org/wiki/%E7%B6%B2%E8%B7%AF%E5%82%B3%E8%BC%B8%E5%8D%94%E5%AE%9A)。HTTPS经由[HTTP](https://zh.wikipedia.org/wiki/HTTP)进行通信，但利用[SSL/TLS](https://zh.wikipedia.org/wiki/%E4%BC%A0%E8%BE%93%E5%B1%82%E5%AE%89%E5%85%A8)来加密数据包。HTTPS开发的主要目的，是提供对[网站](https://zh.wikipedia.org/wiki/%E7%B6%B2%E7%AB%99)服务器的[身份认证](https://zh.wikipedia.org/wiki/%E8%BA%AB%E4%BB%BD%E9%AA%8C%E8%AF%81)，保护交换数据的隐私与[完整性](https://zh.wikipedia.org/wiki/%E5%AE%8C%E6%95%B4%E6%80%A7)。这个协议由[网景](https://zh.wikipedia.org/wiki/%E7%B6%B2%E6%99%AF)公司（Netscape）在1994年首次提出，随后扩展到[互联网](https://zh.wikipedia.org/wiki/%E7%B6%B2%E9%9A%9B%E7%B6%B2%E8%B7%AF)上。
>
> 历史上，HTTPS连接经常用于[万维网](https://zh.wikipedia.org/wiki/%E4%B8%87%E7%BB%B4%E7%BD%91)上的交易支付和企业信息系统中敏感信息的传输。在2000年代晚期和2010年代早期，HTTPS开始广泛使用于保护所有类型网站上的网页真实性，保护账户和保持用户通信，身份和网络浏览的私密性。
>
> --- 来自《维基百科》

其实可以解决很大部分的运营商劫持问题，比如别人打开你的网站，网页上面会出现一些广告之类的，所以需要配置 https 来防止这个问题。

## 正文

Ubuntu 下 LAMP 环境的搭建，就不细说了，主要讲一下如何配置证书。

### 申请证书

打开腾讯云[https://buy.cloud.tencent.com/ssl](https://buy.cloud.tencent.com/ssl)来获得腾讯云提供的免费证书：

![](https://i.loli.net/2018/12/28/5c25f27d232cf.png)

点击`域名型免费版`，然后进去填一下网址和邮箱即可申请到。

过了几分钟，就会受到一个邮件，是一个压缩包，里面有各种服务器的 ssl 证书文件，我们进入 Apache2 这个目录，然后发现里面有三个文件：

```shell
1_root_bundle.crt    # 证书文件
2_xxx.xxx.xxx.crt # 证书文件
3_xxx.xxx.xxx.key # 私钥文件
```

- `1_root_bundle.crt`包含一段代码`-----BEGIN CERTIFICATE-----`和`-----END CERTIFICATE-----`
- `2_xxx.xxx.xxx.crt`文件包括一段证书代码`-----BEGIN CERTIFICATE-----`和`-----END CERTIFICATE-----`
- `3_xxx.xxx.xxx.key`文件包括一段私钥代码`-----BEGIN RSA PRIVATE KEY-----`和`-----END RSA PRIVATE KEY-----`

这是这几个文件的作用。

### 配置证书

在这里，我假设你已经会配置基本的`/etc/apache2/sites-available/000-default.conf`这个文件来达到已经可以通过 http 的方式来访问你的站点。

在`/etc/apache2`这个目录下，有两个有关的目录`sites-available`和`sites-enabled`，我们进入`sites-enabled`目录下可以发现，里面有一个文件`000-default.conf`

```shell
lrwxrwxrwx 1 root root 35 Dec 28 15:24 000-default.conf -> ../sites-available/000-default.conf
```

实质上这个文件是`/etc/apache2/sites-available/000-default.conf`这个文件的软链接。

我们要配置另 ssl 证书，要依靠另一个文件，也就是`default-ssl.conf`，首先我们需要设置一个软链接，把这个文件链接到`sites-enabled`这个文件夹中:

```shell
ln -s /etc/apache2/sites-available/default-ssl.conf /etc/apache2/sites-enabled/000-default-ssl.conf
```

然后去修改这个文件`000-default-ssl.conf`，因为已经做了软链接，其实这时候修改`000-default-ssl.conf`或`default-ssl.conf`都一样。

这个文件没有做任何修改前长这样子（去除自带的注释之后）：

```xml
<IfModule mod_ssl.c>
	<VirtualHost _default_:443>
		ServerAdmin webmaster@localhost

		DocumentRoot /var/www/html

		ErrorLog ${APACHE_LOG_DIR}/error.log
		CustomLog ${APACHE_LOG_DIR}/access.log combined

		SSLEngine on

		SSLCertificateFile	/etc/ssl/certs/ssl-cert-snakeoil.pem
		SSLCertificateKeyFile /etc/ssl/private/ssl-cert-snakeoil.key
	
		<FilesMatch "\.(cgi|shtml|phtml|php)$">
				SSLOptions +StdEnvVars
		</FilesMatch>
		<Directory /usr/lib/cgi-bin>
				SSLOptions +StdEnvVars
		</Directory>

	</VirtualHost>
</IfModule>
```

然后把从腾讯云上面下载好的证书(3个文件)传到你自定义的目录中

然后我们需要修改一下，修改成这样:

```xml
<IfModule mod_ssl.c>
	<VirtualHost _default_:443>
		ServerAdmin 你的邮箱
        
		DocumentRoot /var/www/你的目录
	    ServerName 你的域名

		ErrorLog ${APACHE_LOG_DIR}/error.log
		CustomLog ${APACHE_LOG_DIR}/access.log combined

		SSLEngine on
		# 注意，需要添加这三行
		SSLCertificateFile 你自定义的路径/2_xxx.xxx.xxx.crt
    	SSLCertificateKeyFile 你自定义的路径/3_xxx.xxx.xxx.key
    	SSLCertificateChainFile 你自定义的路径/1_root_bundle.crt
	
		<FilesMatch "\.(cgi|shtml|phtml|php)$">
				SSLOptions +StdEnvVars
		</FilesMatch>
		<Directory /usr/lib/cgi-bin>
				SSLOptions +StdEnvVars
		</Directory>
	</VirtualHost>
</IfModule>
```

重要的三个参数的作用如下表：

| 配置文件参数            | 说明          |
| ----------------------- | ------------- |
| SSLEngine on            | 启用 SSL 功能 |
| SSLCertificateFile      | 证书文件      |
| SSLCertificateKeyFile   | 私钥文件      |
| SSLCertificateChainFile | 证书链文件    |

改好之后保存。

然后这时，我们加载一下 Apache2 的 SSL 模块：

```shell
sudo a2enmod ssl   #加载模块
sudo service apache2 restart # 重启服务
```

这时，在浏览器输入`https://你的域名`应该已经可以通过 https 的方式来访问网站了，这时浏览器那里应该也已经有了一个绿色的小锁。

**但是**，**但是**.....这还不够，因为我们如果不主动输入`https://`的话，直接输入域名，还是会直接跳转到 80 端口的普通的 http 方式访问，所以我们需要强制使用 https 来访问

### 强制使用https

我们只需要打开`/etc/apache2/sites-available/000-default.conf`这个文件，在你的` <VirtualHost*:80></VirtualHost>`这个标签内随便一个地方加上三行：

```
RewriteEngine on
RewriteCond   %{HTTPS} !=on
RewriteRule   ^(.*)  https://%{SERVER_NAME}$1 [L,R]
```

然后保存,然后启动 Apache2 的重定向：

```shell
 sudo a2enmod rewrite
```

然后再重启 Apache2，至此大功告成：

```shell
sudo service apache2 restart
```

然后，打开浏览器直接输入域名，就会自动跳转到 https 的方式。

### 参考链接：

- [证书安装指引](https://cloud.tencent.com/document/product/400/4143)
- [腾讯云 Ubuntu+Apache2配置https和重定向](https://blog.csdn.net/qq_36652619/article/details/78310779)
- [ubuntu下apache2.4怎么配置SSL证书](http://bbs.qcloud.com/thread-22552-1-1.html)
- [Ubuntu下Apache2配置https且强制http转为https](https://www.luochenzhimu.com/archives/448.html)