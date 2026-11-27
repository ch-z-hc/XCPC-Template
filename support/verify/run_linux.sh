#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."

ulimit -s 262144
mkdir -p build/linux-verify
printf 'Stack limit: %s KiB\n' "$(ulimit -s)"

for source in template/*/*.cpp support/tools/contest_main.cpp support/tools/stress/*.cpp support/tools/stress/cases/*/*.cpp support/verify/*.cpp support/verify/oj/*.cpp support/verify/fixtures/*.cpp; do
    includes=()
    if [[ "$source" == template/math/多项式多点求值与快速插值.cpp || "$source" == template/math/线性递推快速求项.cpp ]]; then
        includes=(-include template/math/多项式基本运算.cpp)
    elif [[ "$source" == template/math/等距拉格朗日插值.cpp ]]; then
        includes=(-include template/math/组合数.cpp)
    elif [[ "$source" == template/math/两类斯特林数.cpp ]]; then
        includes=(-include template/math/快速数论变换与多项式乘法.cpp)
    elif [[ "$source" == template/ds/线段树分治.cpp ]]; then
        includes=(-include template/ds/可撤销并查集.cpp)
    elif [[ "$source" == template/graph/最小割树.cpp ]]; then
        includes=(-include template/graph/最大流.cpp)
    elif [[ "$source" == template/graph/上下界最小费用流.cpp ]]; then
        includes=(-include template/graph/势能最短路费用流.cpp)
    elif [[ "$source" == template/string/极大周期子串.cpp ]]; then
        includes=(-include template/string/后缀数组.cpp)
    elif [[ "$source" == template/string/后缀自动机双向扩展.cpp ]]; then
        includes=(-include template/string/后缀自动机.cpp)
    elif [[ "$source" == template/string/回文半长后缀与最少分割.cpp ]]; then
        includes=(-include template/string/回文自动机.cpp)
    elif [[ "$source" == template/math/多项式微积分与转置卷积.cpp ]]; then
        includes=(-include template/math/多项式基本运算.cpp)
    elif [[ "$source" == template/math/多项式平移与等比求值.cpp ]]; then
        includes=(-include template/math/组合数.cpp -include template/math/多项式基本运算.cpp)
    elif [[ "$source" == template/math/多项式幂投影.cpp ]]; then
        includes=(-include template/math/多项式基本运算.cpp)
    elif [[ "$source" == template/math/分块快速阶乘.cpp ]]; then
        includes=(-include template/math/多项式基本运算.cpp -include template/math/多项式多点求值与快速插值.cpp)
    elif [[ "$source" == template/math/拆系数浮点卷积.cpp ]]; then
        includes=(-include template/math/快速傅里叶变换与任意模数卷积.cpp)
    elif [[ "$source" == template/math/半在线子集卷积.cpp ]]; then
        includes=(-include template/math/快速沃尔什变换.cpp)
    elif [[ "$source" == template/math/有标号图计数.cpp ]]; then
        includes=(-include template/math/组合数.cpp -include template/math/多项式基本运算.cpp)
    fi
    if [[ "$source" == template/geometry/* && "$source" != template/geometry/点与向量.cpp && "$source" != template/geometry/整点凸壳与闵可夫斯基和.cpp && "$source" != template/geometry/三维凸包.cpp ]]; then
        includes=(-include template/geometry/点与向量.cpp -include template/geometry/直线与线段.cpp)
        if [[ "$source" == template/geometry/直线与线段.cpp ]]; then
            includes=(-include template/geometry/点与向量.cpp)
        elif [[ "$source" == template/geometry/最小圆覆盖.cpp || "$source" == template/geometry/圆面积并与周长并.cpp ]]; then
            includes+=(-include template/geometry/圆.cpp)
        elif [[ "$source" == template/geometry/凸包与直径与闵可夫斯基和.cpp ]]; then
            includes+=(-include template/geometry/多边形.cpp)
        elif [[ "$source" == template/geometry/二维动态凸包.cpp ]]; then
            includes=(-include template/geometry/二维凸包最大点积查询.cpp)
        elif [[ "$source" == template/geometry/凸多边形包含查询.cpp ]]; then
            includes+=(-include template/geometry/多边形.cpp -include template/geometry/凸包与直径与闵可夫斯基和.cpp)
        fi
    fi
    g++ -std=c++17 -Wall -Wextra -pedantic "${includes[@]}" -fsyntax-only "$source"
done

count=0
for source in support/verify/*.cpp; do
    name="${source##*/}"
    name="${name%.cpp}"
    if [[ "$name" == mcmf_submission_standalone || "$name" == *_submission ]]; then
        continue
    fi
    printf 'RUN %s\n' "$name"
    g++ -std=c++17 -O2 -g -Wall -Wextra -pedantic -D_GLIBCXX_ASSERTIONS \
        -fsanitize=address,undefined -fno-sanitize-recover=all \
        "$source" -o "build/linux-verify/$name"
    "build/linux-verify/$name"
    count=$((count + 1))
done
printf 'PASS: %s verification programs\n' "$count"
python3 support/verify/check_lichao_integer.py
python3 support/verify/print_usage_check.py
python3 support/verify/stress_tools_check.py
