## StrbufTest

## 环境搭建
cmake的版本应大于等于3.14

ubuntu用户安装cmake
```sh
sudo apt install cmake
```
arch系用户安装cmake
```sh
yay cmake
```
---

fork 该仓库并 clone 该 fork 仓库到本地
```sh
git clone git@github.com:[用户名]/[fork 仓库名].git 
```
---

如果出现vscode找不到gtest头文件的问题
在vscode的`settings.json`中加入
```
"configurationProvider": "vector-of-bool.cmake-tools",
```

## 测试流程

进入strbuf目录
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
