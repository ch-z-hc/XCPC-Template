# 使用说明

## 本地预览

在仓库根目录执行：

```powershell
python -m pip install -r requirements-docs.txt
mkdocs serve
```

打开终端输出的本地地址即可预览。MkDocs 在构建前会自动运行 `tools/generate_docs.py`，生成的模板页面位于 `docs/algorithms/`，但不会提交到 Git。

## 严格构建

```powershell
mkdocs build --strict
```

静态站点输出到 `site/`。该目录属于本地构建产物，不会提交到 Git。

## 更新模板文档

修改或新增分类目录中的 `.cpp` 文件后，重新构建站点即可。生成脚本会：

1. 扫描 `ds/`、`geometry/`、`graph/`、`math/`、`others/` 和 `string/`；
2. 从 `README.md` 的接口索引读取公开接口；
3. 从模板开头注释提取适用条件、复杂度、下标约定和易错点；
4. 将 `.cpp` 原文嵌入生成页面。

若生成页提示某项说明缺失，应补充模板开头注释或 `README.md` 接口索引，不要直接编辑 `docs/algorithms/`。

## 部署到 GitHub Pages

`.github/workflows/deploy-docs.yml` 会在 `main` 分支更新时严格构建并部署站点。首次部署前，在 GitHub 仓库的 `Settings > Pages` 中将 `Source` 设为 `GitHub Actions`。
