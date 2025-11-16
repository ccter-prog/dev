# 俄罗斯方块游戏 (Tetris)

一个使用 C++ 和 SFML 开发的经典俄罗斯方块游戏。

## 🎮 特性

- 经典的俄罗斯方块游戏玩法
- 动态加速系统（每消除5行加速一次）
- 最高分记录系统
- 自定义背景音乐支持
- 实时游戏状态显示

## 🛠️ 技术栈

- **编译器**: LLVM/Clang
- **图形库**: SFML 3.0.2 (动态链接)
- **构建系统**: CMake
- **C++标准**: C++17

## 📦 构建说明

### 环境要求
- LLVM-MinGW 工具链
- CMake 3.15+
- SFML 3.0.2

### 构建命令

- cmake -S . -B build_debug -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
- cmake -S . -B build_release -G "Ninja" -DCMAKE_BUILD_TYPE=Release

## 🎵 可以切换背景音乐

- 将你喜欢的音乐放在：可执行程序目录/assets/music/
- 音频需为flac/ogg/mp3/wav

## 🎯 游戏控制

- ← →    左右移动方块
- ↑      旋转方块
- ↓      加速下落
- space  硬降（立即落底）
- R      重新开始游戏
- X      暂停/继续背景音乐

## 🎮 游戏规则

- 消除一行获得相应分数
- 每消除5行提升一个等级，游戏速度加快
- 游戏结束条件：新方块触顶
- 最高分自动保存