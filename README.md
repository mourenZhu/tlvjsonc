# LibTLV
This is a TCP-based application layer protocol - TLV library, which is written in C language and based on the C11 standard. TLVLib is Type-Length-Value Library.

| type(char *) | length(size_t) | value(void *) |  
|:-------------|---------------:|--------------:|
| "type\r\n"   |         length |         value |

# build & install
```shell
cd build
cmake ..
make
make install
```

# example
- [simple_server_client](example/simple_server_client)

# License
The MIT license