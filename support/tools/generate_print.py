"""从源码和命令原文生成 A4 竖版模板册，不改写算法源码。"""
from __future__ import annotations

import argparse
import hashlib
import io
import json
import shutil
from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path

from reportlab.lib.pagesizes import A4
from reportlab.lib.units import mm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.pdfgen import canvas


ROOT = Path(__file__).resolve().parents[2]
OUTPUT = ROOT / "output/pdf/template.pdf"
PAGE_W, PAGE_H = A4
LEFT, RIGHT, GAP = 17 * mm, 15 * mm, 10 * mm
TOP, BOTTOM = PAGE_H - 22 * mm, 16 * mm
COL_W = PAGE_W - LEFT - RIGHT
CODE_SIZE, LEADING = 9.5, 12
CODE_W = COL_W
TITLE_HEIGHT = 25

# 只维护目录名和顺序；正文来自对应文件。
CHAPTERS = [
    ("template", [
        ("support/tools/contest_main.cpp", "template"),
        ("support/tools/linux_commands.txt", "Linux 编译与运行"),
        ("support/tools/stress/stress.cpp", "C++ 对拍"),
    ]),
    ("数据结构", [
        ("template/ds/并查集.cpp", "并查集"),
        ("template/ds/可撤销并查集.cpp", "可撤销并查集"),
        ("template/ds/带权与异或并查集.cpp", "带权与异或并查集"),
        ("template/ds/树状数组.cpp", "树状数组"),
        ("template/ds/二维树状数组.cpp", "二维树状数组"),
        ("template/ds/线段树与树上二分.cpp", "线段树与树上二分"),
        ("template/ds/区间集合与最小未出现数.cpp", "区间集合与 mex"),
        ("template/ds/仿射线段树.cpp", "仿射线段树"),
        ("template/ds/吉司机线段树.cpp", "吉司机线段树"),
        ("template/ds/动态开点线段树合并与分裂.cpp", "动态开点线段树合并与分裂"),
        ("template/ds/李超线段树.cpp", "李超线段树"),
        ("template/ds/主席树.cpp", "主席树"),
        ("template/ds/可持久化数组.cpp", "可持久化数组"),
        ("template/ds/无旋平衡树.cpp", "FHQ Treap"),
        ("template/ds/左偏树与可并堆.cpp", "左偏树：可并堆"),
        ("template/ds/二进制字典树.cpp", "01 Trie"),
        ("template/ds/可持久化二进制字典树.cpp", "可持久化 01 Trie"),
        ("template/ds/二维最近邻.cpp", "KD-tree：二维最近邻"),
        ("template/ds/动态树.cpp", "Link-Cut Tree"),
    ]),
    ("图论", [
        ("template/graph/堆优化最短路.cpp", "Dijkstra 最短路"),
        ("template/graph/零一最短路.cpp", "0-1 BFS"),
        ("template/graph/全源最短路.cpp", "Floyd 最短路"),
        ("template/graph/重赋权全源最短路.cpp", "Johnson 全源最短路"),
        ("template/graph/第几短游走.cpp", "k 短游走"),
        ("template/graph/差分约束.cpp", "差分约束"),
        ("template/graph/拓扑排序.cpp", "拓扑排序"),
        ("template/graph/函数图分解与跳跃.cpp", "函数图分解与跳跃"),
        ("template/graph/最小生成树.cpp", "最小生成树"),
        ("template/graph/重构树.cpp", "Kruskal 重构树"),
        ("template/graph/最小斯坦纳树.cpp", "最小斯坦纳树"),
        ("template/graph/最小树形图.cpp", "最小树形图"),
        ("template/graph/全局最小割.cpp", "全局最小割"),
        ("template/graph/离线动态连通性.cpp", "离线动态连通性"),
        ("template/graph/倍增最近公共祖先.cpp", "倍增 LCA"),
        ("template/graph/欧拉序最近公共祖先.cpp", "欧拉序 LCA"),
        ("template/graph/树链剖分.cpp", "树链剖分"),
        ("template/graph/长链剖分与祖先查询.cpp", "长链剖分与第 k 级祖先"),
        ("template/graph/虚树.cpp", "虚树"),
        ("template/graph/点分治.cpp", "点分治"),
        ("template/graph/点分树.cpp", "点分树"),
        ("template/graph/树的直径.cpp", "树的直径"),
        ("template/graph/换根动态规划与距离和.cpp", "换根 DP：距离和"),
        ("template/graph/树上启发式合并.cpp", "树上启发式合并"),
        ("template/graph/强连通分量.cpp", "强连通分量"),
        ("template/graph/二元可满足性.cpp", "2-SAT"),
        ("template/graph/支配树.cpp", "支配树"),
        ("template/graph/割点.cpp", "割点"),
        ("template/graph/桥.cpp", "桥"),
        ("template/graph/点双连通分量.cpp", "点双连通分量"),
        ("template/graph/圆方树.cpp", "圆方树"),
        ("template/graph/边双连通分量.cpp", "边双连通分量"),
        ("template/graph/边双缩点树.cpp", "边双缩点树"),
        ("template/graph/有向图欧拉路.cpp", "有向图欧拉路"),
        ("template/graph/无向图欧拉路.cpp", "无向图欧拉路"),
        ("template/graph/二分图匹配.cpp", "二分图匹配"),
        ("template/graph/分层增广二分图匹配.cpp", "Hopcroft-Karp"),
        ("template/graph/一般图匹配.cpp", "一般图匹配"),
        ("template/graph/一般图最大权匹配.cpp", "一般图最大权匹配"),
        ("template/graph/最大团与最大独立集.cpp", "最大团与最大独立集"),
        ("template/graph/二分图最大权匹配.cpp", "KM 带权匹配"),
        ("template/graph/最大流.cpp", "最大流"),
        ("template/graph/最小割树.cpp", "Gomory-Hu 最小割树"),
        ("template/graph/上下界网络流.cpp", "上下界网络流"),
        ("template/graph/势能最短路费用流.cpp", "费用流：势能 Dijkstra"),
        ("template/graph/上下界最小费用流.cpp", "上下界最小费用流"),
        ("template/graph/容量缩放费用流.cpp", "费用流：容量缩放"),
    ]),
    ("数学", [
        ("template/math/线性筛素数.cpp", "线性筛素数"),
        ("template/math/线性筛积性函数.cpp", "线性筛积性函数"),
        ("template/math/素性测试与整数分解.cpp", "Miller-Rabin 与 Pollard-Rho"),
        ("template/math/原根.cpp", "原根"),
        ("template/math/快速幂与逆元.cpp", "快速幂与线性、批量逆元"),
        ("template/math/扩展欧几里得与中国剩余定理.cpp", "exgcd、逆元与扩展 CRT"),
        ("template/math/离散对数.cpp", "BSGS 与扩展 BSGS"),
        ("template/math/组合数.cpp", "组合数"),
        ("template/math/卢卡斯定理.cpp", "Lucas 定理"),
        ("template/math/扩展卢卡斯定理.cpp", "扩展 Lucas"),
        ("template/math/二次剩余.cpp", "二次剩余"),
        ("template/math/整除分块.cpp", "整除分块"),
        ("template/math/类欧几里得.cpp", "类欧几里得"),
        ("template/math/杜教筛.cpp", "杜教筛"),
        ("template/math/积性函数求和.cpp", "Min_25 筛"),
        ("template/math/矩阵快速幂.cpp", "矩阵快速幂"),
        ("template/math/高斯消元.cpp", "高斯消元"),
        ("template/math/矩阵求逆.cpp", "矩阵求逆"),
        ("template/math/单纯形与线性规划.cpp", "单纯形：线性规划"),
        ("template/math/行列式与矩阵树定理.cpp", "行列式与矩阵树定理"),
        ("template/math/异或线性基.cpp", "异或线性基"),
        ("template/math/拉格朗日插值.cpp", "拉格朗日插值"),
        ("template/math/等距拉格朗日插值.cpp", "等距拉格朗日插值"),
        ("template/math/最短线性递推.cpp", "BM 与线性递推"),
        ("template/math/快速数论变换与多项式乘法.cpp", "NTT 与多项式乘法"),
        ("template/math/两类斯特林数.cpp", "两类斯特林数"),
        ("template/math/多项式基本运算.cpp", "多项式求逆、对数、指数、幂、开根与除法"),
        ("template/math/线性递推快速求项.cpp", "Bostan-Mori 递推求项"),
        ("template/math/多项式多点求值与快速插值.cpp", "多项式多点求值与快速插值"),
        ("template/math/快速傅里叶变换与任意模数卷积.cpp", "FFT 与任意模数卷积"),
        ("template/math/快速沃尔什变换.cpp", "快速沃尔什变换"),
        ("template/math/约数与倍数变换与卷积.cpp", "约数、倍数变换与卷积"),
        ("template/math/子集卷积.cpp", "子集卷积"),
        ("template/math/自适应辛普森积分.cpp", "自适应辛普森积分"),
    ]),
    ("字符串", [
        ("template/string/字典树.cpp", "字典树"),
        ("template/string/字符串哈希.cpp", "字符串哈希"),
        ("template/string/二维哈希.cpp", "二维哈希"),
        ("template/string/字符串匹配.cpp", "KMP 字符串匹配"),
        ("template/string/扩展字符串匹配.cpp", "Z 函数与扩展 KMP"),
        ("template/string/最小表示法.cpp", "最小表示法"),
        ("template/string/林登分解.cpp", "Lyndon 分解"),
        ("template/string/最长回文子串.cpp", "Manacher 回文"),
        ("template/string/回文自动机.cpp", "回文自动机"),
        ("template/string/多模式匹配自动机.cpp", "AC 自动机"),
        ("template/string/后缀自动机.cpp", "后缀自动机"),
        ("template/string/后缀数组.cpp", "后缀数组"),
        ("template/string/极大周期子串.cpp", "极大周期子串 runs"),
        ("template/string/位集优化最长公共子序列.cpp", "位集优化 LCS"),
    ]),
    ("计算几何", [
        ("template/geometry/点与向量.cpp", "点与向量"),
        ("template/geometry/直线与线段.cpp", "直线与线段"),
        ("template/geometry/多边形.cpp", "多边形"),
        ("template/geometry/凸包与直径与闵可夫斯基和.cpp", "凸包、直径与闵可夫斯基和"),
        ("template/geometry/最小面积包围矩形.cpp", "最小面积包围矩形"),
        ("template/geometry/凸多边形包含查询.cpp", "凸多边形包含查询"),
        ("template/geometry/平面最近点对.cpp", "平面最近点对"),
        ("template/geometry/圆.cpp", "圆"),
        ("template/geometry/半平面交.cpp", "半平面交"),
        ("template/geometry/最小圆覆盖.cpp", "最小圆覆盖"),
        ("template/geometry/矩形面积并与周长并.cpp", "矩形面积并与周长并"),
        ("template/geometry/圆面积并与周长并.cpp", "圆面积并与周长并"),
        ("template/geometry/三维凸包.cpp", "三维凸包"),
    ]),
    ("其他算法", [
        ("template/others/离散化.cpp", "离散化"),
        ("template/others/稀疏表.cpp", "ST 表"),
        ("template/others/滑动窗口最值.cpp", "滑动窗口最值"),
        ("template/others/单调栈与左右最近更小与更大.cpp", "单调栈：左右最近更小与更大"),
        ("template/others/单调凸包与斜率优化.cpp", "单调凸包与斜率优化"),
        ("template/others/笛卡尔树.cpp", "笛卡尔树"),
        ("template/others/莫队.cpp", "莫队"),
        ("template/others/带修改莫队.cpp", "带修改莫队"),
        ("template/others/树上莫队.cpp", "树上莫队"),
        ("template/others/分治三维偏序.cpp", "CDQ 三维偏序"),
        ("template/others/整体二分与动态区间第几小.cpp", "整体二分：动态区间第 k 小"),
        ("template/others/三分搜索.cpp", "三分搜索"),
        ("template/others/树哈希.cpp", "树哈希"),
        ("template/others/分治动态规划与单调队列多重背包.cpp", "分治 DP 与单调队列多重背包"),
        ("template/others/最长上升子序列与方案.cpp", "最长上升子序列与方案"),
        ("template/others/博弈状态函数.cpp", "SG 函数"),
        ("template/others/舞蹈链精确覆盖.cpp", "DLX 精确覆盖"),
    ]),
]


