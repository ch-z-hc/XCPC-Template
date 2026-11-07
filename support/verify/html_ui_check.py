"""使用真实浏览器验证 HTML 复制、搜索、导航与手机目录。"""
from __future__ import annotations

import argparse
import os
import re
import subprocess
from pathlib import Path
import sys

from playwright.sync_api import sync_playwright, expect

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "support/tools"))
from generate_html import entry_id, load_entries


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--url", default="http://127.0.0.1:8765/XCPC-Template/")
    parser.add_argument("--browser", help="可选的 Chromium 可执行文件")
    parser.add_argument("--isolated-fonts", action="store_true",
                        help="Linux 下仅允许 DejaVu 系统字体，验证页面不依赖本机 PDF 字体")
    parser.add_argument("--screenshots", type=Path, default=ROOT / "build/html-preview")
    args = parser.parse_args()
    args.screenshots.mkdir(parents=True, exist_ok=True)
    expected = load_entries()
    errors = []
    env = dict(os.environ)
    if args.isolated_fonts:
        config = args.screenshots.resolve() / "fonts.conf"
        config.write_text('<fontconfig><dir>/usr/share/fonts/truetype/dejavu</dir>'
                          f'<cachedir>{args.screenshots.resolve()}/font-cache</cachedir></fontconfig>')
        env["FONTCONFIG_FILE"] = str(config)
        for name in ("PingFang SC", "SimSun"):
            actual = subprocess.check_output(["fc-match", "-f", "%{family}", name], env=env, text=True)
            assert "DejaVu" in actual, actual
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=True, executable_path=args.browser, env=env)
        context = browser.new_context(viewport={"width": 1440, "height": 1000},
                                      permissions=["clipboard-read", "clipboard-write"])
        context.set_default_timeout(60000)
        page = context.new_page()
        page.on("pageerror", lambda error: errors.append(str(error)))
        page.goto(args.url)
        page.evaluate("document.fonts.ready")
        actual = page.locator(".entry code").all_text_contents()
        assert actual == [entry.source for entry in expected]
        assert page.evaluate('document.fonts.check(\'14px "Maple Mono Web"\')')
        for path in ("support/tools/contest_main.cpp", "template/ds/树状数组.cpp",
                     "template/geometry/点与向量.cpp", "support/tools/linux_commands.txt"):
            entry = next(item for item in expected if item.path == path)
            page.evaluate("id => location.hash = id", entry_id(entry))
            article = page.locator(f'[data-path="{path}"]')
            expect(article).to_be_visible()
            article.locator(".copy-button").click()
            expect(page.locator("#copy-status")).to_have_text(f"已复制：{entry.title}")
            assert page.evaluate("navigator.clipboard.readText()") == entry.source
        page.keyboard.press("/")
        expect(page.locator("#search")).to_be_focused()
        page.locator("#search").fill("Fenwick")
        expect(page.locator('[data-entry="template-ds-树状数组"]')).to_be_visible()
        page.locator("#search").fill("no-such-template-20261107")
        expect(page.locator(".empty-search")).to_be_visible()
        page.keyboard.press("Escape")
        expect(page.locator("#search")).to_have_value("")
        page.locator("#search").fill("树状数组")
        page.locator('[data-entry="template-ds-树状数组"]').click()
        page.locator('[data-entry="template-ds-二维树状数组"]').click()
        expect(page.locator('.entry:not([hidden])')).to_have_attribute("data-path", "template/ds/二维树状数组.cpp")
        page.go_back()
        expect(page.locator('.entry:not([hidden])')).to_have_attribute("data-path", "template/ds/树状数组.cpp")
        page.locator("#search").fill("")
        page.locator("#search").dispatch_event("input")
        page.screenshot(path=str(args.screenshots / "desktop.png"), full_page=True)
        if args.isolated_fonts:
            page.evaluate("document.fonts.ready")
            session = context.new_cdp_session(page)
            session.send("DOM.enable")
            session.send("CSS.enable")
            root = session.send("DOM.getDocument")["root"]["nodeId"]
            for selector, names in (
                ('.entry:not([hidden]) code', {"MapleMono-NF-CN-Regular"}),
            ):
                node = session.send("DOM.querySelector", {"nodeId": root, "selector": selector})["nodeId"]
                fonts = session.send("CSS.getPlatformFontsForNode", {"nodeId": node})["fonts"]
                actual_names = {font["postScriptName"] for font in fonts if font["glyphCount"]}
                assert names <= actual_names, (selector, actual_names)
                assert all(font["isCustomFont"] for font in fonts if font["glyphCount"])
                print(f"PASS: actual rendered fonts {selector}: {sorted(actual_names)}", flush=True)
            session.detach()
            for selector in ('.entry:not([hidden]) h1', '.toc-heading h2',
                             'a[data-entry="template-ds-树状数组"]'):
                label = page.locator(selector).locator('.pdf-label:visible').first
                assert label.locator('svg use').evaluate('el => el.getBBox().width > 0')
            print("PASS: PDF vector labels render without installed PDF fonts", flush=True)
        # Clipboard API 被拒绝时，验证真实的旧接口复制，不只模拟返回成功。
        page.evaluate('() => { navigator.clipboard.writeText = () => Promise.reject(new Error("denied")); }')
        button = page.locator('.entry:not([hidden]) .copy-button')
        expect(button).to_be_enabled()
        button.click()
        expect(button).to_have_text("已复制")
        source = next(item.source for item in expected if item.path == "template/ds/树状数组.cpp")
        assert page.evaluate("navigator.clipboard.readText()") == source
        expect(button).to_be_enabled(timeout=4000)
        page.evaluate("() => { document.execCommand = () => false; }")
        button.click()
        expect(button).to_have_text("请手动复制")
        # Selection 的展示文本会省略末尾换行；Range 应覆盖全部源码节点。
        assert page.evaluate("getSelection().getRangeAt(0).toString()") == source
        print("PASS: desktop code, clipboard, search and history", flush=True)
        page.goto(args.url + "#template-ds-%E6%A0%91%E7%8A%B6%E6%95%B0%E7%BB%84")
        page.reload()
        expect(page.locator('.entry:not([hidden])')).to_have_count(1)
        expect(page.locator('.entry:not([hidden])')).to_have_attribute("data-path", "template/ds/树状数组.cpp")
        page.locator(".skip-link").focus()
        page.keyboard.press("Enter")
        expect(page.locator("#reader")).to_be_focused()
        expect(page.locator('.entry:not([hidden])')).to_have_attribute("data-path", "template/ds/树状数组.cpp")
        page.goto(args.url + "#oeis")
        page.wait_for_url("**/pdf.html#oeis", wait_until="domcontentloaded", timeout=60000)
        expect(page.locator("iframe")).to_have_attribute("src", re.compile(r"pdf/oeis-2up-[0-9a-f]{16}\.pdf#navpanes=0"))
        expect(page.locator(".file")).to_have_text("oeis-2up.pdf")
        page.locator('nav a[href="./"]').click()
        expect(page.locator(".entry:not([hidden])")).to_have_count(1)
        for width in (320, 390, 768):
            mobile = browser.new_context(viewport={"width": width, "height": 844},
                                         is_mobile=True, has_touch=True,
                                         permissions=["clipboard-read", "clipboard-write"])
            mobile.set_default_timeout(60000)
            tab = mobile.new_page()
            tab.on("pageerror", lambda error: errors.append(str(error)))
            tab.goto(args.url)
            tab.evaluate("document.fonts.ready")
            assert tab.evaluate("document.documentElement.scrollWidth <= innerWidth")
            expect(tab.locator(".menu-button")).to_have_attribute("aria-expanded", "false")
            tab.locator(".menu-button").click()
            expect(tab.locator("#search")).to_be_focused()
            assert tab.locator("#reader").evaluate("el => el.inert")
            tab.locator("#search").fill("树状数组")
            if width == 390:
                tab.screenshot(path=str(args.screenshots / "mobile-menu.png"))
            tab.locator('[data-entry="template-ds-树状数组"]').click()
            expect(tab.locator(".menu-button")).to_have_attribute("aria-expanded", "false")
            article = tab.locator('.entry:not([hidden])')
            expect(article).to_have_attribute("data-path", "template/ds/树状数组.cpp")
            assert not tab.locator("#reader").evaluate("el => el.inert")
            assert tab.evaluate("document.documentElement.scrollWidth <= innerWidth")
            if width < 500:
                assert article.locator("pre").evaluate("el => el.scrollWidth > el.clientWidth")
                article.locator("pre").evaluate("el => el.scrollLeft = 100")
                assert article.locator("pre").evaluate("el => el.scrollLeft > 0")
                article.locator("pre").evaluate("el => el.scrollLeft = 0")
            article.locator(".copy-button").click()
            assert tab.evaluate("navigator.clipboard.readText()") == source
            if width == 390:
                tab.screenshot(path=str(args.screenshots / "mobile.png"))
            tab.locator(".menu-button").click()
            tab.locator("#search").fill("")
            tab.keyboard.press("Escape")
            expect(tab.locator(".menu-button")).to_have_attribute("aria-expanded", "false")
            mobile.close()
            print(f"PASS: {width}px mobile", flush=True)
        no_js = browser.new_context(java_script_enabled=False, viewport={"width": 1280, "height": 900})
        tab = no_js.new_page()
        tab.goto(args.url)
        assert tab.locator(".entry:visible").count() == len(expected)
        no_js.close()
        assert not errors, errors
        browser.close()
    print("PASS: exact code and real clipboard; fallback/failure; search/history; 320/390/768px mobile; no-JS reading")


if __name__ == "__main__":
    main()
