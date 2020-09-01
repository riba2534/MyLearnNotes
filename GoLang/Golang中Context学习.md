# Golang context 学习

在服务端的日常开发中，我们几乎总是将函数的第一个参数写为 `ctx context.Context`，你是否有过以下疑惑？ 

1. 这个 `ctx` 是干啥的？为啥每个函数的第一个参数都是 `ctx`？ 
2. `Context` 能给我们带来什么好处？怎么用？ 
3. `Context` 内部是咋实现的，复杂吗？ 
5. 好像从 `ctx` 中能够获取好多信息，那么是怎么获取的？难道里面存了个 `map`？ 我们可以带着以上的问题一起来往下看。

## 怎样实现主协程对多个子协程的控制？

我们经常会遇到主协程与子协程之间存在控制、协同、取消的需求，例如：

- 主协程需要等待全部子协程完成：即子协程怎样通知主协程。
- 在使用流水线增加数据计算吞吐量的场景中：对于一个输入，该输入需要经过流水线上的多个子任务处理函数一步步串行计算最终得到结果，主协程等待一段时间后，若整体任务还没有完成则需要抛弃这个输入。而抛弃的那一刻，该任务可能处于当期某个子任务中，怎样在下一个子任务开始的时候抛弃这个输入不再浪费计算资源：即主协程通知子协程放弃任务。

另外，上下文信息的传递也是强需求，如 `http` 请求的处理中，下游可能需要请求的各种参数（如用户信息、登录设备信息等），总是需要一个函数参数能够带给下游函数。

在实际应用中，通常每来一个请求都会启动一个协程处理该任务，而该协程可能会启动若干个子协程同时工作：有些去数据库拿数据，有些调用下游 `RPC` 接口获取相关数据：

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111752.png)

这些协程需要共享这个请求的基本数据（例如登陆的用户信息），处理请求的最大超时时间等等（如果超过此值再返回数据，请求方因为超时接收不到结果），当请求被取消或是处理时间太长则可能直接放弃了这次请求结果。这时，所有正在为这个请求工作的协程需要快速退出，因为它们的“工作成果”不再被需要了。在相关联的协程都退出后，系统就可以回收相关的资源。

为什么一定要有超时时间呢？ 因每次处理一个请求都新启一个协程，若某个下游服务的响应变慢，而当前系统的请求又没有超时控制，或者超时时间设置地过大，那么等待下游服务返回数据的协程就会越来越多。而协程是要消耗系统资源的，后果就是协程数激增，内存占用飙涨，甚至导致服务不可用。更严重的会导致雪崩效应，整个服务对外表现为不可用。

`Context` 就是为了解决上述问题而生的，主要用来传递上下文信息，包括：取消信号、超时时间、截止时间、携带 `kv` 等。

### 1. 使用 `WaitGroup`

使用 `WaitGroup` 的场景多为多个子任务之间无直接关联，且子任务的数量已知，只有全部子协程都完成后整体的任务才算完成。如下面的例子：

```go
func TestWaitGroup(t *testing.T)  {
   var wg sync.WaitGroup
   var num int32
   for i := 1; i <= 3; i++ {
      wg.Add(1)
      go func(num *int32, deferFun func()) {
         defer deferFun()
         atomic.AddInt32(num, 1)
      }(&num, wg.Done)
   }
   wg.Wait()
   fmt.Println("num: ", num)
}
```

可见 `WaitGroup` 主要用在子协程通知主协程的场景，那么主协程怎么通知子协程呢？考虑以下场景： 开启一个后台子协程一直做一些事情，后来主协程想让这个子协程结束不再浪费计算资源了，子协程怎么知道这个取消的消息呢？还能用 `WaitGroup` 做么？（难道把 `WaitGroup` 传到子协程中调用 `Wait`？但调用 `Wait` 后子协程会阻塞进行等待，但是子协程要处理的任务可能是周期性的，不可能只做一次就阻塞了）

### 2. 使用全局变量

使用全局变量是一种比较简单粗暴的做法，主协程先把全局变量开关打开，开启一个子协程开始周期性的完成任务，每完成一次就查看一下全局变量是否需要退出。但此方法缺点也是比较明显的：

- 全局变量的读写是不是原子的？简单的整数类型数据可以用 `atomic` 库中的函数操作，但如果还需经过较复杂的运算呢？难道还要加个锁？
- 有一个类似需求就要加一个全局变量么？