def register_fonts(code_font: Path, chinese_font: Path) -> None:
    pdfmetrics.registerFont(TTFont("Code", str(code_font)))
    pdfmetrics.registerFont(TTFont("Chinese", str(chinese_font), subfontIndex=0))
    pdfmetrics.registerFont(TTFont("ChineseHeading", "C:/Windows/Fonts/STXINWEI.TTF"))


@lru_cache(maxsize=None)
def glyph(char: str, size: float, heading=False, code=False) -> tuple[str, float]:
    font = ("Times-Bold" if ord(char) < 128 else "ChineseHeading") if heading else (
        "Code" if ord(char) < 128 else "Chinese" if code else "ChineseHeading")
    if font != "Times-Bold" and ord(char) not in pdfmetrics.getFont(font).face.charToGlyph:
        raise ValueError(f"字体缺少字符 {char!r} (U+{ord(char):04X})")
    return font, pdfmetrics.stringWidth(char, font, size)


def width(text: str, size: float, heading=False, code=False) -> float:
    return sum(glyph(char, size, heading, code)[1] for char in text)


def draw_text(pdf: canvas.Canvas, x: float, y: float, text: str,
              size: float, gray: float = 0, heading=False, code=False) -> None:
    pdf.setFillGray(gray)
    obj = pdf.beginText(x, y)
    previous = None
    for char in text:
        font, _ = glyph(char, size, heading, code)
        if font != previous:
            obj.setFont(font, size)
            previous = font
        obj.textOut(char)
    pdf.drawText(obj)


