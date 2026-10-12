"""用 Python 任意精度整数核对李超树；覆盖 P4097 在线解码和 ll 中间量边界。"""
from pathlib import Path
import random
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def main():
    out = ROOT / "build/priority-verify"
    out.mkdir(parents=True, exist_ok=True)
    binary = out / "lichao_integer"
    subprocess.run([
        "g++", "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic",
        "-fsanitize=address,undefined", "-fno-sanitize-recover=all",
        str(ROOT / "support/verify/oj/lichao_integer.cpp"), "-o", str(binary),
    ], check=True)
    rng = random.Random(20261011)
    for exact in (False, True):
        for test in range(200):
            lines, operations, expected = [], [], []
            last = 0
            for step in range(120):
                if step % 3 != 2:
                    if exact:
                        # |x| <= 1e5、|y| <= 1e8，交叉乘积严格落在 ll 内。
                        x0, x1 = rng.randint(-100000, -20), rng.randint(20, 100000)
                        y0, y1 = rng.randint(-10**8, 10**8), rng.randint(-10**8, 10**8)
                        if step % 7 == 0:
                            x0, x1 = rng.randint(-20, 20), rng.randint(-20, 20)
                    else:
                        x0, x1 = rng.randint(1, 39989), rng.randint(1, 39989)
                        y0, y1 = rng.randint(1, 10**9), rng.randint(1, 10**9)
                        if step % 7 == 0:
                            x1, y1 = x0, y0
                        if step % 11 == 0:
                            x0, y0, x1, y1 = 1, 1, 24, 1
                        if step % 11 == 1:
                            x0, y0, x1, y1 = 1, 10**9, 24, 1
                    actual = (x0, y0, x1, y1)
                    if not exact:
                        actual = ((x0 - last - 1) % 39989 + 1,
                                  (y0 - last - 1) % 10**9 + 1,
                                  (x1 - last - 1) % 39989 + 1,
                                  (y1 - last - 1) % 10**9 + 1)
                    operations.append("1 " + " ".join(map(str, actual)))
                    if x0 > x1:
                        x0, x1, y0, y1 = x1, x0, y1, y0
                    lines.append((x0, y0, x1, y1))
                else:
                    x = rng.randint(-20, 20) if exact else rng.randint(1, 39989)
                    if not exact and step % 11 == 2:
                        x = 24
                    arg = x if exact else (x - last - 1) % 39989 + 1
                    operations.append(f"0 {arg}")
                    best, num, den = 0, 0, 1
                    for idx, (x0, y0, x1, y1) in enumerate(lines, 1):
                        if not x0 <= x <= x1:
                            continue
                        a = max(y0, y1) if x0 == x1 else (x1 - x) * y0 + (x - x0) * y1
                        b = 1 if x0 == x1 else x1 - x0
                        if not best or a * den > num * b:
                            best, num, den = idx, a, b
                    last = best
                    expected.append(best)
            data = str(len(operations)) + "\n" + "\n".join(operations) + "\n"
            run = subprocess.run([str(binary)] + (["exact"] if exact else []),
                                 input=data, capture_output=True, text=True, timeout=20)
            assert run.returncode == 0, (exact, test, run.stderr)
            assert list(map(int, run.stdout.split())) == expected, (exact, test)
    print("PASS: Li Chao, 200 online-decoded + 200 ll-boundary cases, Python exact oracle")


if __name__ == "__main__":
    main()
