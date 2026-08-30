# 第三方代码与许可说明

本文件记录仓库所包含的上游代码和二进制组件。它用于说明来源和版权边界，不替代各文件内的原始版权声明，也不构成对第三方代码的重新授权。

## 成都逐飞科技 STC32G12K128 开源库

- 权利人：成都逐飞科技有限公司（SEEKFREE）
- 组件位置：`Seekfree_STC32G12K128_Opensource_Library/Libraries/`、`Project/USER/` 以及相关 Keil 工程配置
- 随附版本：`Libraries/doc/version.txt` 标记为 V1.9.1，日期为 2024-12-30
- 许可材料：根目录 `LICENSE` 包含 GNU General Public License version 3

逐飞源码中的版权头必须保留。部分较早文件包含“All rights reserved”“未经允许不得用于商业用途”等表述，部分较新文件声明 GPL-3.0-or-later。由于这些表述并不完全一致，使用者应以逐飞科技发布该具体文件或版本时的正式授权为准。

本项目在上述基础库上增加或修改了气垫船控制、PID、GPS、IMU、CCD、菜单和调试相关代码。修改内容不改变上游文件的权利归属。

## STC32_MDU32_V1.1.LIB

- 文件：`Seekfree_STC32G12K128_Opensource_Library/Libraries/libraries/STC32_MDU32_V1.1.LIB`
- 类型：STC32G 平台预编译库

## SEEKFREE_CONFIG.LIB

- 文件：`Seekfree_STC32G12K128_Opensource_Library/Libraries/seekfree_peripheral/SEEKFREE_CONFIG.LIB`
- 类型：逐飞外设配置预编译库

## 再分发注意事项

1. 不得删除或替换第三方源码中的版权和许可声明。
2. 发布修改后的源码时，应清楚说明修改内容和日期。
3. 发布 HEX、可执行文件或其他二进制成果前，应确认所链接预编译库允许以该方式再分发。
4. 根目录 GPLv3 文本不应被解释为覆盖或变更第三方组件中更具体的版权声明。
5. 确认逐飞官方上游仓库地址后，应在 README 和本文件中补充准确链接及对应提交或发布版本。
