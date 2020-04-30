# redis学习笔记

主要记录一些redis使用方面的知识。

redis可视化客户端：`Redis Desktop Manager`

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

