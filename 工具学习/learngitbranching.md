# LearnGitBranching笔记

由于之前学习的 Git 只会进行一些简单操作，没有完全理解，所以重新学习。

学习资料来源于：[learngitbranching](https://learngitbranching.js.org/)

把 Git 理解成一个 DAG(有向无环图)，git 的历史是一个 DAG ，每一个节点代表一个 commit ,每一次提交都在修改这个 DAG ，而 branch 和 tag 仅仅是指向 DAG 这个节点的一个指针。

## 1 主要

### 1.1 基础篇

#### 1.1.1 第 1 关

> Git Commit
> Git 仓库中的提交记录保存的是你的目录下所有文件的快照，就像是把整个目录复制，然后再粘贴一样，但比复制粘贴优雅许多！
>
> Git 希望提交记录尽可能地轻量，因此在你每次进行提交时，它并不会盲目地复制整个目录。条件允许的情况下，它会将当前版本与仓库中的上一个版本进行对比，并把所有的差异打包到一起作为一个提交记录。
>
> Git 还保存了提交的历史记录。这也是为什么大多数提交记录的上面都有父节点的原因 —— 我们会在图示中用箭头来表示这种关系。对于项目组的成员来说，维护提交历史对大家都有好处。
>
> 关于提交记录太深入的东西咱们就不再继续探讨了，现在你可以把提交记录看作是项目的快照。提交记录非常轻量，可以快速地在这些提交记录之间切换！

**过关方法**：

```bash
git commit
git commit
```

**记录**：

```bash
git commit -m "消息"  #创建一个新的提交记录
```

#### 1.1.2 第 2 关

> Git Branch
> Git 的分支也非常轻量。它们只是简单地指向某个提交纪录 —— 仅此而已。所以许多 Git 爱好者传颂：
>
> 早建分支！多用分支！
>
> 这是因为即使创建再多分的支也不会造成储存或内存上的开销，并且按逻辑分解工作到不同的分支要比维护那些特别臃肿的分支简单多了。
>
> 在将分支和提交记录结合起来后，我们会看到两者如何协作。现在只要记住使用分支其实就相当于在说：“我想基于这个提交以及它所有的父提交进行新的工作。”

**过关方法**：

```bash
git branch bugFix
git checkout bugFix
```

**记录**：

```bash
git branch newImage # 创建一个名字为 newImage 的分支
# 在新的分支中可以继续使用 git commit 提交记录
git checkout name # 切换到名字为 name 的git分支上,所以在对应分支提交内容前先切换分支
git checkout -b name #创建一个名字为 name 的分支且切换到 name 分支上
```

#### 1.1.3 第 3 关

> 分支与合并
> 太好了! 我们已经知道如何提交以及如何使用分支了。接下来咱们看看如何将两个分支合并到一起。就是说我们新建一个分支，在其上开发某个新功能，开发完成后再合并回主线。
>
> 咱们先来看一下第一种方法 —— git merge。在 Git 中合并两个分支时会产生一个特殊的提交记录，它有两个父节点。翻译成自然语言相当于：“我要把这两个父节点本身及它们所有的祖先都包含进来。”

**过关方法**：

先创建分支 `bugFix` 切过去提交一下，然后切回 `master` 在 master 中提交一次，然后把 `bugFix` 合并到 `master`

```bash
git branch bugFix
git checkout bugFix
git commit
git checkout master
git commit
git merge bugFix # 因此此时在 master 分支，所以就是把 bufFix 和 master 分支合并
```

**记录**：

```bash
git merge name # 把 name 分支与当前分支合并
```

#### 1.1.4 第 4 关

> Git Rebase
> 第二种合并分支的方法是 git rebase。Rebase 实际上就是取出一系列的提交记录，“复制”它们，然后在另外一个地方逐个的放下去。
>
> Rebase 的优势就是可以创造更线性的提交历史，这听上去有些难以理解。如果只允许使用 Rebase 的话，代码库的提交历史将会变得异常清晰。

分支状态本来为：

![](https://i.loli.net/2019/04/23/5cbf14b61e73f.png)

当使用：

```bash
git rebase master
```

后使得 bugFix 节点变成了 master 节点的儿子节点

![](https://i.loli.net/2019/04/23/5cbf14b61cc71.png)



此时，C3 节点仍然存在，而 C3' 是 master 上面的 C3 的副本， 然后再更新 master 节点即可。

现在处于 `master` 分支，只需要用 `git rebase bugFix` 更新 master 节点即可。

**过关方法**：

先新建并切换到 `bugFix` 分支没，提交一次，然后再切换回 master 提交一次，再次切换到 bugFix 分支上，rebase 到 master 分支上

```bash
git checkout -b bugFix
git commit
git checkout master
git commit
git checkout bugFix
git rebase master
```

**记录**：

```bash
git rebase name # 给当前节点创建一个副本，并且接到 name 节点的儿子节点
```

### 1.2 高级篇

#### 1.2.1 第 1 关

> 在提交树上移动
> 在接触 Git 更高级功能之前，我们有必要先学习在你项目的提交树上前后移动的几种方法。
>
> 一旦熟悉了如何在 Git 提交树上移动，你驾驭其它命令的能力也将水涨船高！
>
> HEAD
> 我们首先看一下 “HEAD”。 HEAD 是一个对当前检出记录的符号引用 —— 也就是指向你正在其基础上进行工作的提交记录。
>
> HEAD 总是指向当前分支上最近一次提交记录。大多数修改提交树的 Git 命令都是从改变 HEAD 的指向开始的。
>
> HEAD 通常情况下是指向分支名的（如 bugFix）。在你提交时，改变了 bugFix 的状态，这一变化通过 HEAD 变得可见。

![](https://i.loli.net/2019/04/23/5cbf19e2f3898.png)

**过关方法**：

从 bugFix 分支中分离出 HEAD 并让其指向一个提交记录，提交记录用哈希值来表示

```bash
git checkout C4
```

**记录**：

- HEAD 指向当前分支最近的一次提交记录
- 用 `git checkout ` 可以在提交树上移动

#### 1.2.2 第 2 关

> 相对引用
> 通过指定提交记录哈希值的方式在 Git 中移动不太方便。在实际应用时，并没有像本程序中这么漂亮的可视化提交树供你参考，所以你就不得不用 git log 来查查看提交记录的哈希值。
>
> 并且哈希值在真实的 Git 世界中也会更长（译者注：基于 SHA-1，共 40 位）。例如前一关的介绍中的提交记录的哈希值可能是 fed2da64c0efc5293610bdd892f82a58e8cbc5d8。舌头都快打结了吧...
>
> 比较令人欣慰的是，Git 对哈希的处理很智能。你只需要提供能够唯一标识提交记录的前几个字符即可。因此我可以仅输入fed2 而不是上面的一长串字符。

**过关过程**：

切换到 `bugFix` 的父节点。

```bash
git checkout bugFix
git checkout HAED^
```

**记录**：

- 使用 `^` 向上移动一个提交记录
- 使用 `~<num>` 向上移动多个提交记录，如 `-3`

```bash
git checkout HAED^ # 切换成父节点
git checkout HAED^^ # 切换成父节点的父节点
```

#### 1.2.3 第 3 关

> “~”操作符 相对引用
> 如果你想在提交树中向上移动很多步的话，敲那么多 ^ 貌似也挺烦人的，Git 当然也考虑到了这一点，于是又引入了操作符 ~。
>
> 该操作符后面可以跟一个数字（可选，不跟数字时与 ^ 相同，向上移动一次），指定向上移动多少次。咱们还是通过实际操作看一下吧

**过关过程**：

把对应的节点移到对应的位置。

```bash
git checkout C6
git branch -f master HEAD
git branch -f bugFix HEAD~4
git checkout HEAD~3
```

**记录**：

- 使用 `-f` 选项让分支指向另一个提交

  ```bash
  git branch -f master HEAD~3
  ```

  上面的命令会使 master 分支强制指向 HEAD 的第 3 级父亲节点。

#### 1.2.4 第 4 关



### 1.3 移动提交记录



### 1.4 杂项



### 1.5 高级话题



## 2 远程


### 2.1 Push & Pull — Git远程仓库



### 2.2 关于 origin 和它的周边 —— Git 远程仓库高级操作

