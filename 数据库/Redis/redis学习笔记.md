# redis学习笔记

主要记录一些redis使用方面的知识。

redis可视化客户端：`Redis Desktop Manager`

## 基本介绍

### Redis 简介

Redis 是完全开源免费的，遵守BSD协议，是一个高性能的key-value数据库。

Redis 与其他 key - value 缓存产品有以下三个特点：

- Redis支持数据的持久化，可以将内存中的数据保存在磁盘中，重启的时候可以再次加载进行使用。
- Redis不仅仅支持简单的key-value类型的数据，同时还提供list，set，zset，hash等数据结构的存储。
- Redis支持数据的备份，即master-slave模式的数据备份。

### Redis 优势

- 性能极高 – Redis能读的速度是110000次/s,写的速度是81000次/s 。
- 丰富的数据类型 – Redis支持二进制案例的 Strings, Lists, Hashes, Sets 及 Ordered Sets 数据类型操作。
- 原子 – Redis的所有操作都是原子性的，意思就是要么成功执行要么失败完全不执行。单个操作是原子性的。多个操作也支持事务，即原子性，通过MULTI和EXEC指令包起来。
- 丰富的特性 – Redis还支持 publish/subscribe, 通知, key 过期等等特性。

### Redis与其他key-value存储有什么不同？

- Redis有着更为复杂的数据结构并且提供对他们的原子性操作，这是一个不同于其他数据库的进化路径。Redis的数据类型都是基于基本数据结构的同时对程序员透明，无需进行额外的抽象。
- Redis运行在内存中但是可以持久化到磁盘，所以在对不同数据集进行高速读写时需要权衡内存，因为数据量不能大于硬件内存。在内存数据库方面的另一个优点是，相比在磁盘上相同的复杂的数据结构，在内存中操作起来非常简单，这样Redis可以做很多内部复杂性很强的事情。同时，在磁盘格式方面他们是紧凑的以追加的方式产生的，因为他们并不需要进行随机访问。

## 基础操作

- `help 命令名称`：获取命令帮助
  - 帮助从上到下依次是，命令格式、功能描述、出现版本、所属群组
- `help @组名`：获取当前组的所有命令

## redis数据类型

一共有 5 种数据类型，如下：

- string
- hash
- list
- set
- sorted_set

以下分别是redis的各个命令的基本操作。

### `string`

形式就是k-v结构

#### 基本操作：

- 添加，修改数据：`set key value`
- 添加，修改多个数据：`mset k1 v1 k2 v2 ...`
- 获取数据：`get key`
- 获取多个数据：`mget k1 k2 ...`
- 删除数据：`del key`
- 获取数据字符个数（字符串长度）：`strlrn(key)`
- 追加到原始信息后部（存在就追加，否则新建）：`append k v`

#### 扩展操作：

场景1：在大型应用中，需要对MySQL进行分库分表，这时候要确保主键唯一，但是MySQL本身无法保证，这时可以借助redis来保证。

设置数值数据增加指定范围的值：

```
incr key # 如果key是纯数字，值会加一
incrby key increment # 上面是值加一，这里可以指定加的数量
incrbyfloat key increment # 可以操作浮点数
```

设置数值数据减少指定范围的值：

```
decr key # 如果key是纯数字，值会减一
decrby key value
```

- increment 可以为正值或者负值

- string在redis默认为字符串，当遇到 `incr` 的命令后会转成数值型
- redis 中所有操作都是原子性的，采用单线程处理所有业务，命令是一个一个执行的，所以不用考虑并发带来的数据影响，故而可以解决分布式id的问题

场景2：数据需要时效性时：

设置数据具有指定的生命周期（过期时间）：

```
setex key seconds value # 设置一个seconds秒后过期的key
psetex key milliseconds value # 以毫秒为单位设置一个key
```

#### 注意事项

- string 存储的最大数据量为 512MB
- 数值计算最大范围是 `2^63-1`

#### 业务场景

1. 比如微博的粉丝量，浏览量，这些信息可以放在redis

2. 设计好一个key之后，里面可以存json,存的数据容易读

### `hash`

存储结构为：一个key里面有很多个`k-v`数据

#### 基本操作:

```
# 添加/修改数据
hset key field value

# 获取数据
hget key field
hgetall key

# 删除数据
hdel key field1

# 添加/修改多个数据
hmset key k1 v1 k2 v2 ...

# 获取多个数据
hmget key k1 k2 ...

# 获取哈希表中字段数量
hlen key

# 获取哈希表中一个键是否存在
hexists key field
```

#### 扩展操作:

获取哈希表中所有的键或者值

```
hkeys key
hvals key
```

设置指定字段的数值数据增加指定范围的值

```
hincrby key field incrment
hincrbyfloat key field incrment
```

操作之前进行判定，如果field有值就什么都不做，否则做：

```
hsetnx key field value
```

#### 注意事项：

1. hash类型下的数据只能存string，不允许存其他
2. 每一个哈希表最大可以存储 `2^32-1`个键值对
3. hgetall 可能数据量很多，最好不要全部取

#### 业务场景：

- 电商购物车：
  - key为用户id，field为商品id，value为数量
  - key为用户id，field为 `商品id:数量`,value为json格式的数据，为商品信息
- 用 json 还是用 hash
  - json 读方便
  - hash 写方便

### `list`

一个key对应一组数据，顺序存储。

#### 基本操作

```
# 添加/修改数据
lpush key v1 v2 v3 ...  # 列表左边添加数据
rpush key v2 v2 v3 ...  # 列表右边添加数据

# 获取数据
lrange key st ed  # 0 -1 可以获取所有数据
lindex key index  # 获取指定下标
llen key          # 获取列表中元素数量

# 获取并移除数据，左边或右边
lpop key
rpop key

# 移除指定数据
lrem key count value # 从列表中删除count个value
```

#### 扩展操作

从多个列表中取数据，如果在timeout时间内取到了就返回且删除左或右元素，否则返回nil

```
blpop k1 k2 k3 ... timeout
brpop k1 k2 k3 ... timeout
```

使用场景可以理解为一个任务队列，有多个队列，如果有任务就取出来执行。

如果有需要顺序的存储可以用，比如朋友圈点赞

#### 注意事项

- list中的数据都是string，数据总量最大为 `2^63-1`
- list有索引的概念，基本就是栈和队列的模型
- list可以对数据进行分页，通常第一页的数据来源于list，第2页及以后通过数据库的形式加载

#### 业务场景

- 社交网站中，关注列表需要后面关注的先显示
- 新闻、资讯类网站显示最近的新闻
- 企业运营的数据，如何保证多台服务器操作日志统一顺序输出，全部塞进一个列表

### `set`

#### 基本操作

