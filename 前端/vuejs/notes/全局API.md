# 全局API

## 概述

全局API并不在构造器里，而是先声明全局变量或者直接在Vue上定义一些新功能，Vue内置了一些全局API，比如我们今天要学习的指令Vue.directive。说的简单些就是，在构造器外部用Vue提供给我们的API函数来定义新的功能。

## `Vue.directive`

可以用来自定义指令。

标签功能为自己定义的标签功能。

自定义指令中3个需要传递的参数：

- el: 指令所绑定的元素，可以用来直接操作DOM。
- binding: 一个对象，包含指令的很多信息。
- vnode: Vue编译生成的虚拟节点

自定义指令的生命周期：

自定义指令有五个生命周期（也叫钩子函数），分别是 bind,inserted,update,componentUpdated,unbind

1. bind:只调用一次，指令第一次绑定到元素时调用，用这个钩子函数可以定义一个绑定时执行一次的初始化动作。
2. inserted:被绑定元素插入父节点时调用（父节点存在即可调用，不必存在于document中）。
3. update:被绑定于元素所在的模板更新时调用，而无论绑定值是否变化。通过比较更新前后的绑定值，可以忽略不必要的模板更新。
4. componentUpdated:被绑定元素所在模板完成一次更新周期时调用。
5. unbind:只调用一次，指令与元素解绑时调用。

代码见：[vue.directive.html](code/vue.directive.html)

## `Vue.extended` 扩展实例构造器

代码：[vue.extend.html](code/vue.extend.html)

可以自己定义自己的标签，然后渲染出来，具体看代码。

## `Vue.set`修改值

代码：[vue.set.html](code/vue.set.html)

使用 Vue.set 可以改变某一个元素的值

## Vue的生命周期

一个很经典的图，可以描述vue的生命周期：

![](https://cn.vuejs.org/images/lifecycle.png)

上图描述了vue的生命周期

具体看代码的例子：[vue.life.html](code/vue.life.html)



## `template`模板

代码：[template.html](code/template.html)

模板的几种写法：

1. 直接利用Vue中的template，直接写 html
2. 利用id选择器，写选择的标签
3. 利用script标签进行挂载

## `component`自定义组件

代码：

1. [component-1.html](code/component-1.html)

分类：

1. 全局template

   ```javascript
   Vue.component("标签名称",{template:``})
   ```

2. 局部template

   ```javascript
   var app = new Vue({
       el: "#app",
       components: {
           "panda": {
               template: `<div style="color: green;">局部panda组件</div>`
           }
       }
   })
   ```

### props

代码：[component-2.html](code/component-2.html)

使用`props`可以增加标签选项，但是`Vue.js`的限制，中间不能带`-`，这样会失效，需要把`-`换成小驼峰写法。

### Component父子组件关系

component的嵌套，在一个component中嵌套另一个。

代码：[component-3.html](code/component-3.html)

### Component 标签

使用:

```javascript
<component :is="who"></component>
```

可以指定组件.

代码：[component-4.html](code/component-4.html)



## 参考资料

1. [Vue2.0视频教程-全局API](https://jspang.com/detailed?id=22#toc218)