### 3. 使用 `chan` + `select`

考虑以下例子：

```go
func TestWithChan(t *testing.T) {
   ch := make(chan struct{})
   go func() {
      for {
         select {
         case <-ch:
            fmt.Println("监控退出，停止了...")
            return
         default:
            fmt.Println("goroutine监控中...")
            time.Sleep(2 * time.Second)
         }
      }
   }()
   time.Sleep(5 * time.Second)
   fmt.Println("可以了，通知监控停止")
   close(ch)
   time.Sleep(5 * time.Second)
}
```

上述例子中定义里一个 `buf` 为0的通道，子协程周期性的完成任务，用 `select` 判断通道是否关闭，如果通道关闭那么子协程就可以退出了，满足主协程通知子协程的需求。 更进一步，那如果是流水线场景呢？

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111803.png)

可以将任务的入参通道与取消任务信号的通道传入到流水线任务中，主协程保存取消通道的引用并在合适的时机关闭此通道。流水线上处理函数在每个任务的开始时候检查一下通道是否为关闭，如果关闭则抛弃当前任务。如以下例子摘自[Go Concurrency Patterns: Pipelines and cancellation](https://blog.golang.org/pipelines)，目的为计算某个目录下每个文件的 `md5`，采用了3级流水线的方式：

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111815.png)

```go
package main

import (
   "crypto/md5"
   "errors"
   "fmt"
   "io/ioutil"
   "os"
   "path/filepath"
   "sort"
   "sync"
)

// walkFiles starts a goroutine to walk the directory tree at root and send the
// path of each regular file on the string channel.  It sends the result of the
// walk on the error channel.  If done is closed, walkFiles abandons its work.
func walfFiles(done <-chan struct{}, root string) (<-chan string, <-chan error) {
   paths := make(chan string)
   errc := make(chan error, 1)

   go func() {
      // Close the paths channel after Walk returns.
      defer close(paths)
      // No select needed for this send, since errc is buffered.
      errc <- filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
         if err != nil {
            return err
         }
         if !info.Mode().IsRegular() {
            return nil
         }
         select {
         case paths <- path:
         case <-done:
            return errors.New("walk canceled")
         }
         return nil
      })
   }()
   return paths, errc
}

// A result is the product of reading and summing a file using MD5.
type result struct {
   path string
   sum  [md5.Size]byte
   err  error
}

// digester reads path names from paths and sends digests of the corresponding
// files on c until either paths or done is closed.
func digester(done <-chan struct{}, paths <-chan string, c chan<- result) {
   for path := range paths {
      data, err := ioutil.ReadFile(path)
      select {
      case c <- result{path, md5.Sum(data), err}:
      case <-done:
         return
      }
   }
}

// MD5All reads all the files in the file tree rooted at root and returns a map
// from file path to the MD5 sum of the file's contents.  If the directory walk
// fails or any read operation fails, MD5All returns an error.  In that case,
// MD5All does not wait for inflight read operations to complete.
func MD5All(root string) (map[string][md5.Size]byte, error) {
   // MD5All closes the done channel when it returns; it may do so before
   // receiving all the values from c and errc.
   done := make(chan struct{})
   defer close(done)

   paths, errc := walfFiles(done, root)

   // Start a fixed number of goroutines to read and digest files.
   c := make(chan result)
   var wg sync.WaitGroup
   const Num = 20
   wg.Add(Num)
   for i := 0; i < Num; i++ {
      go func() {
         digester(done, paths, c)
         wg.Done()
      }()
   }
   go func() {
      wg.Wait()
      close(c)
   }()

   // End of pipeline. OMIT
   m := make(map[string][md5.Size]byte)
   for r := range c {
      if r.err != nil {
         return nil, r.err
      }
      m[r.path] = r.sum
   }
   // Check whether the Walk failed.
   if err := <-errc; err != nil { // HLerrc
      return nil, err
   }
   return m, nil
}
func main() {
   // Calculate the MD5 sum of all files under the specified directory,
   // then print the results sorted by path name.
   m, err := MD5All(os.Args[1])
   if err != nil {
      fmt.Println(err)
      return
   }
   var paths []string
   for path := range m {
      paths = append(paths, path)
   }
   sort.Strings(paths)
   for _, path := range paths {
      fmt.Printf("%x  %s\n", m[path], path)
   }
}
```