def wrap(text: str, size: float, max_width: float) -> list[str]:
    """仅作显示折行，拼接结果必须逐字符等于原文。"""
    if not text:
        return [""]
    result = []
    while text:
        length, measured = 0, 0.0
        for char in text:
            advance = glyph(char, size, code=True)[1]
            if measured + advance > max_width:
                break
            measured += advance
            length += 1
        if not length:
            raise ValueError("可用行宽小于单个字符")
        if length < len(text):
            # 尽量在空白处分行，但不删除或添加原文空格。
            space = text.rfind(" ", max(1, length // 2), length)
            if space >= 0:
                length = space + 1
        result.append(text[:length])
        text = text[length:]
    return result


@dataclass
class Entry:
    number: str
    chapter: str
    path: str
    title: str
    source: str


def load_entries() -> list[Entry]:
    entries = []
    for chapter_number, (chapter, files) in enumerate(CHAPTERS, 1):
        for entry_number, (path, title) in enumerate(files, 1):
            source = (ROOT / path).read_text(encoding="utf-8")
            entries.append(Entry(f"{chapter_number}.{entry_number}", chapter, path,
                                 title, source))
    paths = [entry.path for entry in entries]
    actual = {path.relative_to(ROOT).as_posix() for path in (ROOT / "template").rglob("*.cpp")}
    actual.discard("template/others/常用定义.cpp")  # 公共定义与比赛入口重复，不收录。
    listed = {path for path in paths if path.startswith("template/")}
    if len(paths) != len(set(paths)) or actual != listed:
        raise ValueError(f"目录重复或漏收：{actual ^ listed}")
    return entries


class Book:
    def __init__(self, target, entries: list[Entry], locations=None):
        self.pdf = canvas.Canvas(target, pagesize=(PAGE_W, PAGE_H), pageCompression=1)
        self.pdf.setTitle("XCPC 算法模板")
        self.pdf.setAuthor("")
        self.entries = entries
        self.locations = locations or {}
        self.page = 1
        self.col = 0
        self.y = TOP
        self.chapter = "目录"
        self.page_titles = []
        self.result = {}
        self.audit = []

    @property
    def x(self):
        return LEFT

    def running_header(self):
        pdf = self.pdf
        label = str(self.page)
        draw_text(pdf, PAGE_W - RIGHT - width(label, 8), PAGE_H - 11 * mm, label, 8)
        pdf.setStrokeGray(.65)
        pdf.setLineWidth(.35)
        pdf.line(LEFT, PAGE_H - 13 * mm, PAGE_W - RIGHT, PAGE_H - 13 * mm)

    def new_page(self, chapter: str):
        self.running_header()
        self.pdf.showPage()
        self.page += 1
        self.chapter, self.col, self.y = chapter, 0, TOP
        self.page_titles = []

    def next_column(self):
        self.new_page(self.chapter)

    def contents(self):
        draw_text(self.pdf, LEFT, TOP, "XCPC 算法模板", 24, heading=True)
        col_width = (COL_W - GAP) / 2
        number_width = max(width(entry.number, 9) for entry in self.entries)
        title_offset = 7 + number_width + width("  ", 9)
        self.y = TOP - 44
        def next_contents_column():
            if self.col == 0:
                self.col, self.y = 1, TOP - 44 if self.page == 1 else TOP
            else:
                self.new_page("目录")
            return LEFT + self.col * (col_width + GAP)

        for number, (chapter, _) in enumerate(CHAPTERS, 1):
            items = [entry for entry in self.entries if entry.chapter == chapter]
            if self.y - 38 < BOTTOM:
                next_contents_column()
            x = LEFT + self.col * (col_width + GAP)
            draw_text(self.pdf, x, self.y, f"{number}  {chapter}", 12, heading=True)
            self.y -= 21
            for entry in items:
                if self.y < BOTTOM + 14:
                    x = next_contents_column()
                draw_text(self.pdf, x + 7, self.y, entry.number, 9)
                draw_text(self.pdf, x + title_offset, self.y, entry.title, 9)
                page = str(self.locations.get(entry.path, ""))
                draw_text(self.pdf, x + col_width - width(page, 9), self.y, page, 9)
                start = x + title_offset + width(entry.title, 9) + 6
                end = x + col_width - width(page, 9) - 6
                self.pdf.setStrokeGray(.55)
                self.pdf.setLineWidth(.35)
                self.pdf.setDash(1, 2)
                self.pdf.line(start, self.y + 2, end, self.y + 2)
                self.pdf.setDash()
                self.pdf.linkRect("", entry.path,
                                  (x, self.y - 3, x + col_width, self.y + 10), thickness=0)
                self.y -= 17
            self.y -= 12

    def title(self, entry: Entry):
        label = entry.number + "  " + entry.title
        draw_text(self.pdf, self.x, self.y, label, 13, heading=True)
        self.y -= 8
        self.pdf.setStrokeGray(.4)
        self.pdf.setLineWidth(.4)
        self.pdf.line(self.x, self.y, self.x + COL_W, self.y)
        self.y -= 17

    def entry(self, entry: Entry):
        rows = []
        for number, source_line in enumerate(entry.source.splitlines(), 1):
            line = source_line.expandtabs(4)
            pieces = wrap(line, CODE_SIZE, CODE_W)
            assert "".join(pieces) == line
            for part, text in enumerate(pieces):
                assert width(text, CODE_SIZE, code=True) <= CODE_W + .001
                rows.append((number, part, text))
        title_height = TITLE_HEIGHT
        available = self.y - BOTTOM
        full_height = title_height + len(rows) * LEADING
        if available < title_height + min(8, len(rows)) * LEADING or (
                full_height > available and (full_height <= TOP - BOTTOM or self.page_titles)):
            self.next_column()
        self.result[entry.path] = self.page
        self.pdf.bookmarkHorizontalAbsolute(entry.path, self.y + 12)
        self.pdf.addOutlineEntry(entry.number + " " + entry.title,
                                 entry.path, level=1)
        self.title(entry)
        self.page_titles.append(entry.title)
        index = 0
        while index < len(rows):
            capacity = int((self.y - BOTTOM) // LEADING)
            end = min(index + capacity, len(rows))
            if end < len(rows):
                if len(rows) - end < 6:
                    end = len(rows) - 6
                # 只在页末附近的空行处分隔，避免过度留白。
                candidates = [i + 1 for i in range(max(index + 1, end - 4), end)
                              if rows[i][2] == "" and rows[i][1] == 0]
                if candidates:
                    end = candidates[-1]
                elif rows[end][1] and end > index + 1:
                    while end > index and rows[end][1]:
                        end -= 1
            if end <= index:
                raise ValueError(f"无法分页：{entry.path}")
            for number, part, text in rows[index:end]:
                draw_text(self.pdf, self.x, self.y, text, CODE_SIZE, code=True)
                self.audit.append({"path": entry.path, "line": number, "part": part,
                                   "text": text, "page": self.page, "column": self.col,
                                   "x": self.x, "y": self.y,
                                   "width": width(text, CODE_SIZE, code=True)})
                self.y -= LEADING
            index = end
            if index < len(rows):
                self.next_column()
                self.page_titles.append(entry.title)
        self.y -= 24

    def build(self):
        self.pdf.bookmarkPage("contents")
        self.pdf.addOutlineEntry("目录", "contents", level=0)
        self.contents()
        self.new_page("正文")
        for number, (chapter, _) in enumerate(CHAPTERS, 1):
            first = next(entry for entry in self.entries if entry.chapter == chapter)
            first_rows = sum(len(wrap(line.expandtabs(4), CODE_SIZE, CODE_W))
                             for line in first.source.splitlines())
            first_height = 35 + TITLE_HEIGHT + first_rows * LEADING
            if self.y - BOTTOM < 160 or (
                    self.page_titles and first_height > self.y - BOTTOM):
                self.next_column()
            self.chapter = f"{number} {chapter}"
            draw_text(self.pdf, self.x, self.y, self.chapter, 18, heading=True)
            self.y -= 35
            key = "chapter-" + chapter
            self.pdf.bookmarkHorizontalAbsolute(key, self.y + 35)
            self.pdf.addOutlineEntry(f"{number} {chapter}", key, level=0)
            for entry in self.entries:
                if entry.chapter == chapter:
                    self.entry(entry)
        self.running_header()
        self.pdf.save()
        return self.result, self.page, self.audit


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=OUTPUT)
    parser.add_argument("--code-font", type=Path, default=Path("C:/Windows/Fonts/consola.ttf"))
    parser.add_argument("--chinese-font", type=Path, default=Path("C:/Windows/Fonts/simsun.ttc"))
    args = parser.parse_args()
    register_fonts(args.code_font, args.chinese_font)
    entries = load_entries()
    locations, pages, _ = Book(io.BytesIO(), entries).build()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    final_locations, final_pages, audit = Book(str(args.output), entries, locations).build()
    assert (locations, pages) == (final_locations, final_pages)
    manifest = {
        "pages": pages, "font_size": CODE_SIZE, "line_height": LEADING,
        "page_size": [PAGE_W, PAGE_H],
        "entries": [{"path": entry.path, "title": entry.title,
                     "page": locations[entry.path],
                     "sha256": hashlib.sha256((ROOT / entry.path).read_bytes()).hexdigest()}
                    for entry in entries],
        "rows": audit,
    }
    args.output.with_suffix(".layout.json").write_text(
        json.dumps(manifest, ensure_ascii=False, indent=2), encoding="utf-8")
    if args.output.resolve() == OUTPUT.resolve():
        shutil.copyfile(args.output, ROOT / "template.pdf")
    print(f"Generated {args.output}: {len(entries)} entries, {pages} pages")


if __name__ == "__main__":
    main()
