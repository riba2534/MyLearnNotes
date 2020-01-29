# learn-vue

vue.js 学习过程

视频来自于：https://www.bilibili.com/video/av76249419/

- 03
  - 普通渲染
- 04
  - el 挂载点
- 05
  - data 数据对象
- 07
  - v-text 指令
- 08
  - v-html 指令
- 09
  - v-on 绑定事件
  - 主要用于绑定按钮点击，写法一般为：`<button v-on:click="jiafen">加分</button>`
  - 可以用`<button @click="jianfen">减分</button>` 代替
  - 绑定键盘回车事件:`<input type="text" v-on:keyup.enter="onEnter" v-model="secondCount">`
- 10
  - 计时器实现
  - 创建 vue 实例时：el（挂载点）,data（数据）,methods（方法）
  - v-on 指令的作用是绑定时间，简写为`@`
  - 方法中通过`this`，关键字获取`data`中的数据
  - `v-text`指令的作用是，设置文本值，简写为`{{}}`
- 11
  - `v-show` 根据表达式的值的真假，切换元素的显示与隐藏
- 12
  - `v-if`指令
  - 根据表达式真假切换元素的显示状态
  - 本质是通过操作 DOM 的方式
- 13
  - `v-bind`指令
  - 为元素绑定属性
  - 完整写法：`v-bind: 属性名`
  - 简写写法：`: 属性名`
  - 需要动态的增删 class 建议使用对象的方式
- 14
  - 图片切换样例
- 15
  - `v-for`循环
- 16
  - `v-on`补充
  - 事件绑定的方法写成函数调用的形式，可以传入自定义参数
  - 定义方法时需要定义形参来接收传入的实参
  - 事件后面跟上`. 修饰符`可以对事件进行限制
  - `.enter`可以触发的按键为回车
- 17
  - `v-model`指令，绑定元素
- 18
  - 小黑记事本