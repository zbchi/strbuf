## StrbufTest

[![CC BY SA 4.0](https://img.shields.io/github/license/XUPTLinuxGroup2020/Favorites?style=flat-square)](https://creativecommons.org/licenses/by-sa/4.0/)

本仓库用于本地测试 Strbuf 任务

## 环境搭建
Cmake 的版本应大于等于 3.14

Ubuntu 用户安装 Cmake
```sh
sudo apt install cmake
```
Arch 系用户安装 Cmake
```sh
yay cmake
```
---

fork 该仓库到本地
```sh
git clone git@github.com:xiyou-linuxer/StrbufTest.git 
```
---

如果出现 `VS Code` 找不到 gtest 头文件的问题
在 `VS Code` 的 `settings.json` 中加入
```
"configurationProvider": "vector-of-bool.cmake-tools",
```

## 测试流程

进入 strbuf 目录
```sh
cd StrbufTest/strbuf
```
创建测试代码 

例如：
张三若想在 `strbuf` 这项练习中测试代码，则需在 `strbuf` 目录下创建 `ZhangSan.c`
```
strbuf
├── FileTest
├── strbuf.h
├── test.cpp
└── ZhangSan.c
```
测试代码
```sh
./test.sh
```
当出现下面的字样时，说明你已经通过了所有的测试点
```
[----------] Global test environment tear-down
[==========] 38 tests from 5 test suites ran. (1 ms total)
[  PASSED  ] 38 tests.

```

### LICENSE

本项目采用[知识共享署名-相同方式共享 4.0 国际许可协议](https://creativecommons.org/licenses/by-sa/4.0/)进行许可．
贡献本仓库视为同意贡献内容基于上述协议授权．

![知识共享许可协议](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)
