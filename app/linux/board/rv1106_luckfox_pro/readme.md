
板级配置：

<SDK目录>/project/cfg/BoardConfig_IPC/


**├── build.sh -> project/build.sh ---- SDK编译脚本**
├── media --------------------------- 多媒体编解码、ISP等算法相关（可独立SDK编译）
├── sysdrv -------------------------- U-Boot、kernel、rootfs目录（可独立SDK编译）
├── project ------------------------- 参考应用、编译配置以及脚本目录
│   ├── cfg
│       ├── BoardConfig_IPC
│           ├── BoardConfig-EMMC-NONE-RV1103_Luckfox_Pico-IPC.mk
│           ├── BoardConfig-EMMC-NONE-RV1103_Luckfox_Pico_Mini_A-IPC.mk
│           ├── BoardConfig-SPI_NAND-NONE-RV1103_Luckfox_Pico_Mini_B-IPC.mk
│           ├── BoardConfig-SPI_NAND-NONE-RV1103_Luckfox_Pico_Plus-IPC.mk
│           ├── BoardConfig-SPI_NAND-NONE-RV1106_Luckfox_Pico_Pro_Max-IPC.mk
│           └── ...
├── output -------------------------- SDK编译后镜像文件存放目录
├── docs ---------------------------- SDK文档目录
└── tools --------------------------- 烧录镜像打包工具以及烧录工具


`RK_KERNEL_DTS` 指定设备树位置


/root/luckfox/luckfox-pico/sysdrv/source/kernel/arch/arm/boot/dts/rv1106-luckfox-pico-pro-max-ipc.dtsi

/root/luckfox/luckfox-pico/sysdrv/source/kernel/arch/arm/boot/dts/rv1106-pinctrl.dtsi

/root/luckfox/luckfox-pico/sysdrv/source/kernel/arch/arm/boot/dts/rv1106g-luckfox-pico-pro.dts
