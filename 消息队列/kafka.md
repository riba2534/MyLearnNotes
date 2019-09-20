# Kafka 

## 简介

> Apache Kafka is a distributed high throughput message system
>
> Apache Kafka is a distributed streaming platform

Apache Kafka是一个分布式发布 - 订阅消息系统和一个强大的队列，可以处理大量的数据，并使您能够将消息从一个端点传递到另一个端点。 Kafka适合离线和在线消息消费。 Kafka消息保留在磁盘上，并在群集内复制以防止数据丢失。 Kafka构建在ZooKeeper同步服务之上。 它与Apache Storm和Spark非常好地集成，用于实时流式数据分析。

## 设计理念

- 低延迟：以时间复杂度 O(1) 的方式提供消息处理能力，即使对 TB 以上的数据也能保证常数时间复杂度的访问性能
- 高吞吐：即使在非常廉价的商用机器上也能做到单机支持每秒十万条以上的消息传输
- 水平扩展：支持 Kafka Broker 间的消息分区，及分布式消费，同时支持在线水平扩展
- 顺序性：保证每个 Partition 内的消息顺序传输
- 多场景：同时支持离线数据处理和实时数据处理