我们真实业务场景大部分情况下不是一条规整的、任务时间划分均匀的流水线，而是子任务超时时间不尽相同的树状结构，且每个子任务可能又要划分为多个串行或者并行的子任务。 如下图：任务1中存在3个子任务，如果任务1中还想对3个子任务有其自己的超时控制，那么子任务1-3是不是既要感知主协程的取消信号，又要感知任务1的取消信号？如果层级更深，最深级的任务难道要对N级取消信号遍历一遍？怎么方便的做到以下控制呢： 1. 上层取消任务后，当前任务的所衍生的所有下游任务都能知道该任务被取消了。 2. 当前任务取消后，不影响到同层其他任务，也不影响到上层任务。

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111825.png)

### 4. 使用 `context`

我们先看一个简单的例子，使用 `Context` 对上面的例子重写：

```go
func TestWithCancel(t *testing.T) {
   ctx, cancel := context.WithCancel(context.Background())
   go func(ctx context.Context) {
      for {
         select {
         case <-ctx.Done():
            fmt.Println("监控退出，停止了...")
            return
         default:
            fmt.Println("goroutine监控中...")
            time.Sleep(2 * time.Second)
         }
      }
   }(ctx)
   time.Sleep(5 * time.Second)
   fmt.Println("可以了，通知监控停止")
   cancel()
   time.Sleep(5 * time.Second)
}
```

你一定会对 `context.Background()` 很熟悉，该函数返回一个 `Context` 类型的值（具体细节后面详谈），`context.WithCancel` 函数输入一个 `Context` 类型的值，返回一个新的可以取消子 `Context` 和一个可以取消该子 `Context` 的函数 `cancel`。 在子协程中的 `select` 中，可以看到 `<-ctx.Done()` 返回一个通道，如果主协程关闭了通道（即发送了取消信号），那么 `<-ctx.Done()` 会被 `select` 选中，子协程退出。 怎么取消一个 `Context`？在主协程中明确调用了 `cancel` 函数，该函数会关闭 `Context` 内部的通道，即 `Context` 通过关闭通道的方式，将取消任务的信息告诉了子协程。 下面讨论一下 `Context` 包中的实现，就会了解到 `Context` 的取消信号是怎样在一个任务处理树中传递的。

## `Context` 内部是怎么实现的？

### 1. 结构初览

首先看一下整体的结构：

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111830.png)

接口：`Context` `canceler` 结构体：`emptyCtx` 为 `Context` 的一个实现；`cancelCtx`、`valueCtx` 中包含了 `Context` 类型的值（其实是其父 `Context` 的指针）；`timerCtx` 中包含了一个 `cancelCtx` 的值

`WithCancel`、`WithTimeout`、`WithDeadline`，传入一个父的 `Context` 类型值，返回一个新的可取消的 `Context` 和用于取消该返回 `Context` 的函数，同时保证父 `Context` 取消后该子 `Context` 也会被取消。`WithTimeout`、`WithDeadline` 内部存在了一个定时器，到时间点后会自动帮我们调用取消函数。 `WithValue`：传入一对 `kv`，返回一个新的 `Context`，新的 `Context` 内部会记录这对 `kv`，供以后查询。

如果我们把一个网络请求的处理看做是一个任务的话，使用 `Context` 帮助我们在子任务树中传递取消任务的信号，并可以根据 `WithXXX` 生成新的子 `Context` 传递到下游子任务中，整体呈现出一个树状结构。上层 `Context` 被取消后会沿着树向下递归的告知所衍生的所有 `Context` 该任务被取消。相反的，子 `Context` 被取消并不会影响到其父 `Context` 是否被取消。另外， `Context` 还可以携带 `kv` 在任务中传递。

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111834.png)

### 2. `Context` 接口

```go
type Context interface {
        Deadline() (deadline time.Time, ok bool)
        Done() <-chan struct{}
        Err() error
        Value(key interface{}) interface{}
}
```

- `Deadline`：获取是否设置了到期时间以及所设置的截止时间。如果有设置过期时间的话，`Context` 会到那个时间点时自动发起取消Context的操作。
- `Done`：返回一个通道，如果通道关闭则代表该 `Context` 已经被取消；如果返回的为 `nil`，则代表该 `Context` 是一个永远不会被取消的 `Context`。
- `Err`：返回该 `Context` 被取消的原因，如果只使用 `Context` 包的 `Context` 类型的话，那么只可能返回 `Canceled` （代表被明确取消）或者 `DeadlineExceeded` （因超时而取消）。
- `Value`：你可能经常看到代码中使用该函数从 `ctx` 中获取一个 `string key` 中的值，即一个 `Context` 内部是可以携带很多 `kv` 的，那这里的 `kv` 是怎么存储的呢？是用的 `map` 么？（答案是可以想象成一个树中的某个节点，每个节点保存了指向父节点的指针，从当前的节点一层层向根节点寻找）。