Redis的Set是string类型的无序集合。集合成员是唯一的，这就意味着集合中不能出现重复的数据。

Redis 中 集合是通过哈希表实现的，所以添加，删除，查找的复杂度都是O(1)。

集合中最大的成员数为 232 - 1 (4294967295, 每个集合可存储40多亿个成员)。

```
# 添加数据
sadd key member1 [member2]
# 获取数据
smembers key
# 删除数据
srem key member1 [member2]
# 获取集合数据总量
scard key
# 判断集合中是否包含指定数据
sismember key member
```

#### 扩展操作

```
# 随机获取集合中指定数量的数据
srandmember key [count]

# 随机获取集合中的某个数据并将改数据移出集合
spop key

# 求两个集合的交、并、差集
sinter key1 [key2]
sunion key1 [key2]
sdiff key1 [key2]

# 求两个集合的交、并、差集并存储到指定集合中
sinterstore destination key [key ...]
sunionstore destination key [key ...]
sdiffstore destination key [key ...]

# 将指定数据从原始集合中移动到目标集合中
smove source destination member
```

#### 注意事项

- set 类型中数据只存一份

### `sorted_set`

#### 基本操作

```
# 将一个或多个 member 元素及其 score 值加入到有序集 key 当中。
zadd key score member [[score member] [score member] ...]

# 获取全部数据
zrange key st ed [withscores] # 升序
zrevrange key start stop [WITHSCORES] # 降序

# 删除数据
zrem key member [member...]

# 返回有序集 key 中，所有 score 值介于 min 和 max 之间(包括等于 min 或 max )的成员。有序集成员按 score 值递增(从小到大)次序排列。
zrangebyscore key min max [withscore] [limit]
# 返回有序集 key 中， score 值介于 max 和 min 之间(默认包括等于 max 或 min )的所有的成员。有序集成员按 score 值递减(从大到小)的次序排列。
zrevrangebyscore key max min [withscore]   # 逆序排列所有成员

# 条件删除数据
zremrangebyrank key st ed
zremrangebyscore key min max

# 获取集合数据总量
zcard key
zcount key min max

# 集合交并操作
zinterstore destination numkeys key [key...]  # numkeys代表数量，就是几个集合
zunoinstore destination numkeys key [key...]
```

#### 扩展操作

```
# 获取数据对应的索引（排名）
zrank key number  # 正序
zrevrank key number # 逆序索引

# score 值获取与修改
zscore key number # 获取number的分数
zincrby key increment member # 为有序集 key 的成员 member 的 score 值加上增量 increment 。
```

#### 注意事项

- socre保存的是64位
- score保存的数据可以是一个 double 值，但是可能会丢失精度
- `sorted_set` 底层还是基于 set 的，因此数据不能重复，如果重复添加相同数据，score会被覆盖

使用 `time` 可以知道当前时间戳.

## 通用指令

### `key`通用操作

```
# 删除指定 key
del key

# 获取key是否存在
exists key

# 获取key的类型
type key
```

#### 扩展操作

```
# 为key指定有效期
expire key seconds       # 多少秒后过期
pexpire key milliseconds # 多少毫秒后过期
expireat key timestamp   # 将key的过期时间设置为timestamp所代表的的秒数的时间戳
pexpireat key milliseconds-timestamp # 将key的过期时间设置为timestamp所代表的的毫秒数的时间戳

# 获取key的有效时间
ttl key
pttl key

# 把key从时效性转换为永久性
persist key

# 为key改名
rename key newkey  # 直接改名
renamenx key newkey # 改名前判断新的key是不是已经存在了

# 对所有key排序
sort key # Sort the elements in a list, set or sorted set
```

#### 查询模式

```
# 查询key
keys pattern
```

查询模式匹配规则：

- `*`匹配任意数量的符号
- `?`匹配一个任意符号
- `[]`匹配一组中的任意指定符号

例子：

```
keys * # 查询所有
keys it* # 查询所有it开头
keys *ab # 查询所有以ab结尾
keys ??ab # 前面两个字符任意，后面以ab结尾
keys user:? # 查询所有以user:开头，最后一个字符任意
keys u[st]er:1 # 查询所有以u开头，以er:1记为，中间包含一个字母，s或t
```

### 数据库通用操作

- redis 为每个服务提供有16个数据库，编号从0到15
- 每个数据库之间的数据相互独立
- redis 默认在第0号数据库，可以通过切换来改变数据库

#### db基本操作

```
# 切换数据库
select index

# 其他操作
quit
ping
echo message

# 数据移动
move key db # 把当前数据库中的key移动到另一个数据库

# 数据清除
dbsize # 返回目前数据库中key的数量
flushdb # 删除当前数据库
flushall # 删除所有数据库
```

## Linux下的redis配置命令

`redis-server`的默认启动端口是 `6379` ,如果启动时想指定别的端口，可以：

```
redis-server --port 6380
redis-cli -p 6380

redis-server conf文件地址
```

在 Linux 中，redis 的配置文件保存在 `/etc/redis/redis.conf` 中。

Linux 中杀掉指定 `PID` 的进程：

```shell
kill -s 9 PID
```

由于其中注释信息比较多，查看配置时可以使用：

```shell
cat redis.conf| grep -v "#" | grep -v "^$"
```

将所有的以`#`号开头的行，和所有的空行全部去除掉，`grep -v` 加关键字可以去指定关键字开头的行。

### 重要配置

- 设置服务器以守护进程方式运行

  ```
  daemonize yes|no
  ```

- 绑定主机地址

  ```
  bind 127.0.0.1
  ```

- 设置服务器端口号

  ```
  port 6379
  ```

- 设置数据库数量

  ```
  database 16
  ```

- 设置服务器以指定日志记录级别

  ```
  loglevel debug|verbose|notice|warning # 日志级别开发期为verbose，生产环境建议 notice
  ```

- 日志记录文件名

  ```
  logfile 端口号.log
  ```

- 设置同一时间最大客户连接数，默认无限制。客户端连接数达到上限，redis就会关闭新链接

  ```
  maxclients 0
  ```

- 客户端闲置等待最大时常，达到最大值后关闭连接

  ```
  timeout 300
  ```

- **多服务器配置**：导入并加载指定配置文件信息，用于快速创建redis公共配置较多的redis实例配置文件

  ```
  include /path/redis.conf
  ```

  

### `redis.conf`详解：

