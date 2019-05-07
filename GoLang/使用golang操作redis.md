## 使用 golang 操作 redis

首先使用 `go get` 安装第三方开源库。

```shell
go get github.com/garyburd/redigo/redis
```

### 连接 redis

```go
package main

import (
    "fmt"
    "github.com/garyburd/redigo/redis"
)
func main() {
    c, err := redis.Dial("tcp", "localhost:6379")
    if err != nil {
        fmt.Println("conn redis failed,", err)
        return
    } 
    fmt.Println("redis conn success")
    defer c.Close()
}
```

通过这个语句可以判断是否连接到了 redis

### 代码示例

接下来直接通过代码来举例各种功能

```go
package main

import (
	"fmt"
	"time"

	"github.com/garyburd/redigo/redis"
)

func main() {
	c, err := redis.Dial("tcp", "localhost:6379") //连接库
	if err != nil {
		fmt.Println("conn redis failed,", err)
		return
	}
	defer c.Close()
	/* string类型的 set 和 get操作  */
	fmt.Println("string 类型的 set 和 get操作:")
	_, err = c.Do("Set", "abc", 100) //进行 set abc 100 的操作
	if err != nil {
		fmt.Println(err)
		return
	}
	r1, err := redis.Int(c.Do("Get", "abc")) //读取数据 get abc ，然后转换成int型
	if err != nil {
		fmt.Println("get abc failed,", err)
		return
	}
	fmt.Println(r1) //输出 100 ，为刚刚设置的 abc

	/* string 类型的批量操作 */
	fmt.Println("string 类型的批量操作:")
	_, err = c.Do("mset", "abc", 100, "efg", 300) // 执行 mset abc 100 efg 300
	if err != nil {
		fmt.Println(err)
		return
	}
	r2, err := redis.Ints(c.Do("mget", "abc", "efg")) //执行 mget abc efg,并转换成 int 型的切片
	if err != nil {
		fmt.Println("get abc failed", err)
		return
	}
	for _, v := range r2 {
		fmt.Println(v)
	}
	/* 设置过期时间，过期时间过了之后，该元素会自动删除 */
	fmt.Println("设置过期时间，过期时间过了之后，该元素会自动删除:")
	_, err = c.Do("set", "abc", "100")
	_, err = c.Do("expire", "abc", 3) // 执行 expire abc 3 ,代表3秒后 abc 这个元素就不存在了
	if err != nil {
		fmt.Println(err)
		return
	}
	r3, err := redis.String(c.Do("get", "abc")) //执行 get abc
	fmt.Println("abc is : ", r3)
	time.Sleep(5 * time.Second) //停止5秒
	r3, err = redis.String(c.Do("get", "abc"))
	if err != nil {
		fmt.Println(err) //如果输出，就证明元素已经删除了
	} else {
		fmt.Println("after 10s , abc is : ", r3)
	}

	/* list 队列操作 */
	fmt.Println("list 队列操作")
	_, err = c.Do("lpush", "book_list", "abc", "ceg", 300) //执行 lpush book_list abc ceg 300
	if err != nil {
		fmt.Println(err)
		return
	}
	r4, err := redis.String(c.Do("lpop", "book_list")) //执行 lpop book_list ,移出并获取列表的第一个元素
	if err != nil {
		fmt.Println("get abc failed", err)
		return
	}
	fmt.Println("删除的元素为：", r4)
	r5, err := redis.Strings(c.Do("lrange", "book_list", "0", "10")) // 执行 lrange book_list 0 10
	fmt.Print("剩余元素为：")
	for _, v := range r5 {
		fmt.Print(v, " ")
	}
	fmt.Println()

	/* hash表操作 */
	fmt.Println("hash表操作")
	_, err = c.Do("hset", "books", "abc", 100) //执行 hset books abc 100
	if err != nil {
		fmt.Println(err)
		return
	}
	r6, err := redis.Int(c.Do("hget", "books", "abc")) // 执行 hget books abc
	if err != nil {
		fmt.Println("get abc failed", err)
		return
	}
	fmt.Println(r6)
}

```

### redis 连接池

直接通过代码来看

```go
package main

import (
	"fmt"

	"github.com/garyburd/redigo/redis"
)

var pool *redis.Pool //创建redis连接池

func init() {
	pool = &redis.Pool{ //实例化一个连接池
		MaxIdle: 16, //最初的连接数量
		// MaxActive:1000000,    //最大连接数量
		MaxActive:   0,   //连接池最大连接数量,不确定可以用0（0表示自动定义），按需分配
		IdleTimeout: 300, //连接关闭时间 300秒 （300秒不使用自动关闭）
		Dial: func() (redis.Conn, error) { //要连接的redis数据库
			return redis.Dial("tcp", "localhost:6379")
		},
	}
}

func main() {
	c := pool.Get() //从连接池，取一个链接
	defer c.Close() //函数运行结束 ，把连接放回连接池

	_, err := c.Do("Set", "abc", 200)
	if err != nil {
		fmt.Println(err)
		return
	}

	r, err := redis.Int(c.Do("Get", "abc"))
	if err != nil {
		fmt.Println("get abc faild :", err)
		return
	}
	fmt.Println(r)
	pool.Close() //关闭连接池
}
```

