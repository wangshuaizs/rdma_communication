# Mellanox RDMA文档中的样例
编译库的需求：`libibverbs `
编译: `make`
运行方式：
 1. 走ROCE:
       服务端：./server -g 2 -d mlx4_0 -i 2 -s 2
       客户端：./client -g 2 -d mlx4_0 -i 2 -s 2 <服务端IP>
