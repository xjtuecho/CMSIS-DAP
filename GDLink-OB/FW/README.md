# GDLink-OB firmware

目前GDLink分2个版本：

- 热缩管塑封的为GDLink-S
- 带外壳的为GDLink-Lite

两者固件不兼容，不能通刷，如果您的设备使用正常，不建议刷固件。

GDLink-S硬件v23.2.10版本开始，固件与GDLink-Lite保持一致，两者可通刷，后续只维护GDLink-Lite固件。

2021年7月之前出货版本默认引导程序版本为v21.3.14，使用TDO作为启动选择管脚，使用
JTAG时，TDO连接目标板容易导致误进入引导程序，可以USB上电以后再接TDO避免该问题。
7月之后出货版本引导程序升级为v21.6.21，改用TRESET作为引导选择管脚。

# 固件更新方法

- 首先安装虚拟串口驱动`USB Virtual Com Port Driver.exe`，x64目录对应64位系统，x86目录对应32位系统
- 使用跳线冒或者杜邦线短接端子上的引导选择管脚(TDO或者TRESET)和`GND`，然后插入PC，识别出一个虚拟串口
- 使用`超级终端`连接虚拟串口，串口参数`115200-8-N-1`
- 执行`ymodem`命令，菜单`传送-发送文件`，文件名选择要烧录的`hex`文件，协议选择`Ymodem`
- 点击`发送`按钮，等待发送完成即可

# 修订历史

## v24.3.8

- 适配GDLink Programmer 4.9.0.21134 版本，版本号r74。

## v23.2.10

- 更新S版本硬件，使用12M贴片晶振，管脚分配与Lite版本一致，固件可通刷。

## v21.6.21

- 使能JTAG功能，实测使用JTAG可以连接烧录RISC-V内核的GD32VF103。
- XBOOT_USB将引导选择管脚改为TRESET，防止使用JTAG时TDO被拉低导致进入XBOOT_USB。
- 从该版本开始同时提供8M和12M晶振固件，2022/6/25以后出货的GDLink-Lite硬件为12M晶振。

## v21.5.20

- 适配gdlink programmer 4.3.7和4.5.1两个版本。
  注意这两个版本的软件会监听5600端口，目的未知，还是推荐使用3.0.0版本。
- gdlink programmer 3.0.0 不支持 Cortex-M33 内核的 GD32E50x 芯片，需要使用 4.5.1 版本。

## v21.3.14

- 初始发布。
- GDLink-Lite固件为了兼容QFN32和QFN28封装，修改了LED和TRESET管脚的位置，与GDLink-S固件不兼容。