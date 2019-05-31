## http 包使用

一个简单的web服务器

```go
package main

import "net/http"

func main() {
	http.ListenAndServe(":8080", http.FileServer(http.Dir(".")))
}

```
