# "一生一芯"工程项目

这是"一生一芯"的工程项目. 通过运行
```bash
bash init.sh subproject-name
```
进行初始化, 具体请参考[实验讲义][lecture note].

[lecture note]: https://ysyx.oscc.cc/docs/

# 项目说明

## NEMU

nemu是一个RISCV32指令集模拟器，并且模拟了一些常见的输入输出，如串口, 时钟, 键盘, VGA，支持异常处理机制。

其中包含模拟处理器模块、输入输出模块和DEBUG模块

**模拟处理器模块**：模拟CPU的执行过程，包括取指、译指、执行和更新pc。

**输入输出模块**：模拟了一些常见的输入输出，如串口, 时钟, 键盘, VGA

**DEBUG模块包含**

```text
DEBUG = GDB + trace + 差分测试（Differential Testing）
```

- GDB：简易GDB，包含单步执行、打印程序状态、设置监视点、帮助等
- trace：可以记录执行过的指令、访问过的函数、设备和异常。
- 差分测试（Differential Testing）：Spike作为REF(Reference, 参考实现)，nemu作为DUT(Design Under Test, 测试对象)，双方每执行完一条指令, 就检查各自的寄存器和内存的状态, 如果发现状态不一致, 就马上报告错误, 停止客户程序的执行 。

## ABSTRACT-MACHINE

AM--裸机(bare-metal)运行时环境，把程序运行时需要的环境抽象为API提供给程序。

具体的，AM包括下面几个部分

```text
AM = TRM + IOE + CTE
```

- TRM(Turing Machine) - 图灵机, 最简单的运行时环境, 为程序提供基本的计算能力
- IOE(I/O Extension) - 输入输出扩展, 为程序提供输出输入的能力
- CTE(Context Extension) - 上下文扩展, 为程序提供上下文管理的能力

## NPC

用RTL实现的单周期处理器，指令集为RISCV32。

其中包含CPU模块和DEBUG模块

**CPU模块包含**

```Text
CPU = IFU + IDU + EXU + 更新pc + 内存
```

- IFU(Instruction Fetch Unit): 负责根据当前PC从存储器中取出一条指令
- IDU(Instruction Decode Unit): 负责对当前指令进行译码, 准备执行阶段需要使用的数据和控制信号
- EXU(EXecution Unit): 负责根据控制信号对数据进行执行操作, 并将执行结果写回寄存器或存储器
- 更新PC: 通过RTL实现时, 这一操作一般与PC寄存器一同实现, 因而无需为此划分一个独立的模块
- 内存：保存用户程序和数据

**DEBUG模块包含**

```text
DEBUG = GDB + trace + 差分测试（Differential Testing）
```

- GDB：简易GDB，包含单步执行、打印程序状态、设置监视点、帮助等
- trace：可以记录执行过的指令、访问过的函数、设备和异常。
- 差分测试（Differential Testing）：nemu作为REF(Reference, 参考实现)，npc作为DUT(Design Under Test, 测试对象)，双方每执行完一条指令, 就检查各自的寄存器和内存的状态, 如果发现状态不一致, 就马上
