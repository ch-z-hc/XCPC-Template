#!/usr/bin/env bash
# 默认运行最大子段和；也可传入生成器、暴力、待测程序的源码路径。
set -u
tests=${TESTS:-0}
[[ "$tests" =~ ^(0|[1-9][0-9]{0,8})$ ]] || { printf 'TESTS 必须在 0..999999999 内\n'; exit 2; }
base=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
ulimit -s 262144 || exit 2
mkdir -p build/stress
work=$(mktemp -d "$PWD/build/stress/manual.XXXXXX") || exit 2
printf '工作目录：%s\n' "$work"
trap 'printf "\n已停止，现场保留在 %s\n" "$work"; exit 130' INT TERM

g++ -std=c++17 -O2 -Wall -Wextra -pedantic "${1:-$base/generator.cpp}" -o "$work/gen" || exit 2
g++ -std=c++17 -O2 -Wall -Wextra -pedantic "${2:-$base/brute.cpp}" -o "$work/brute" || exit 2
g++ -std=c++17 -O2 -Wall -Wextra -pedantic "${3:-$base/standard.cpp}" -o "$work/a" || exit 2

# 改 seed 可复现；改 timeout 的秒数可调整每轮时限。
for ((seed = 1; tests == 0 || seed <= tests; ++ seed)); do
    printf '%s\n' "$seed" > "$work/seed.txt"
    if timeout -k 1s 2s "$work/gen" "$seed" > "$work/input.txt" 2> "$work/gen.err"; then :
    else printf 'seed=%s，生成器异常，退出码 %s\n' "$seed" "$?"; exit 2; fi
    if timeout -k 1s 2s "$work/brute" < "$work/input.txt" > "$work/brute.out" 2> "$work/brute.err"; then :
    else printf 'seed=%s，暴力异常，退出码 %s\n' "$seed" "$?"; exit 2; fi
    if timeout -k 1s 2s "$work/a" < "$work/input.txt" > "$work/a.out" 2> "$work/a.err"; then :
    else printf 'seed=%s，待测程序异常，退出码 %s\n' "$seed" "$?"; exit 2; fi
    if diff -wB "$work/brute.out" "$work/a.out" > "$work/diff.txt"; then
        printf 'OK %s\n' "$seed"
    else
        printf 'seed=%s，答案不同，现场：%s\n' "$seed" "$work"
        exit 1
    fi
done
