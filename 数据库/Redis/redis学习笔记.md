# redis学习笔记

主要记录一些redis使用方面的知识。

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

设置数据具有指定的生命周期：

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



