## Mellanox RDMA文档中的样例 (7.2 Code for Send, Receive, RDMA Read, RDMA Write)
编译库的需求：`libibverbs `
编译参数:GCC <文件名>  -o service  -libverbs
运行方式：
1. 有IB网络支持：
       服务端：./service
       客户端：./service 服务端IP
 2. 走ROCE:
       服务端：./service   -g  0
       客户端：./service -g 0  服务端IP

## Based on : https://github.com/fruitdish/RDMA-EXAMPLE