### 3. `emptyCtx`

`emptyCtx` 是 `int` 类型的重新定义，是一个空的 `Context` 类型，实现了 `Context` 的所有接口。但其不能发送取消信息、没有过期时间、也不能存储任何 `kv`，该类型存在的目的为作为 `Context` 树的根节点。

```go
// An emptyCtx is never canceled, has no values, and has no deadline. It is not
// struct{}, since vars of this type must have distinct addresses.
type emptyCtx int
func (*emptyCtx) Deadline() (deadline time.Time, ok bool) {
   return
}
func (*emptyCtx) Done() <-chan struct{} {
   return nil
}
func (*emptyCtx) Err() error {
   return nil
}
func (*emptyCtx) Value(key interface{}) interface{} {
   return nil
}
var (
   background = new(emptyCtx)
   todo       = new(emptyCtx)
)
func Background() Context {
   return background
}
func TODO() Context {
   return todo
}
```

`Background` 和 `TODO` 有什么区别么？

- 在代码上没有任何区别，都是 `emptyCtx` 类型的值。

- `Background` 主要用于 `main` 函数、初始化以及测试代码中，也经常作为 `Context` 树结构的最顶层

- `TODO` 注释上这样解释，意思是你不知道用啥 `Context`的时候就用它了

  > // TODO returns a non-nil, empty Context. Code should use context.TODO when // it's unclear which Context to use or it is not yet available (because the // surrounding function has not yet been extended to accept a Context // parameter)

### 4. `cancelCtx`

下面进入到 `Context` 核心科技中

#### 4.1 `CancelFunc`

```go
type CancelFunc func()
```

调用函数 `WithCancel`、`WithTimeout`、`WithDeadline`都会返回新的子 `Context` 和 `CancelFunc` 类型的函数，在 `CancelFunc` 函数内部都使用了闭包的方式存储了子 `Context` 的指针，调用 `CancelFunc` 后会将子 `Context` 的内部通道关闭，达到取消该子 `Context` 的效果。

#### 4.2 `cancelCtx`

`cancelCtx` 是真正具有取消功能的 `Context` 类型：

- 包含了一个 `Context` 类型的值，存储了当前 `cancelCtx` 的父 `Context` 的指针。
- `done` 作为取消信号的通道，子协程监听该通道了解到是否需要取消任务。
- `children` 存储了当前 `Context` 衍生的所有可取消类型的子 `Context`。
- `err` 会被第一次取消的时候设置。

```go
type cancelCtx struct {
   Context

   mu       sync.Mutex            // protects following fields
   done     chan struct{}         // created lazily, closed by first cancel call
   children map[canceler]struct{} // set to nil by the first cancel call
   err      error                 // set to non-nil by the first cancel call
}

func (c *cancelCtx) Done() <-chan struct{} {
   c.mu.Lock()
   if c.done == nil {
      c.done = make(chan struct{})
   }
   d := c.done
   c.mu.Unlock()
   return d
}
```

`cancelCtx` 的核心函数为成员函数 `cancel`。首先定义了 `canceler` 接口，另外创建了一个全局的且为关闭的通道 `closedchan` 来复用结束信号（如果子协程在调用 `cancelCtx` 的 `Done` 函数来监听取消通道之前，这个 `cancelCtx` 就已经被取消了，那么没必要再创建一个新的通道了，大家都复用全局的这个已关闭就可以）。

```go
// A canceler is a context type that can be canceled directly. The
// implementations are *cancelCtx and *timerCtx.
type canceler interface {
   cancel(removeFromParent bool, err error)
   Done() <-chan struct{}
}
// closedchan is a reusable closed channel.
var closedchan = make(chan struct{})

func init() {
   close(closedchan)
}
```

前面提出过如果主协程的超时时间到达时，主协程已经发送了取消信号但是其衍生的所有子协程怎么知道的问题，这里的解决办法即对所有的 `children` 进行递归调用取消函数，深度优先遍历。

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111844.png)

