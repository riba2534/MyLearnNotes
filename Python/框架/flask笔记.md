# Flask 笔记

## 1. 入门操作

### 1.1 小 demo

```python
from flask import Flask
app = Flask(__name__)
@app.route('/')
def index():
    return 'Index Page'
@app.route('/hello')
def hello():
    return 'Hello, World'
```

以上是 Flask 的一个入门例子，分为以下几个步骤：

1. 首先导入了 Flask 类，该类的实例会成为一个 wsgi 应用
2. 装饰器表明了路由，告诉 flask 的路径
3. 函数名称用于生成相关的 url ，函数返回要在用户的浏览器上显示的信息

### 1.2 运行过程

#### 1.2.1 方法一(推荐)

1. 在环境变量中，需要保持一个变量名叫做 `FLASK_APP` ，这个名称为要执行的 `.py` 文件的名称，否则会报错
2. 然后执行命令行指令 `flask run` 

#### 1.2.2 方法二(不推荐，老的做法)

在代码添加：

```python
if __name__ == "__main__":
    app.run()
```

然后直接使用 `python3 名称` 即可，但是这种做法是老的做法，不推荐。

### 1.3 注意事项

1. 默认运行的时候，flask 框架是以调试模式运行的，所以不安全，运行时默认只有本地的开发机可以访问，而其他机器访问这个IP地址的时候则不能访问，如果想让所有的人都可以访问，那么 `flask run --host=0.0.0.0` 就可以了。
2. `FLASK_APP` 环境变量中储存的是模块的名称，运行 **flask run** 命令就 会导入这个模块。如果模块的名称不对，那么就会出现导入错误。出现错误的时机是在 应用开始的时候。如果调试模式打开的情况下，会在运行到应用开始的时候出现导入 错误。

### 1.4 构造 url

#### 变量规则

通过把 URL 的一部分标记为 `<variable_name>` 就可以在 URL 中添加变量。标记的 部分会作为关键字参数传递给函数。通过使用 `<converter:variable_name>` ，可以 选择性的加上一个转换器，为变量指定规则。请看下面的例子:

```python
@app.route('/user/<username>')
def show_user_profile(username):
    # show the user profile for that user
    return 'User %s' % username

@app.route('/post/<int:post_id>')
def show_post(post_id):
    # show the post with the given id, the id is an integer
    return 'Post %d' % post_id

@app.route('/path/<path:subpath>')
def show_subpath(subpath):
    # show the subpath after /path/
    return 'Subpath %s' % subpath
```

转换器类型：

| `string` | （缺省值） 接受任何不包含斜杠的文本 |
| -------- | ----------------------------------- |
| `int`    | 接受正整数                          |
| `float`  | 接受正浮点数                        |
| `path`   | 类似 `string` ，但可以包含斜杠      |
| `uuid`   | 接受 UUID 字符串                    |

#### 唯一的 URL / 重定向行为

以下两条规则的不同之处在于是否使用尾部的斜杠。:

```python
@app.route('/projects/')
def projects():
    return 'The project page'

@app.route('/about')
def about():
    return 'The about page'
```

`projects` 的 URL 是中规中举的，尾部有一个斜杠，看起来就如同一个文件夹。 访问一个没有斜杠结尾的 URL 时 Flask 会自动进行重定向，帮你在尾部加上一个斜杠。

`about` 的 URL 没有尾部斜杠，因此其行为表现与一个文件类似。如果访问这个 URL 时添加了尾部斜杠就会得到一个 404 错误。这样可以保持 URL 唯一，并帮助 搜索引擎避免重复索引同一页面。

#### URL 构建

[`url_for()`](https://dormousehole.readthedocs.io/en/latest/api.html#flask.url_for) 函数用于构建指定函数的 URL。它把函数名称作为第一个 参数。它可以接受任意个关键字参数，每个关键字参数对应 URL 中的变量。未知变量 将添加到 URL 中作为查询参数。

为什么不在把 URL 写死在模板中，而要使用反转函数 [`url_for()`](https://dormousehole.readthedocs.io/en/latest/api.html#flask.url_for) 动态构建？

1. 反转通常比硬编码 URL 的描述性更好。
2. 你可以只在一个地方改变 URL ，而不用到处乱找。
3. URL 创建会为你处理特殊字符的转义和 Unicode 数据，比较直观。
4. 生产的路径总是绝对路径，可以避免相对路径产生副作用。
5. 如果你的应用是放在 URL 根路径之外的地方（如在 `/myapplication` 中，不在 `/` 中）， [`url_for()`](https://dormousehole.readthedocs.io/en/latest/api.html#flask.url_for) 会为你妥善处理。

例如，这里我们使用 [`test_request_context()`](https://dormousehole.readthedocs.io/en/latest/api.html#flask.Flask.test_request_context) 方法来尝试使用 [`url_for()`](https://dormousehole.readthedocs.io/en/latest/api.html#flask.url_for) 。 [`test_request_context()`](https://dormousehole.readthedocs.io/en/latest/api.html#flask.Flask.test_request_context) 告诉 Flask 正在处理一个请求，而实际上也许我们正处在交互 Python shell 之中， 并没有真正的请求。参见 [本地环境](https://dormousehole.readthedocs.io/en/latest/quickstart.html#context-locals) 。

```python
from flask import Flask, url_for
app = Flask(__name__)
@app.route('/')
def index():
    return 'index'
@app.route('/login')
def login():
    return 'login'
@app.route('/user/<username>')
def profile(username):
    return '{}\'s profile'.format(username)
with app.test_request_context():
    print(url_for('index'))
    print(url_for('login'))
    print(url_for('login', next='/'))
    print(url_for('profile', username='John Doe'))
```

#### 静态文件

动态的 web 应用也需要静态文件，一般是 CSS 和 JavaScript 文件。理想情况下你的 服务器已经配置好了为你的提供静态文件的服务。但是在开发过程中， Flask 也能做好 这项工作。只要在你的包或模块旁边创建一个名为 `static`的文件夹就行了。 静态文件位于应用的 `/static` 中。

使用特定的 `'static'` 端点就可以生成相应的 URL

```
url_for('static', filename='style.css')
```

这个静态文件在文件系统中的位置应该是 `static/style.css` 。