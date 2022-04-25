# HomeManager

一个嵌入式相关的实训课程项目，智能家居系统。

## 编译

### 环境需求

- linux
- gcc
- cmake >= 3.16
- ninja (可选)

### 编译源码

```shell
# 获取源码
git clone https://github.com/Hami-Lemon/HomeManager.git
cd HomeManager
mkdir build && cd build
# 编译
cmake ..
make install

# 使用ninja构建
cmake -G Ninja ..
ninja install

# 交叉编译arm平台（需要arm-linux-gnueabi-gcc）
cmake -G Ninja -DARM_LINUX=ON ..
ninja install
```