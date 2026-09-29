# cpp-todo

A tiny command-line Todo List in C++.

零第三方依赖，全部代码放在一个文件里，适合通读源码学习。

## 功能

- 添加任务、查看清单
- 标记完成 / 取消完成
- 删除单个任务、一键清理已完成
- 退出时自动保存到 `todos.txt`，下次打开自动读回来
- `todos.txt` 是纯文本，可以自己用记事本改

## 环境要求

只需要一个支持 C++17 的编译器（本机已装 MinGW-w64 的 `g++` 16.2.0）。

先在 PowerShell 里确认一下：

```powershell
g++ --version
```

能打印出版本号就可以继续。

## 运行方式

### 方式一：一行命令（最快）

在项目根目录打开 PowerShell：

```powershell
g++ -std=c++17 -Wall -O2 src/main.cpp -o todo.exe
.\todo.exe
```

### 方式二：CMake

```powershell
cmake -S . -B build -G Ninja
cmake --build build
.\build\todo.exe
```

> `todos.txt` 会生成在**运行程序时所在的目录**。上面两种方式都从项目根目录启动，所以数据文件就在根目录。

## 怎么用

启动后先显示当前清单，再显示菜单，输入数字回车即可：

```
=== cpp-todo ===
  Nothing to do yet - add your first task!

  1) add        2) list       3) done
  4) undo       5) remove     6) clear done
  0) quit
> 1
  task: buy milk
  1. [ ] buy milk
  1 left, 0 done
```

- `1) add`：输入任务内容后回车，内容可以是中文。
- `3) done` / `4) undo`：先输入任务编号，再回车。
- `6) clear done`：一次性删掉所有已完成的任务。
- `0) quit`：退出并保存；也可以按 `Ctrl+Z` 再回车退出。
- 输错不会退出程序，只会提示你重新输入。

## 数据格式

`todos.txt` 一行一个任务，`|` 前面是状态（`0` 未完成，`1` 已完成）：

```
0|buy milk
1|write code
```

这个文件是你自己的数据，不会被提交到 Git。

## 项目结构

```
cpp-todo/
├── src/main.cpp      # 全部代码
├── CMakeLists.txt    # CMake 构建脚本
├── .gitignore
└── README.md
```