```
# redis.conf
# Redis configuration file example.
# ./redis-server /path/to/redis.conf

################################## INCLUDES ###################################
#这在你有标准配置模板但是每个redis服务器又需要个性设置的时候很有用。
# include /path/to/local.conf
# include /path/to/other.conf

######################### 网络 #########################
# redis 监听的端口号
port 6379

# 此参数确定了 TCP 连接中已完成队列（完成三次握手之后）的长度， 
# 当然此值必须不大于 Linux 系统定义的 /proc/sys/net/core/somaxconn 值，默认是 511，而 Linux 的默认参数值是 128。
# 当系统并发量大并且客户端速度缓慢的时候，可以将这二个参数一起参考设定。
# 该内核参数默认值一般是 128，对于负载很大的服务程序来说大大的不够。一般会将它修改为2048或者更大。
# 在/etc/sysctl.conf中添加:net.core.somaxconn = 2048，然后在终端中执行 sysctl -p。
tcp-backlog 511

# 指定 redis 只接收来自于该 IP 地址的请求，如果不进行设置，那么将处理所有请求
bind 127.0.0.1

# 配置 unix socket 来让 redis 支持监听本地连接。
# unixsocket /var/run/redis/redis.sock

# 配置 unix socket 使用文件的权限
# unixsocketperm 700

# 此参数为设置客户端空闲超过 timeout，服务端会断开连接，为 0 则服务端不会主动断开连接，不能小于 0。
timeout 0

# 客户端空闲多少秒后关闭连接（0为不关闭）timeout 0
# tcp-keepalive 设置。如果非零，则设置 SO_KEEPALIVE 选项来向空闲连接的客户端发送 ACK，用途如下：
# 1）能够检测无响应的对端
# 2）让该连接中间的网络设备知道这个连接还存活
# 在 Linux 上，这个指定的值（单位秒）就是发送 ACK 的时间间隔。
# 注意：要关闭这个连接需要两倍的这个时间值。
# 在其他内核上这个时间间隔由内核配置决定
# 从 redis3.2.1 开始默认值为 300 秒
tcp-keepalive 0

################################ GENERAL #####################################

# 是否在后台执行，yes：后台运行；no：不是后台运行（老版本默认）
daemonize yes

# 3.2 版本里的参数，是否开启保护模式，默认开启。
# 如果没有设置 bind 项的 ip 和 redis 密码的话，服务将只允许本地访问
protected-mode yes

# redis的进程文件
pidfile /var/run/redis/redis-server.pid

# 指定了服务端日志的级别。级别包括：debug（很多信息，方便开发、测试），
# verbose（许多有用的信息，但是没有debug级别信息多），notice（适当的日志级别，适合生产环境），warn（只有非常重要的信息）
loglevel notice

# 指定了记录日志的文件。空字符串的话，日志会打印到标准输出设备。后台运行的 redis 标准输出是 /dev/null。
logfile /var/log/redis/redis-server.log

# 是否打开记录 syslog 功能
# syslog-enabled no

# syslog的标识符。
# syslog-ident redis

# 日志的来源、设备
# syslog-facility local0

# 数据库的数量，默认使用的数据库是 DB 0
databases 16

################################ SNAPSHOTTING ################################
# 快照配置
# 注释掉“save”这一行配置项就可以让保存数据库功能失效
# 设置 sedis 进行数据库镜像的频率。
# 900秒（15分钟）内至少1个key值改变（则进行数据库保存--持久化） 
# 300秒（5分钟）内至少10个key值改变（则进行数据库保存--持久化） 
# 60秒（1分钟）内至少10000个key值改变（则进行数据库保存--持久化）
save 900 1
save 300 10
save 60 10000

# 当 RDB 持久化出现错误后，是否依然进行继续进行工作
# yes：不能进行工作，no：可以继续进行工作，可以通过 info 中的 rdb_last_bgsave_status 了解 RDB 持久化是否有错误
stop-writes-on-bgsave-error yes

# 使用压缩 rdb 文件，rdb 文件压缩使用 LZF 压缩算法，yes：压缩，但是需要一些 cpu 的消耗。no：不压缩，需要更多的磁盘空间
rdbcompression yes

# 是否校验 rdb 文件。从 rdb 格式的第五个版本开始，在 rdb 文件的末尾会带上 CRC64 的校验和。
# 这跟有利于文件的容错性，但是在保存 rdb 文件的时候，会有大概 10% 的性能损耗，所以如果你追求高性能，可以关闭该配置。
rdbchecksum yes

# rdb文件的名称
dbfilename dump.rdb

# 数据目录，数据库的写入会在这个目录。rdb、aof 文件也会写在这个目录
dir /var/lib/redis

################################# REPLICATION #################################
# 主从同步配置。
# 1) redis 主从同步是异步的，但是可以配置在没有指定 slave 连接的情况下使 master 停止写入数据。
# 2) 连接中断一定时间内，slave 可以执行部分数据重新同步。
# 3) 同步是自动的，slave 可以自动重连且同步数据。
# slaveof <masterip> <masterport>

# 如果 master 设置了 requirepass，那么 slave 要连上 master，需要有 master 的密码才行。
# masterauth 就是用来配置 master 的密码，这样可以在连上 master 后进行认证。
# masterauth <master-password>

# 当从库同主机失去连接或者复制正在进行，从机库有两种运行方式：
# 1) 如果 slave-serve-stale-data 设置为 yes （默认设置），从库会继续响应客户端的请求。
# 2) 如果 slave-serve-stale-data 设置为 no，除去 INFO 和 SLAVOF 命令之外的任何请求都会返回一个错误 SYNC with master in progress。
slave-serve-stale-data yes

# 作为从服务器，默认情况下是只读的（yes），可以修改成 NO，用于写（不建议）。
# 注意：只读的 slave 不是为了暴露给互联网上不可信的客户端而设计的。它只是一个防止实例误用的保护层。
# 一个只读的 slave 支持所有的管理命令比如 config,debug 等。
# 为了限制你可以用 'rename-command' 来隐藏所有的管理和危险命令来增强只读 slave 的安全性。
slave-read-only yes

# 是否使用 socket 方式复制数据。目前 redis 复制提供两种方式，disk 和 socket。
# 如果新的 slave 连上来或者重连的 slave 无法部分同步，就会执行全量同步，master 会生成 rdb 文件。
# 有 2 种方式：disk 方式是 master 创建一个新的进程把 rdb 文件保存到磁盘，再把磁盘上的 rdb 文件传递给 slave。
# socket 是 master 创建一个新的进程，直接把 rdb 文件以 socket 的方式发给 slave。
# disk 方式的时候，当一个 rdb 保存的过程中，多个 slave 都能共享这个 rdb 文件。socket 的方式就的一个个 slave 顺序复制。
# 在磁盘速度缓慢，网速快的情况下推荐用 socket 方式。
repl-diskless-sync no

# diskless 复制的延迟时间，防止设置为 0。一旦复制开始，节点不会再接收新 slave 的复制请求直到下一个 rdb 传输。
# 所以最好等待一段时间，等更多的 slave 连上来。
repl-diskless-sync-delay 5

# slave 根据指定的时间间隔向服务器发送 ping 请求。时间间隔可以通过 repl_ping_slave_period 来设置，默认 10 秒。
# repl-ping-slave-period 10

# 同步的超时时间
# 1）slave 在与 master SYNC 期间有大量数据传输，造成超时
# 2）在 slave 角度，master 超时，包括数据、ping等
# 3）在 master 角度，slave 超时，当 master 发送 REPLCONF ACK pings
# 确保这个值大于指定的 repl-ping-slave-period，否则在主从间流量不高时每次都会检测到超时
# repl-timeout 60

# 是否禁止复制 tcp 链接的 tcp nodelay 参数，可传递 yes 或者 no。默认是 no，即使用 tcp nodelay。
# 如果 master 设置了 yes 来禁止 tcp nodelay 设置，在把数据复制给 slave 的时候，会减少包的数量和更小的网络带宽。
# 但是这也可能带来数据的延迟。默认我们推荐更小的延迟，但是在数据量传输很大的场景下，建议选择 yes。
repl-disable-tcp-nodelay no

# 复制缓冲区大小，这是一个环形复制缓冲区，用来保存最新复制的命令。
# 这样在 slave 离线的时候，不需要完全复制 master 的数据，
# 如果可以执行部分同步，只需要把缓冲区的部分数据复制给 slave，就能恢复正常复制状态。
# 缓冲区的大小越大，slave 离线的时间可以更长，复制缓冲区只有在有 slave 连接的时候才分配内存。
# 没有 slave 的一段时间，内存会被释放出来，默认 1m。
# repl-backlog-size 5mb

# master 没有 slave 一段时间会释放复制缓冲区的内存，repl-backlog-ttl 用来设置该时间长度。单位为秒。
# repl-backlog-ttl 3600

# 当 master 不可用，Sentinel 会根据 slave 的优先级选举一个 master。最低的优先级的 slave，当选 master。而配置成 0，永远不会被选举。
slave-priority 100

# redis 提供了可以让 master 停止写入的方式，如果配置了 min-slaves-to-write，健康的 slave 的个数小于 N，mater 就禁止写入。
# master 最少得有多少个健康的 slave 存活才能执行写命令。
# 这个配置虽然不能保证 N 个 slave 都一定能接收到 master 的写操作，
# 但是能避免没有足够健康的 slave 的时候，master 不能写入来避免数据丢失。设置为 0 是关闭该功能。
# min-slaves-to-write 3

# 延迟小于 min-slaves-max-lag 秒的 slave 才认为是健康的 slave。
# min-slaves-max-lag 10

# 设置 1 或另一个设置为 0 禁用这个特性。
# Setting one or the other to 0 disables the feature.
# By default min-slaves-to-write is set to 0 (feature disabled) and
# min-slaves-max-lag is set to 10.

################################## SECURITY ###################################
# requirepass 配置可以让用户使用 AUTH 命令来认证密码，才能使用其他命令。
# 这让 redis 可以使用在不受信任的网络中。为了保持向后的兼容性，可以注释该命令，因为大部分用户也不需要认证。
# 使用 requirepass 的时候需要注意，因为 redis 太快了，每秒可以认证 15w 次密码，简单的密码很容易被攻破，所以最好使用一个更复杂的密码。
# requirepass foobared

# 把危险的命令给修改成其他名称。比如 CONFIG 命令可以重命名为一个很难被猜到的命令，这样用户不能使用，而内部工具还能接着使用。
# rename-command CONFIG b840fc02d524045429941cc15f59e41cb7be6c52

# 设置成一个空的值，可以禁止一个命令
# rename-command CONFIG ""

################################### LIMITS ####################################
# 设置能连上 redis 的最大客户端连接数量。默认是 10000 个客户端连接。
# 由于 redis 不区分连接是客户端连接还是内部打开文件或者和 slave 连接等，所以 maxclients 最小建议设置到 32。
# 如果超过了 maxclients，redis 会给新的连接发送 max number of clients reached，并关闭连接。
# maxclients 10000

# redis配置的最大内存容量。当内存满了，需要配合 maxmemory-policy 策略进行处理。
# 注意 slave 的输出缓冲区是不计算在 maxmemory 内的。所以为了防止主机内存使用完，建议设置的 maxmemory 需要更小一些。
# maxmemory <bytes>

# 内存容量超过 maxmemory 后的处理策略。
# volatile-lru：利用 LRU 算法移除设置过过期时间的 key。
# volatile-random：随机移除设置过过期时间的 key。
# volatile-ttl：移除即将过期的 key，根据最近过期时间来删除（辅以 TTL）
# allkeys-lru：利用 LRU 算法移除任何 key。
# allkeys-random：随机移除任何 key。
# noeviction：不移除任何 key，只是返回一个写错误。
# 上面的这些驱逐策略，如果 redis 没有合适的key驱逐，对于写命令，还是会返回错误。redis将不再接收写请求，只接收get请求。
# 写命令包括：set setnx setex append incr decr rpush lpush rpushx lpushx linsert lset 
# rpoplpush sadd sinter sinterstore sunion sunionstore sdiff sdiffstore zadd zincrby zunionstore 
# zinterstore hset hsetnx hmset hincrby incrby decrby getset mset msetnx exec sort。
# maxmemory-policy noeviction

# lru 检测的样本数。使用 lru 或者 ttl 淘汰算法，从需要淘汰的列表中随机选择 sample 个 key，选出闲置时间最长的 key 移除。
# maxmemory-samples 5

############################## APPEND ONLY MODE ###############################
# 默认 redis 使用的是 rdb 方式持久化，这种方式在许多应用中已经足够用了。
# 但是 redis 如果中途宕机，会导致可能有几分钟的数据丢失，根据 save 来策略进行持久化，
# Append Only File 是另一种持久化方式，可以提供更好的持久化特性。
# Redis会把每次写入的数据在接收后都写入 appendonly.aof 文件，每次启动时 Redis 都会先把这个文件的数据读入内存里，先忽略 RDB 文件。
appendonly no

# aof 文件名
appendfilename "appendonly.aof"

# aof 持久化策略的配置
# no 表示不执行 fsync，由操作系统保证数据同步到磁盘，速度最快。
# always 表示每次写入都执行 fsync，以保证数据同步到磁盘。
# everysec 表示每秒执行一次 fsync，可能会导致丢失这 1s 数据。
appendfsync everysec

# 在 aof 重写或者写入 rdb 文件的时候，会执行大量 IO，此时对于 everysec 和 always 的 aof 模式来说，
# 执行 fsync 会造成阻塞过长时间，no-appendfsync-on-rewrite 字段设置为默认设置为 no。
# 如果对延迟要求很高的应用，这个字段可以设置为 yes，否则还是设置为 no，这样对持久化特性来说这是更安全的选择。
# 设置为 yes 表示 rewrite 期间对新写操作不 fsync，暂时存在内存中，等 rewrite 完成后再写入，默认为 no，建议 yes。
# Linux 的默认 fsync 策略是 30 秒。可能丢失 30 秒数据。
no-appendfsync-on-rewrite no

# aof 自动重写配置。当目前 aof 文件大小超过上一次重写的 aof 文件大小的百分之多少进行重写，
# 即当 aof 文件增长到一定大小的时候 Redis 能够调用 bgrewriteaof 对日志文件进行重写。
# 当前 AOF 文件大小是上次日志重写得到 AOF 文件大小的二倍（设置为 100）时，自动启动新的日志重写过程。
auto-aof-rewrite-percentage 100

#设置允许重写的最小 aof 文件大小，避免了达到约定百分比但尺寸仍然很小的情况还要重写
auto-aof-rewrite-min-size 64mb

# aof 文件可能在尾部是不完整的，当 redis 启动的时候，aof 文件的数据被载入内存。
# 重启可能发生在 redis 所在的主机操作系统宕机后，尤其在 ext4 文件系统没有加上 data=ordered 选项（redis宕机或者异常终止不会造成尾部不完整现象。）
# 出现这种现象，可以选择让 redis 退出，或者导入尽可能多的数据。
# 如果选择的是 yes，当截断的 aof 文件被导入的时候，会自动发布一个 log 给客户端然后 load。
# 如果是 no，用户必须手动 redis-check-aof 修复 AOF 文件才可以。
aof-load-truncated yes

################################ LUA SCRIPTING ###############################
# 如果达到最大时间限制（毫秒），redis 会记个 log，然后返回 error。当一个脚本超过了最大时限。
# 只有 SCRIPT KILL和SHUTDOWN NOSAVE 可以用。第一个可以杀没有调 write 命令的东西。要是已经调用了 write，只能用第二个命令杀。
lua-time-limit 5000

################################ REDIS CLUSTER ###############################
# 集群开关，默认是不开启集群模式。
# cluster-enabled yes

# 集群配置文件的名称，每个节点都有一个集群相关的配置文件，持久化保存集群的信息。
# 这个文件并不需要手动配置，这个配置文件有 Redis 生成并更新，每个 Redis 集群节点需要一个单独的配置文件，
# 请确保与实例运行的系统中配置文件名称不冲突
# cluster-config-file nodes-6379.conf

# 节点互连超时的阀值。集群节点超时毫秒数
# cluster-node-timeout 15000

# 在进行故障转移的时候，全部 slave 都会请求申请为 master，但是有些 slave 可能与 master 断开连接一段时间了，
# 导致数据过于陈旧，这样的 slave 不应该被提升为 master。
# 该参数就是用来判断 slave 节点与 master 断线的时间是否过长。判断方法是：
# 比较 slave 断开连接的时间和 (node-timeout * slave-validity-factor) + repl-ping-slave-period
# 如果节点超时时间为三十秒，并且 slave-validity-factor 为 10，假设默认的 repl-ping-slave-period 是 10 秒，
# 即如果超过 310 秒 slave 将不会尝试进行故障转移 
# cluster-slave-validity-factor 10

# master 的 slave 数量大于该值，slave 才能迁移到其他孤立 master 上，如这个参数若被设为 2，
# 那么只有当一个主节点拥有 2 个可工作的从节点时，它的一个从节点会尝试迁移。
# cluster-migration-barrier 1

# 默认情况下，集群全部的 slot 有节点负责，集群状态才为 ok，才能提供服务。
# 设置为no，可以在 slot 没有全部分配的时候提供服务。
# 不建议打开该配置，这样会造成分区的时候，小分区的 master 一直在接受写请求，而造成很长时间数据不一致。
# cluster-require-full-coverage yes

################################## SLOW LOG ###################################
# slog log 是用来记录 redis 运行中执行比较慢的命令耗时。
# 当命令的执行超过了指定时间，就记录在 slow log 中，slog log 保存在内存中，所以没有 IO 操作。
# 执行时间比 slowlog-log-slower-than 大的请求记录到 slowlog 里面，单位是微秒，所以 1000000 就是 1 秒。
# 注意，负数时间会禁用慢查询日志，而 0 则会强制记录所有命令。
slowlog-log-slower-than 10000

# 慢查询日志长度。当一个新的命令被写进日志的时候，最老的那个记录会被删掉。
# 这个长度没有限制。只要有足够的内存就行。你可以通过 SLOWLOG RESET 来释放内存。
slowlog-max-len 128

################################ LATENCY MONITOR ##############################
# 延迟监控功能是用来监控 redis 中执行比较缓慢的一些操作，用 LATENCY 打印 redis 实例在跑命令时的耗时图表。
# 只记录大于等于下边设置的值的操作。0 的话，就是关闭监视。
# 默认延迟监控功能是关闭的，如果你需要打开，也可以通过 CONFIG SET 命令动态设置。
latency-monitor-threshold 0

############################# EVENT NOTIFICATION ##############################
# 键空间通知使得客户端可以通过订阅频道或模式，来接收那些以某种方式改动了 Redis 数据集的事件。
# 因为开启键空间通知功能需要消耗一些 CPU ，所以在默认配置下，该功能处于关闭状态。
# notify-keyspace-events 的参数可以是以下字符的任意组合，它指定了服务器该发送哪些类型的通知：
##K 键空间通知，所有通知以 __keyspace@__ 为前缀
##E 键事件通知，所有通知以 __keyevent@__ 为前缀
##g DEL 、 EXPIRE 、 RENAME 等类型无关的通用命令的通知
##$ 字符串命令的通知
##l 列表命令的通知
##s 集合命令的通知
##h 哈希命令的通知
##z 有序集合命令的通知
##x 过期事件：每当有过期键被删除时发送
##e 驱逐(evict)事件：每当有键因为 maxmemory 政策而被删除时发送
##A 参数 g$lshzxe 的别名
#输入的参数中至少要有一个 K 或者 E，否则的话，不管其余的参数是什么，都不会有任何 通知被分发。详细使用可以参考http://redis.io/topics/notifications

notify-keyspace-events ""

############################### ADVANCED CONFIG ###############################
# 数据量小于等于 hash-max-ziplist-entries 的用 ziplist，大于 hash-max-ziplist-entries 用 hash
hash-max-ziplist-entries 512

# value 大小小于等于 hash-max-ziplist-value 的用 ziplist，大于 hash-max-ziplist-value 用 hash。
hash-max-ziplist-value 64

# 数据量小于等于 list-max-ziplist-entries 用 ziplist，大于 list-max-ziplist-entries 用 list。
list-max-ziplist-entries 512

# value 大小小于等于 list-max-ziplist-value 的用 ziplist，大于 list-max-ziplist-value 用 list。
list-max-ziplist-value 64

# 数据量小于等于 set-max-intset-entries 用 iniset，大于 set-max-intset-entries 用 set。
set-max-intset-entries 512

# 数据量小于等于 zset-max-ziplist-entries 用 ziplist，大于 zset-max-ziplist-entries 用 zset。
zset-max-ziplist-entries 128

# value大小小于等于zset-max-ziplist-value 用 ziplist，大于 zset-max-ziplist-value 用 zset。
zset-max-ziplist-value 64

# value 大小小于等于 hll-sparse-max-bytes 使用稀疏数据结构（sparse），大于 hll-sparse-max-bytes 使用稠密的数据结构（dense）。
# 一个比 16000 大的 value 是几乎没用的，建议的 value 大概为 3000。如果对 CPU 要求不高，对空间要求较高的，建议设置到 10000 左右。
hll-sparse-max-bytes 3000

# Redis 将在每 100 毫秒时使用 1 毫秒的 CPU 时间来对 redis 的 hash 表进行重新 hash，可以降低内存的使用。
# 当你的使用场景中，有非常严格的实时性需要，不能够接受 Redis 时不时的对请求有 2 毫秒的延迟的话，把这项配置为 no。
# 如果没有这么严格的实时性要求，可以设置为 yes，以便能够尽可能快的释放内存。
activerehashing yes

## 对客户端输出缓冲进行限制可以强迫那些不从服务器读取数据的客户端断开连接，用来强制关闭传输缓慢的客户端。
# 对于 normal client，第一个 0 表示取消 hard limit，第二个 0 和第三个 0 表示取消 soft limit
# normal client 默认取消限制，因为如果没有寻问，他们是不会接收数据的。
client-output-buffer-limit normal 0 0 0

# 对于 slave client 和 MONITER client，如果 client-output-buffer 一旦超过 256mb，又或者超过 64mb 持续 60 秒，那么服务器就会立即断开客户端连接。
client-output-buffer-limit slave 256mb 64mb 60

# 对于 pubsub client，如果 client-output-buffer 一旦超过 32mb，又或者超过 8mb 持续 60 秒，那么服务器就会立即断开客户端连接。
client-output-buffer-limit pubsub 32mb 8mb 60

# redis 执行任务的频率为 1s 除以 hz。
hz 10

# 在 aof 重写的时候，如果打开了 aof-rewrite-incremental-fsync 开关，系统会每 32MB 执行一次 fsync。
# 这对于把文件写入磁盘是有帮助的，可以避免过大的延迟峰值。
aof-rewrite-incremental-fsync yes
```

