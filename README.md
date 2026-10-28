# 奶蛙

Linux 内核模块，使用内核 Kbuild 构建，不作为用户态可执行程序链接。
内核代码使用 `linux/*` 头文件，不引入 libc 的 `stdio.h`、`sys/types.h`。
头文件路径、配置和编译选项由 Kbuild 提供，无需手动链接用户态库。

## 构建

安装目标内核对应的开发头文件、GNU Make 和内核构建所需的编译工具链后运行：

```sh
make
# 指定其他已准备好的内核构建目录：
make KDIR=/path/to/kernel/build
make clean
```

目标产物为 `naiwa_n.ko`。Makefile 不加载或安装内核模块。

CMake/CLion 的 `naiwa_n` 构建目标也会调用同一套 Makefile：

```sh
cmake -S . -B build
cmake --build build
# 指定目标内核时，在配置命令中添加 -DKDIR=/path/to/kernel/build
```

CLion 直接打开本项目并加载 CMake。`kernel_index` 为代码分析提供内核头文件、
配置宏和 GNU C 扩展，不参与默认构建；实际模块构建仍选择 `naiwa_n`。
修改 `KDIR` 或升级内核后，重新加载 CMake。跨架构索引可设置
`-DKERNEL_ARCH=arm64` 等内核架构目录名。

编译成功后，可运行 `make compile_commands` 生成 IDE 使用的
`compile_commands.json`（需要 Python 3 和内核提供的
`scripts/clang-tools/gen_compile_commands.py`）。在支持编译数据库的 IDE 中
打开该文件，以获取实际的内核头文件搜索路径和宏定义。

## 设备行为

模块加载后创建 `/dev/naiwa`。读取时返回固定 ASCII 图案，支持分段读取，
读完后返回 EOF，不包含字符串末尾的 NUL 字节。
写入数据直接丢弃并返回写入长度，不修改图案或文件偏移。

## 发布版本

GitHub Releases 提供已编译的 `naiwa_n.ko`。预编译模块仅适用于发布说明中
注明的目标内核和架构；其他环境请使用对应内核的构建目录自行编译。

## 许可证

Copyright (C) 2026 Net Salmon

本项目采用 GNU General Public License v3.0（GPL-3.0-only），
许可全文见 [LICENSE](LICENSE)。
