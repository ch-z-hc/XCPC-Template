#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."

ulimit -s 262144
mkdir -p build/linux-verify
printf 'Stack limit: %s KiB\n' "$(ulimit -s)"

for source in template/*/*.cpp support/tools/contest_main.cpp support/tools/stress/*.cpp support/tools/stress/cases/*/*.cpp support/verify/*.cpp support/verify/oj/*.cpp support/verify/fixtures/*.cpp; do
    includes=()
    if [[ "$source" == template/math/Multipoint.cpp || "$source" == template/math/BostanMori.cpp ]]; then
        includes=(-include template/math/FPS.cpp)
    elif [[ "$source" == template/math/EquidistantLagrange.cpp ]]; then
        includes=(-include template/math/Combination.cpp)
    elif [[ "$source" == template/math/Stirling.cpp ]]; then
        includes=(-include template/math/NTT.cpp)
    elif [[ "$source" == template/graph/GomoryHu.cpp ]]; then
        includes=(-include template/graph/Dinic.cpp)
    elif [[ "$source" == template/graph/LowerBoundMinCostFlow.cpp ]]; then
        includes=(-include template/graph/MinCostFlow.cpp)
    elif [[ "$source" == template/string/Runs.cpp ]]; then
        includes=(-include template/string/SuffixArray.cpp)
    fi
    if [[ "$source" == template/geometry/* && "$source" != template/geometry/Point.cpp && "$source" != template/geometry/ConvexHull3D.cpp ]]; then
        includes=(-include template/geometry/Point.cpp -include template/geometry/Line.cpp)
        if [[ "$source" == template/geometry/Line.cpp ]]; then
            includes=(-include template/geometry/Point.cpp)
        elif [[ "$source" == template/geometry/MinCircle.cpp || "$source" == template/geometry/CircleUnion.cpp ]]; then
            includes+=(-include template/geometry/Circle.cpp)
        elif [[ "$source" == template/geometry/ConvexHull.cpp ]]; then
            includes+=(-include template/geometry/Polygon.cpp)
        elif [[ "$source" == template/geometry/ConvexQuery.cpp ]]; then
            includes+=(-include template/geometry/Polygon.cpp -include template/geometry/ConvexHull.cpp)
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