## Redis 持久化

redis持久化，顾名思义，身为一个内存级数据库，要想保证应对突发状况，就需要把内存中的数据持久化到硬盘中。

redis中有两种持久化方式

- RDB：将当前数据状态进行保存，以快照的形式。
- AOF：将数据的操作过程进行保存，以日志的形式。

### RDB

#### RDB启动方式`save`

```
# 手动进行一次持久化
save
```

在 Linux 中`dump.rdb`保存在`/var/lib/redis`中

在 `redis.conf` 中，关于RDB 的设置有：

- `dbfilename dump.rdb`
  - 说明：设置本地数据库文件，默认为 `dump.rdb`
  - 经验：通常设置为 `dump-端口号.rdb`
- `dir /var/lib/redis`
  - 说明：设置存储rdb文件的路径
  - 经验：通常设置在存储空间较大的目录中，名称为 `data`
- `rdbcompression yes`
  - 说明：设置存储本地数据时是否压缩数据，默认为 `yes`,采用`LZF`进行压缩
  - 经验：通常为开启状态，如果设置为`no`，可以节省CPU时间，但是会使存储的文件变大
- `rdbchecksum yes`
  - 说明：设置 RDB 文件格式校验，该校验过程在写文件和读文件过程均进行
  - 经验：通常默认为开启状态，如果设置为`no`，可以节约读写性过程约`10%`时间消耗，但是存储的数据有一定的损坏风险

