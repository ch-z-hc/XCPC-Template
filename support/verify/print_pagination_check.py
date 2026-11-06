"""检查自适应字号的页数、可读性边界和逐行分页完整性。"""
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from generate_print import (
    BOTTOM, CODE_SIZE, CODE_W, LEADING, MAX_CODE_SIZE, MIN_CODE_SIZE,
    TITLE_HEIGHT, TOP, card_bottom, card_top, plan_code, split_rows, start_on_fresh_page,
)


def check(rows, top, max_width):
    minimum = split_rows(rows, top, MIN_CODE_SIZE)
    if not minimum:
        return
    plan = plan_code(rows, top, max_width)
    assert MIN_CODE_SIZE <= plan.font_size <= MAX_CODE_SIZE
    assert max_width * plan.font_size / CODE_SIZE <= CODE_W + 1e-6
    assert sum(plan.chunks) == len(rows)
    assert len(plan.chunks) == len(minimum)
    assert abs(plan.line_height / plan.font_size - LEADING / CODE_SIZE) < 1e-8
    y = top - TITLE_HEIGHT
    for count in plan.chunks:
        bottom = y - card_top(plan.font_size) - card_bottom(plan.font_size) - (count - 1) * plan.line_height
        assert bottom >= BOTTOM - 1e-6
        y = TOP
    if len(plan.chunks) > 1:
        assert plan.chunks[0] >= 8 and plan.chunks[-1] >= 6
    upper = min(MAX_CODE_SIZE, CODE_SIZE * CODE_W / max_width)
    normal = split_rows(rows, top, CODE_SIZE)
    if len(normal) != 1 and plan.font_size + .001 < upper:
        larger = split_rows(rows, top, plan.font_size + .001)
        assert not larger or len(larger) > len(plan.chunks)


def main():
    for count in range(1, 301):
        for blank_every in (0, 7):
            rows = [(i, "" if blank_every and i % blank_every == 0 else "x") for i in range(count)]
            for top in (TOP, TOP - 44, TOP - 200, BOTTOM + 150):
                for width in (CODE_W * .7, CODE_W):
                    check(rows, top, width)
    rows = list(enumerate(["x"] * 60))
    plan = plan_code(rows, TOP, CODE_W * .7)
    assert len(split_rows(rows, TOP, CODE_SIZE)) == 2
    assert len(plan.chunks) == 1 and MIN_CODE_SIZE < plan.font_size < CODE_SIZE
    gap = TOP - TITLE_HEIGHT - card_top(plan.font_size) - card_bottom(plan.font_size) - 59 * plan.line_height - BOTTOM
    assert abs(gap) < .001
    # 页尾只剩不足半页时，不放下一份长代码的开头。
    rows = list(enumerate(["x"] * 66))
    fresh = plan_code(rows, TOP, CODE_W * .7)
    top = BOTTOM + (TOP - BOTTOM) * .4
    assert start_on_fresh_page(plan_code(rows, top, CODE_W * .7), fresh, top)
    # 足够接排的长代码、能完整放下的短代码仍利用剩余空间。
    top = TOP - 100
    assert not start_on_fresh_page(plan_code(rows, top, CODE_W * .7), fresh, top)
    short = list(enumerate(["x"] * 8))
    top = BOTTOM + 160
    assert not start_on_fresh_page(plan_code(short, top, CODE_W * .7),
                                   plan_code(short, TOP, CODE_W * .7), top)
    # 缩小字号可整页容纳时，也应整体移到下一页。
    rows = list(enumerate(["x"] * 60))
    top = TOP - 100
    assert start_on_fresh_page(plan_code(rows, top, CODE_W * .7),
                               plan_code(rows, TOP, CODE_W * .7), top)
    print("PASS: pagination boundaries, whole-source coverage, width limits and near-full final page")


if __name__ == "__main__":
    main()
