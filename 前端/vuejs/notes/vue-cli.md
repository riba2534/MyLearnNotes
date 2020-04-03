# vue-cli

- [Vue.js官网](https://cn.vuejs.org/)
- [Vue-cli文档](https://cli.vuejs.org/zh/)
- [饿了么前端框架](https://element.eleme.cn/#/zh-CN)
- [橙卡视频](https://www.bilibili.com/video/BV1vx411975P)
- [Vue-router](https://router.vuejs.org/zh/)

## 安装

```
npm install -g @vue/cli
```

## 使用

创建项目：

```
vue create my-project
# OR
vue ui
```

## 添加模块

```
vue add element
vue add ......
```

## 使用 vue-router

如果要使用router功能，需要在其中加入标签`<router-view></router-view>`：

```vue
<template>
  <div id="app">
    <navbar></navbar>
    <router-view></router-view>
  </div>
</template>

<script>
import navbar from "./components/navbar.vue";

export default {
  name: "app",
  components: {
    navbar
  }
};
</script>
<style>
</style>
```

## 大致流程

1. `index.html`是主页面
2. vue渲染其中`id=app`的模块
3. 然后去看 `App.vue`
4. 然后去看各种组件的组合。
5. vue是组件式的。
6. 通过add可以添加其他库
7. `package.json`中包括各种命令生成。
8. 运行一个npm项目，基本`npm install`和看`package.json`就行了