数据恢复：

**redis在启动时，会去在配置文件中指定的数据存储路径中，如果发现有备份好的数据，就会自动加载进内存。**

#### `save` 指令的工作原理

由于 redis 是单线程执行的，假设现在有四个客户端同时向 redis 服务器发送不同的请求，如果有一个请求是 `save` 则，在 `save` 操作完成之前，redis的进程会一直阻塞， 这就会倒是其他的客户端阻塞，所以**线上环境一般不建议使用**

#### `bgsave`指令

作用是在后台指定保存命令。

原理：

1. 客户端给服务端发送`bgsave`指令
2. 服务端给客户端返回已经开始后台执行的消息
3. 服务端 `fork` 出一个子进程，让新进程去做这件事情，创建`rdb`

区别是：`bgsave`是针对`save`阻塞的优化。

关于`bgsave`的配置有：

- `stop-writes-on-bgsave-error yes`
  - 说明：后台存储如果出现错误现象，是否停止保存操作
  - 经验：默认为开启状态

#### 自动执行持久化

命令：

```
# 配置文件中
save second changes
```

作用：

- 满足限定时间范围内key的变化数量达到指定数量开始进行持久化，在后台默认用的是`bgsave`

参数：

- second：监控时间范围
- changes：监控key的变化量

#### RDB三种启动方式对比

