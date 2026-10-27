# 打印字体

字体文件仅保存在本机，不提交到 Git。生成器缺少字体时直接报错，不静默替换。

| 用途 | 字体 | 默认文件 |
| --- | --- | --- |
| 正文中文、目录中文 | 宋体 | `C:/Windows/Fonts/simsun.ttc` |
| 正文西文、目录西文 | Latin Modern Roman | `lmroman10-regular.otf` |
| 非代码数学符号 | Latin Modern Math | `latinmodern-math.otf` |
| 标题、代码卡片标题、页眉页码 | 苹方简体 | `PingFang.ttc` |
| 代码及中文注释 | Maple Mono NF CN | `MapleMono-NF-CN-Regular.ttf` |

除系统宋体外，将上述文件放在本目录，或通过生成器的字体参数指定路径。

- Latin Modern Roman：[CTAN 官方字体文件](https://mirrors.ctan.org/fonts/lm/fonts/opentype/public/lm/lmroman10-regular.otf)。
- Latin Modern Math：[CTAN 官方字体文件](https://mirrors.ctan.org/fonts/lm-math/opentype/latinmodern-math.otf)。
- Maple Mono NF CN：[官方下载说明](https://font.subf.dev/zh-cn/download/)，本次使用 [v7.9 的 unhinted 版本](https://github.com/subframe7536/maple-font/releases/tag/v7.9)，解压后取常规体文件。
- 苹方：本次使用 [Apple 官方字体目录](https://mesu.apple.com/assets/macos/com_apple_MobileAsset_Font7/com_apple_MobileAsset_Font7.xml) 提供的 [PingFang 字体资源包](https://updates.cdn-apple.com/2024/mobileassets/062-14283/992898D8-65BC-436B-B9F9-EB2D576A24BC/com_apple_MobileAsset_Font7/3419f2a427639ad8c8e139149a287865a90fa17e.zip)，版本为 `20.0d4e1`。资源包 SHA-1 为 `def134242e35228aeaa0e3532cced33d8ca0912d`，与官方目录一致；解压后取 `AssetData/PingFang.ttc`。本项目不分发字体文件，也不使用第三方转载包或替代字体。TTC 默认按字体名称选择 `PingFang SC Regular` 和 `PingFang SC Semibold`，不假定集合中的编号。

Windows 当前用户已安装的副本位于 `%LOCALAPPDATA%/Microsoft/Windows/Fonts/PingFang.ttc`；项目生成器使用本目录中的原始 TTC。集合包含简体、繁体、香港及澳门苹方，各有 6 个字重。不修改其他系统字体或应用的默认字体；部分已打开的应用需要重新启动后才会刷新字体列表。

可分别通过 `--code-font`、`--chinese-font`、`--latin-font`、`--math-font`、`--heading-font` 指定字体。苹方粗体是独立文件时，用 `--heading-bold-font` 指定；特殊字体集合可用 `--heading-index` 和 `--heading-bold-index` 指定编号。

ReportLab 不直接支持 CFF 轮廓的 OTF。生成器仅将用于 PDF 嵌入的轮廓转换或 TTC 拆分结果缓存到 `build/fonts/`，不修改原字体文件。数学符号使用 Latin Modern Math；源码和注释中的公式保持代码原文，不解释为 LaTeX。