```go
// cancel closes c.done, cancels each of c's children, and, if
// removeFromParent is true, removes c from its parent's children.
func (c *cancelCtx) cancel(removeFromParent bool, err error) {
   if err == nil {
      panic("context: internal error: missing cancel error")
   }
   c.mu.Lock()
   if c.err != nil {
      c.mu.Unlock()
      return // already canceled
   }
   c.err = err
   if c.done == nil {
      c.done = closedchan
   } else {
      close(c.done)
   }
   for child := range c.children {
      // NOTE: acquiring the child's lock while holding parent's lock.
      child.cancel(false, err)
   }
   c.children = nil
   c.mu.Unlock()

   if removeFromParent {
      removeChild(c.Context, c)
   }
}
```

如果当前子 `Context` 取消了，其父 `Context` 的 `children` 中也就没有必要再存储该子 `Context` 了，调用了 `removeChild` 实现该部分逻辑：根据存储的父 `Context` 向上一层层的找（由 `parentCancelCtx` 实现），如果父 `Context` 是已知的 `cancelCtx` 和 `timerCtx` 类型就从 `children` 中删除，如果是 `valueCtx` 类型则继续向上层查找其父 `Context`。

```go
// parentCancelCtx follows a chain of parent references until it finds a
// *cancelCtx. This function understands how each of the concrete types in this
// package represents its parent.
// 递归的向上查找cancelCtx类型的节点，找到第一个就停止
func parentCancelCtx(parent Context) (*cancelCtx, bool) {
   for {
      switch c := parent.(type) {
      case *cancelCtx:
         return c, true
      case *timerCtx:
         return &c.cancelCtx, true // timerCtx里面包装了一个cancelCtx类型的节点
      case *valueCtx:
         parent = c.Context // 继续向上找
      default:
         return nil, false
      }
   }
}

// removeChild removes a context from its parent.
func removeChild(parent Context, child canceler) {
   p, ok := parentCancelCtx(parent)
   if !ok {
      return
   }
   p.mu.Lock()
   if p.children != nil {
      delete(p.children, child)
   }
   p.mu.Unlock()
}
```

#### 4.3 `WithCancel`

`WithCancel` 帮助创建一个子 `cancelCtx`，并保证父 `Context` 取消时该新建的子 `cancelCtx` 也能被通知取消。 `propagateCancel` 根据传入的父 `Context` 值沿着树向上查找到 `cancelCtx` 类型的节点，将新建的子 `cancelCtx` 加入到该节点的 `children` 中。如果找不到 `cancelCtx` 类型的节点的话，那么就要新启一个协程等待父 `Context` 被取消的时候明确调用新产生的子 `cancelCtx` 的取消函数，从而将 `parent` 和子 `cancelCtx` 组织成一树形结构。

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111853.png)

```go
func WithCancel(parent Context) (ctx Context, cancel CancelFunc) {
   c := newCancelCtx(parent)
   propagateCancel(parent, &c)

   // true 表示把自己从parent中移除, 因为自己已经cancel掉了, parent没有必要再保存自己
   // Canceled 是一个预定义测错误
   return &c, func() { c.cancel(true, Canceled) }
}

// newCancelCtx returns an initialized cancelCtx.
func newCancelCtx(parent Context) cancelCtx {
   return cancelCtx{Context: parent}
}

// propagateCancel arranges for child to be canceled when parent is.
func propagateCancel(parent Context, child canceler) {
   if parent.Done() == nil {
      return // parent is never canceled
   }
   if p, ok := parentCancelCtx(parent); ok {
      p.mu.Lock()
      if p.err != nil {
         // parent has already been canceled
         child.cancel(false, p.err)
      } else {
         if p.children == nil {
            p.children = make(map[canceler]struct{})
         }
         p.children[child] = struct{}{}
      }
      p.mu.Unlock()
   } else {
      go func() {
         select {
         case <-parent.Done():
            child.cancel(false, parent.Err())
         case <-child.Done():
         }
      }()
   }
}
```

需要注意，如果发现父 `Context` 已经取消了，那么会立刻将当前新产生的子 `Context` 也取消掉。

#### 5. `timerCtx`