|      方式      | `save`指令 | `bgsave`指令 |
| :------------: | :--------: | :----------: |
|      读写      |    同步    |     异步     |
| 阻塞客户端指令 |     是     |      否      |
|  额外内存消耗  |     否     |      是      |
|   启动新进程   |     否     |      是      |

其他指令：

- `debug reload`：服务器运行过程中重启
- `shutdown save`：关闭服务器时指定保存数据

#### RDB优缺点

优点：

- RDB是一个紧凑压缩的二进制文件，存储效率高
- RDB内部存储的是redis在某个时间节点的数据快照，非常适合用于数据备份，全量恢复等场景
- RDB恢复数据的速度比AOF快得多
- 应用：服务器每隔X小时执行bgsave备份，并将RDB文件拷贝到远程机器中，用于灾难恢复

缺点：

- RDB方式无论是执行指令还是利用配置，无法做到实时持久化，具有较大的可能性丢失数据
- bgsave指令每次运行需要`fork`一个新进程，要牺牲一些性能
- redis众多版本中未进行RDB文件格式的统一，可能出现各版本服务之间的数据无法兼容现象

### AOF

由于RDB基于快照，不能实时存储，且需要性能与额外的空间，所以redis还有另一种持久化方案。

那就是记录操作记录。

- AOF（append onlyfile）持久化：以独立日志的方式每次写命令，重启时重新执行AOF文件中的命令达到恢复数据的目的。与RDB相比可以简单的描述为改记录数据为记录数据产生的过程。
- AOF的主要作用是为了解决数据持久化的实时性，目前已经是redis数据持久化的主流方式。

