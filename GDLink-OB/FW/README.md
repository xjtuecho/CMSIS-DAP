# GDLink-OB firmware

目前GDLink分2个版本：

- 热缩管塑封的为GDLink-S
- 带外壳的为GDLink-Lite

两者固件不兼容，不能通刷，如果您的设备使用正常，不建议刷固件。

2021年7月之前出货版本默认引导程序版本为v21.3.14，使用TDO作为启动选择管脚，使用
JTAG时，TDO连接目标板容易导致误进入引导程序，可以USB上电以后再接TDO避免该问题。
7月之后出货版本引导程序升级为v21.6.21，改用TReset作为引导选择管脚。

# 修订历史

## v21.6.21

- 使能JTAG功能，实测使用JTAG可以连接烧录RISC-V内核的GD32VF103。
- XBOOT_USB将引导选择管脚改为TReset，防止使用JTAG时TDO被拉低导致进入XBOOT_USB。

## v21.5.20

- 适配gdlink programmer 4.3.7和4.5.1两个版本。
  注意这两个版本的软件会监听5600端口，目的未知，还是推荐使用3.0.0版本。
- gdlink programmer 3.0.0 不支持 Cortex-M33 内核的 GD32E50x 芯片，需要使用 4.5.1 版本。

## v21.3.14

- 初始发布。
- GDLink-Lite固件为了兼容QFN32和QFN28封装，修改了LED和TReset管脚的位置，与GDLink-S固件不兼容。