`timerCtx` 帮助实现一个超时自动取消的 `Context`，内部仍然使用 `cancelCtx` 实现取消，额外增一个定时器 `Timer` 定时调用 `cancle` 函数实现该功能（ `WithTimeOut` 将当前时间+超时时间计算得到绝对时间后使用 `WithDeadLine` 实现）。 但是如果其父 `Context` 也有超时过期的取消功能，且父 `Context` 的超时时间点在传入的时间点之前，那么就没有必要再使用 `timerCtx` 生成子 `Context` 了，使用 `WithCancel` 就可以了。

```go
// A timerCtx carries a timer and a deadline. It embeds a cancelCtx to
// implement Done and Err. It implements cancel by stopping its timer then
// delegating to cancelCtx.cancel.
type timerCtx struct {
   cancelCtx
   timer *time.Timer // Under cancelCtx.mu.

   deadline time.Time
}
func (c *timerCtx) Deadline() (deadline time.Time, ok bool) {
   return c.deadline, true
}
func (c *timerCtx) cancel(removeFromParent bool, err error) {
   c.cancelCtx.cancel(false, err)
   if removeFromParent {
      // Remove this timerCtx from its parent cancelCtx's children.
      removeChild(c.cancelCtx.Context, c)
   }
   c.mu.Lock()
   if c.timer != nil {
      c.timer.Stop()
      c.timer = nil
   }
   c.mu.Unlock()
}
func WithDeadline(parent Context, d time.Time) (Context, CancelFunc) {
   if cur, ok := parent.Deadline(); ok && cur.Before(d) {
      // The current deadline is already sooner than the new one.
      return WithCancel(parent)
   }
   c := &timerCtx{
      cancelCtx: newCancelCtx(parent),
      deadline:  d,
   }
   propagateCancel(parent, c)
   dur := time.Until(d)
   if dur <= 0 { // 再检查一遍看时间有没有到达
      c.cancel(true, DeadlineExceeded) // deadline has already passed
      return c, func() { c.cancel(false, Canceled) }
   }
   c.mu.Lock()
   defer c.mu.Unlock()
   if c.err == nil {
      c.timer = time.AfterFunc(dur, func() {
         c.cancel(true, DeadlineExceeded)
      })
   }
   return c, func() { c.cancel(true, Canceled) }
}
func WithTimeout(parent Context, timeout time.Duration) (Context, CancelFunc) {
   return WithDeadline(parent, time.Now().Add(timeout))
}
```