#### AOF 写数据过程

在redis中执行命令以后，首先把命令放在缓存中，符合某种策略之后，将命令同步到AOF文件中。

#### AOF写数据的三种策略(`appendfsync`)

- always（每次）
  - 每次写入均同步，性能较低，不建议使用
- everysec（每秒）
  - 每秒将缓冲区的指令同步到AOF中，数据准确性高，性能较高，在系统突然宕机丢失一秒数据
- no（系统控制）
  - 由操作系统控制每次同步到AOF文件的周期，整体过程不可控。

#### AOF功能开启

```
# 是否开启AOF持久化功能，默认不开启
appendonly yes|no

# AOF写数据策略
appendfsync everysec

# 自定义文件名
appendfilename "appendonly.aof"
```

#### AOF重写机制

随着命令不断的写入AOF，文件会越来越大，redis引入了AOF重写机制压缩文件体积。AOF文件重写是将Redis进程内的数据转化为写命令同步到新AOF文件的过程。简单说就是对于同一个数据的若干个命令执行结果可以合并，我们转化成最终结果数据的指令进行记录即可。

作用：

- 降低磁盘占用，提高磁盘利用效率
- 提高持久化效率，降低持久化写时间，提高IO性能
- 降低数据恢复用时，提高数据恢复效率

#### AOF 重写规则

- 进程内已超时的数据不再写入文件
- 忽略无效指令，重写时使用进程内数据直接生成，这样新的AOF文件只保留最终数据的写入命令
- 对同一条命令的多条指令合并为同一条命令

#### AOF重写方式

```
# 手动重写
bgrewriteaof

# 自动重写触发条件
auto-aof-rewrite-percentage 100
auto-aof-rewrite-min-size 64mb

# 自动重写出发对比参数（运行 info Persistence 获取具体信息）
aof_current_size
aof_base_size

# 自动重写出发条件
aof_current_size>auto-aof-rewrite-min-size
(aof_current_size-aof_base_size)/aof_base_size >= auto-aof-rewrite-percentage
```

手动重写流程与rdb类似，也是`fork`出一个新的进程进行这个操作。

### `RDB` VS `AOF`

|  持久化方式  |        RDB         |        AOF         |
| :----------: | :----------------: | :----------------: |
| 占用存储空间 | 小（数据级：压缩） | 大（指令级：重写） |
|   存储速度   |         慢         |         快         |
|   恢复速度   |         快         |         慢         |
|  数据安全性  |     会丢失数据     |    根据策略决定    |
|   资源消耗   |     高/重量级      |     低/轻量级      |
|  启动优先级  |         低         |         高         |

如何选择：

- 对数据非常敏感，用默认的AOF持久化方案
  - 每秒存一次，最多丢失一秒数据
  - 注意：AOF文件体积较大，且恢复速度慢
- 数据呈现有效性，建议使用RDB
  - 数据可以良好的做到阶段内无丢失，且恢复速度快，阶段点数据适合RDB
  - 注意：性能比较慢
- 综合比对：
  - RDB与AOF是一种权衡，各有利弊
  - 不能承受分钟级的数据丢失，对业务数据敏感，用AOF
  - 可以承受用RDB
  - 灾难恢复用RDB
  - 双保险策略，同时开启。

## Redis 事务

redis 事务就是一个命令执行的队列，将一些列预定义命令包装成一个整体（一个队列）。当执行时，一次性按照添加顺序依次执行，中间不会被打断或者干扰。

### 事务的基本操作

- 开启事务：`multi`

  - 设定事务的开启位置，此指令执行以后，后续所有的指令均加入到事务中

- 执行事务：`exec`

  - 设定事务的结束位置，同时执行事务。与`multi`成对出现，成对使用

- 取消事务：`discard`

  - 终止当前事务的定义，`multi`之后，`exec`之前
  

redis事务实质上是维护了一个队列，没有立即执行，逐次把事务加入队列，当exec的时候，就执行队列里面的操作。

### 事务的注意事项

- 如果事务的执行过程中，遇到了语法错误，那么整个事务直接终止
- 如果事务里面语法没问题，但是执行的过程中遇到了比如类型不匹配，事务仍然可以执行
- 已经执行完的命令对应的数据不会自动回滚，回滚需要程序员自己在代码中保证

### 锁

#### 监视锁

监控一个值变不变

- 对key添加监视锁，在执行exec前如果key发生了变化，终止事务执行

  ```
  watch key1 [key2...]
  ```

- 取消对所有key的监视

  ```
  unwatch
  ```

- 必须在开启事务之前进行`watch`或者`unwatch`

#### 分布式锁

- 使用 `setnx` 设置一个公共锁，返回`1`代表成功拿到锁，`0`代表失败

```
setnx lock-key value
```

利用setnx的返回值特征，有值则返回设置失败，无值则代表成功。

通过使用 `expire` 来指定一个过期时间，来防止忘了释放锁的情况。

## redis 删除策略

redis是一种内存级数据库，所有数据放在内存中，内存中的数据可以通过ttl指令获取其状态。

- xx：具有时效性的数据
- -1：永久性数据
- -2：**已经过期的数据** 或 被删除的数据 或 未定义的数据

当redis对数据进行删除的时候，并不会马上删除，而是有数据删除策略

### 数据删除策略

#### 定时删除

- 创建一个定时器，当key设置有过期时间，且过期时间到达时，由定时器任务立即执行对键的删除操作
- 优点：节约内存，到时就删除，快速释放掉不必要的内存占用
- 缺点：CPU压力很大，无论此时CPU负载有多高，均占用CPU，会影响redis服务器的响应时间和指令吞吐量
- 总结：用处理器性能换取时间

#### 惰性删除

- 数据到达过期时间，不做处理，等下次访问该数据时，判断一下，执行删除，返回空
- 优点：节约CPU性能，发现必须删除再删除
- 缺点：内存压力很大，出现长期占用内存数据
- 用时间换空间

#### 定期删除

上面两种方案都太极端。redis采用折中方案：

- 周期性轮询redis中的时效性数据，采用随机抽取策略，利用过期时间占比的方式控制删除频度
- 特点1：CPU性能占用设置有峰值，检测频度可以自定义设置
- 特点2：内存压力不是很大， 长期占用内存的冷数据会被持续清理
- 总结：周期性检查存储空间

### 逐出算法

redis使用内存存储数据，在执行一个命令前，会调用 `freeMemoryIfNeeded()` 检测内存是否充足。如果内存不满足新加入数据的最低存储要求，redis要临时删除一些数据为当前指令清理存储空间。清理数据的策略被称为逐出算法。