![img](https://image-1252109614.cos.ap-beijing.myqcloud.com/img/20200901111858.png)

`timerCtx3` 设定超时时间是5秒，但 `timerCtx4` 设置超时时间为6秒，那么 `timerCtx4` 所在任务会发现设定的6秒超时时间还没到任务就被取消了。所以在设定超时时间的时候，可以先用 `Deadline` 查看一下距离现在时刻还有多少余量时间可用，如果所留时间太少大概率完成不了任务的话就直接返回错误信息。

#### 6. `valueCtx`

`valueCtx` 内部仍然使用 `Context` 存储其父 `Context` 的指针，并用 `interface{}` 存储 `kv`，重定义了 `Value` 函数来对比 `key`，如果当前 `valueCtx` 存储的 `key` 不是要找的话，会沿着树向上一直查找直到根节点，因此只会查询本节点及父节点们的数据，不能查询兄弟节点的数据。

```go
type valueCtx struct {
   Context
   key, val interface{}
}
func (c *valueCtx) Value(key interface{}) interface{} {
   if c.key == key {
      return c.val
   }
   return c.Context.Value(key)
}
func WithValue(parent Context, key, val interface{}) Context {
   if key == nil {
      panic("nil key")
   }
   if !reflectlite.TypeOf(key).Comparable() {
      panic("key is not comparable")
   }
   return &valueCtx{parent, key, val}
}
```

`WithValue` 创建 `context` 节点的过程实际上就是创建链表节点的过程。两个节点的 `key` 值是可以相等的，但它们是两个不同的 `context` 节点。查找的时候，会向上查找到最近一个和 `key` 相等的节点（如果下游覆盖了上游所加入的 `kv`，上游的 `kv` 在下游再也获取不到的了）。所以整体上而言，用 `WithValue` 构造的其实是低效率的链表。

## 实际应用

### `Kite` 框架中的 `RPC` 的超时配置是怎么生效的？

问题：怎么使用 `Context` 来实现“主协程最多给子协程X秒完成任务，若子协程到时还未完成就丢弃该任务，若子协程提前完成任务则主协程也可以立即知道返回上游处理结果”？ 分析： 1. 主协程可以等待 `Context` 的 `Done` 返回的通道关闭，判断 `Err` 的返回值就可以知道任务是提前完成了，还是超时被取消了，使用 `WithTimeout` 产生一个自带定时器的 `Context`。 2. 子协程完成任务后需要立刻通知主协程，这里的通知需要调用步骤1中 `WithTimeout` 返回的取消函数。 3. 子协程完成任务后怎么立刻知道并调用步骤1中产生的取消函数呢？看来还需要在子协程中产生一个与主协程无关的 `Context`，并用 `select` 等待主协程的超时消息通道和子协程的任务完成消息通道，如果发现主协程超时已经到了，那么子协程也可以退出不用再等待任务完成了。

```go
func TestMainWaitChild(t *testing.T) {
   var taskTime = 1

   ctx := context.Background()
   mainCtx, mainCancel := context.WithTimeout(ctx, 2*time.Second)

   go func() {
      childCtx, childCancel := context.WithCancel(context.Background())
      go func() {
         fmt.Println("实际任务开始执行")
         time.Sleep(time.Duration(taskTime) * time.Second)
         childCancel()
         fmt.Println("实际任务执行完毕，关闭子任务")
      }()
      select {
      case <-mainCtx.Done():
         fmt.Println("子协程发现主协程已经不再等待实际任务完成，退出子协程")
         return
      case <-childCtx.Done():
         mainCancel()
         fmt.Println("子协程发现实际任务已完成，告知主协程")
         return
      }
   }()

   <-mainCtx.Done()
   err := mainCtx.Err()
   if err == context.Canceled {
      fmt.Println("主协程ctx正常结束")
   } else if err == context.DeadlineExceeded {
      fmt.Println("主协程ctx发现实际任务超时未完成而取消")
   }
   time.Sleep(1 * time.Second)
}
```

若 `taskTime` =3：

- 实际任务开始执行
- 子协程发现主协程已经不再等待实际任务完成，退出子协程
- 主协程 `ctx` 发现实际任务超时未完成而取消

若 `taskTime` =1：

- 实际任务开始执行
- 实际任务执行完毕，关闭子任务
- 子协程发现实际任务已完成，告知主协程
- 主协程 `ctx` 正常结束

上面的例子使用了两个 `ctx` 完成该需求，相似的，我们在 `MS` 平台上配置上游对下游某个函数的超时时间是 `xxx ms`，你考虑过怎么实现的么？

```go
func RPCTimeoutMW(next endpoint.EndPoint) endpoint.EndPoint {
   return func(ctx context.Context, request interface{}) (interface{}, error) {
      rpcInfo := GetRPCInfo(ctx)
      ctx, cancel := context.WithTimeout(ctx, time.Duration(rpcInfo.RPCTimeout)*time.Millisecond)
      defer cancel()

      var resp interface{}
      var err error
      done := make(chan error, 1)
      go func() {
         defer func() {
            if err := recover(); err != nil {
               const size = 64 << 10
               buf := make([]byte, size)
               buf = buf[:runtime.Stack(buf, false)]

               logs.CtxError(ctx, "KITC: panic: to=%s, toCluster=%s, method=%s, Request: %v, err: %v\n%s",
                  rpcInfo.To, rpcInfo.ToCluster, rpcInfo.Method, request, err, buf)

               done <- fmt.Errorf("KITC: panic, %v\n%s", err, buf)
            }
            close(done)
         }()

         resp, err = next(ctx, request)
      }()

      select {
      case panicErr := <-done:
         if panicErr != nil {
            panic(panicErr.Error()) // throws panic error
         }
         return resp, err
      case <-ctx.Done():
         return nil, makeTimeoutErr(rpcInfo)
      }
   }
}
```

### 将 `Context` 放在传递消息的结构中

我们看到 `Context` 在实际使用中大多情况放在函数的第一参数中，也是作者提倡的用法。 可以放在结构体中传递么？ 当你需要将 `Context` 放在一个纯粹用作通过通道传递的消息结构中也是可以的，如 `net/http` 包的 `Client` 请求一个 `url` 的实现就可以使用 `req = req.WithContext(ctx)` 将 `Context` 传入到 `req` 中。下面是另一个将 `Context` 放在请求中的例子，摘自 https://www.golangtraining.in/lessons/http/context-http.html

```go
package main
 
import "fmt"
import "context"
 
// A message processes parameter and returns the result on responseChan.
// ctx is places in a struct, but this is ok to do.
type message struct {
        responseChan chan<- int
        parameter    string
        ctx          context.Context
}
 
func ProcessMessages(work <-chan message) {
        for job := range work {
                select {
                // If the context is finished, don't bother processing the
                // message.
                case <-job.ctx.Done():
                        continue
                default:
                }
                // Assume this takes a long time to calculate
                hardToCalculate := len(job.parameter)
                select {
                case <-job.ctx.Done():
                case job.responseChan <- hardToCalculate:
                }
        }
}
 
func newRequest(ctx context.Context, input string, q chan<- message) {
        r := make(chan int)
        select {
        // If the context finishes before we can send msg onto q,
        // exit early
        case <-ctx.Done():
                fmt.Println("Context ended before q could see message")
                return
        case q <- message{
                responseChan: r,
                parameter:    input,
                // We are placing a context in a struct.  This is ok since it
                // is only stored as a passed message and we want q to know
                // when it can discard this message
                ctx: ctx,
        }:
        }
 
        select {
        case out := <-r:
                fmt.Printf("The len of %s is %d\n", input, out)
        // If the context finishes before we could get the result, exit early
        case <-ctx.Done():
                fmt.Println("Context ended before q could process message")
        }
}
 
func main() {
        q := make(chan message)
        go ProcessMessages(q)
        ctx := context.Background()
        newRequest(ctx, "hello", q)
        newRequest(ctx, "world", q)
        close(q)
}
```

> The one exception to not storing a context is when you need to put it in a struct that is used purely as a message that is passed across a channel. In this example, we break this general rule of not storing Context by putting it in a message. However, this is an appropriate use of Context because it still flows through the program, but along a channel rather than a stack trace. Also notice here how the Context is used in four places:
>
> - To time out q in case the processor is too full
> - To let q know if it should even process message
> - To time out q sending the message back to newRequest()
> - To time out newRequest() waiting for a response back from ProcessMessage

一般情况下为什么不要把 `Context` 放在结构体中呢？ 如果包含 `Context` 结构体的实例仅只为处理某一次请求而存在，完成任务后就会被销毁的话，那么问题不大，但如果该结构体的值的生命周期贯穿多个任务那么就要小心了，例如： 上游 `Context` 的取消会递归取消下游所有衍生的 `Context`，如果结构体中包含的 `Context` 在第一次任务的执行过程中被上游取消了，那么之后涉及该结构体值的第二次、第三次任务调用时，检查 `Context` 时都会认为该次任务被取消而直接返回错误。 可以参考这篇文章：[Go Context的踩坑经历](https://zhuanlan.zhihu.com/p/34417106)

## 总结

### `Context` 的设计理念

> - Cancelation should be advisory 取消操作应该是建议性质的，调用者并不知道被调用者内部实现，调用者不应该 `interrupt/panic` 被调用者。 调用者应该通知被调用者处理不再必要，被调用者来决定如何处理后续操作。 实现：调用者和被调用者之间利用一个单向 `channel` 来实现取消信息的传递，调用者发送取消信号(`close`)，被调用者通过监听此信号，来捕获到取消操作。
> - Cancelation should be transitive 取消操作应该被传播。 实现：`Context` 是线程安全的，可以传递给多个被调用者，`channel` 的 `close` 信号是广播性质的；另外 `Context` 在组织上实现了父子关系的存储，取消操作会自动向下传播。

### 最佳实践

> 1. 所有的长耗时、可能阻塞的任务都需要 `Context`
> 2. 不要把 `Context` 放在结构体中，要以参数的方式传递（也有例外，如 `http.Request` 中的 `request` 的实现就将 `Context` 放入了结构体中）
> 3. 以 `Context` 作为参数的函数方法，应该把 `Context` 作为第一个参数，放在第一位。
> 4. 给一个函数方法传递 `Context` 的时候，不要传递 `nil`，如果不知道传递什么，就使用 `context.TODO`
> 5. `Context` 的 `Value` 相关方法应该传递必须的数据，不要什么数据都使用这个传递
> 6. 不要试图在 `Context.Value` 里存某个可变更的值，然后改变
> 7. `Context` 是线程安全的，可以放心的在多个 `goroutine` 中传递
> 8. 要养成关闭 `Context` 的习惯，特别是超时的 `Context`
> 9. `Context` 应该随 `Request` 消亡而消亡