逐出算法不是 100% 可以清理出存储空间，如果不成功会反复执行。当所有数据尝试完毕以后，如果不能达到内存的清理要求，会出现错误信息。

#### 逐出相关配置

在配置文件中：

- 最大可使用内存

```
maxmemory
```

默认为0，表示不限制。生产环境中根据需求设定。

- 每次选取待删除数据的个数

```
maxmemory-samples
```

选取数据时并不会全库扫描，导致严重的性能损耗，降低读写性能。因此采用随机获取数据的方式作为待检测删除数据。

- 删除策略

```
maxmemory-policy volatile-lru
```

达到最大内存后，对被挑选出来的数据进行删除的策略。

数据逐出相关策略：

检测易失数据（可能会过期的数据集`server.db[i].expires`）

- `volatile-lru`：挑选最近最少使用的数据淘汰（按时间选）**早期默认为这个**
- `volatile-lfu`：挑选最近使用次数最少的数据淘汰（按使用次数选）
- `volatile-ttl`：挑选要过期的数据淘汰
- `volatile-random`：随机挑选数据进行淘汰

检测全库数据（所有数据集`server.db[i].dict`）

- `allkeys-lru`
- `allkeys-lfu`
- `allkeys-random`

放弃数据驱逐：

- `no-enviction`：进制驱逐数据，redis4.0默认策略，可能造成内存泄漏

## redis 高级数据类型

### `Bitmaps`

#### 基础操作

```
# 获取指定key对应偏移量的bit值
getbit key offset

# 设置指定key对应偏移量的bit值，只能为0或1
setbit key offset value
```

#### 扩展操作

```
# 对指定的key按位进行交、并、非、异或的操作，并且将结果保存在desykey中
# and 交，or 并，not 非，xor 异或
bitop op destkey key1 [key2...]

# 统计指定key中1的数量
bitcount key [st ed]
```

### `HyperLogLog`

Redis 在 2.8.9 版本添加了 HyperLogLog 结构。

Redis HyperLogLog 是用来做基数统计的算法，HyperLogLog 的优点是，在输入元素的数量或者体积非常非常大时，计算基数所需的空间总是固定 的、并且是很小的。

在 Redis 里面，每个 HyperLogLog 键只需要花费 12 KB 内存，就可以计算接近 2^64 个不同元素的基 数。这和计算基数时，元素越多耗费内存就越多的集合形成鲜明对比。

但是，因为 HyperLogLog 只会根据输入元素来计算基数，而不会储存输入元素本身，所以 HyperLogLog 不能像集合那样，返回输入的各个元素。

```
# 添加数据
pfadd key element [element...]

# 统计数据
pfcount key [key ...]

# 合并数据
pfmerge destkey sourcekey [sourcekey...]
```

### `GEO`

Redis 是最热门的 nosql 数据库之一，它的最大特点就是快。
所以在 LBS 这种需要大量写入和查询的应用场景中，用它来存储用户的地理位置信息最适合不过了。

Redis 的 GEO 是 3.2 版本的新特性。这个功能可以将用户给定的地理位置信息储存起来， 并对这些信息进行操作。

目前 redis 支持以下 6 个 GEO 的相关操作

- geoadd：增加某个地理位置的坐标。
- geopos：获取某个地理位置的坐标。
- geodist：获取两个地理位置的距离。
- georadius：根据给定地理位置坐标获取指定范围内的地理位置集合。
- georadiusbymember：根据给定地理位置获取指定范围内的地理位置集合。
- geohash：获取某个地理位置的geohash值。

基本操作：

```
# 添加坐标点
GEOADD key longitude latitude member [longitude latitude member ...]

# 获取坐标点
GEOPOS key member [member ...]

# 计算坐标点中距离
GEODIST key member1 member2 [unit]

# 根据坐标求范围内的数据
GEORADIUS key longitude latitude radius m|km|ft|mi [WITHCOORD] [WITHDIST] [WITHHASH] [COUNT count] [ASC|DESC] [STORE key] [STOREDIST key]

# 根据点求范围内数据
GEORADIUSBYMEMBER key member radius m|km|ft|mi [WITHCOORD] [WITHDIST] [WITHHASH] [COUNT count] [ASC|DESC] [STORE key] [STOREDIST key]

# 获取指定点对应的坐标hash值
GEOHASH key member [member ...]
```

## 主从复制

互联网「三高」架构：

- 高并发
- 高性能
- 高可用

互联网可用性计算标准：

> 一年总共有 `365*24*60*60=31536000` 秒，假设服务器宕机 x 秒，则可用性为：`(31536000-x)/31536000*100%` ,值即为可用性。
>
> 业界可用性目标为 5 个 9 ，即服务器全年宕机时常低于315秒，约 5.25 分钟

### 多台服务器连接方案

- 提供数据方 `master`
  - 主服务器，主节点，主库，主客户端
- 接受数据费 `slave`
  - 从服务器，从节点，从库，从客户端
- 需要解决的问题：
  - 数据同步
- 核心工作：
  - master数据复制到slave中

### 主从复制简介

将master的数据及时，有效的复制到slave中

特征：一个master可以拥有多个slave，但是一个slave只对应一个master

**主写从读**

职责：

- master
  - 写数据
  - 执行写操作时，将出现变化的数据自动同步到slave上
  - 读数据（可忽略）
- slave
  - 读数据
  - 写数据（禁止）

主从复制作用：

- 读写分离：master写，slave读，提高服务器读写负载能力
- 负载均衡：基于主从结构，配合读写分离，slave分担master的负载，根据需求的变化，改变slave的数量，通过多个从节点分担数据负载，提高并发量与数据吞吐量
- 故障恢复：当master出现问题，由slave提供服务，实现快速的故障恢复。
- 高可用基石：基于主从复制，构建哨兵模式与集群，实现redis的高可用方案。

### 主从复制工作流程

主从复制过程大概可以分为三个阶段：

- 建立连接阶段
- 数据同步阶段
- 命令传播阶段

建立连接阶段工作流程：

1. 设置master的地址和端口，保存master信息
2. 建立socket连接
3. 发送ping命令
4. 身份验证
5. 发送slave端口信息

至此，主从连接成功！

状态：

```
slave:
保存master的地址与端口。
master:
保存slave的端口。
```

### 主从连接（slave 连接 master）

```
# 方式一：客户端发送命令
slaveof <masterip> <masterport>

# 方式二：启动服务器参数
redis-server -slaveof <masterip> <masterport>

# 方式三：服务器配置
slaveofg <masterip> <masterport>
```

- slave系统信息
  - master_link_down_since_seconds
  - masterhost
  - masterport
- master系统信息
  - slave_listening_port（多个）