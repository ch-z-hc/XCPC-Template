# Linux / Windows Git Bash：编译 gen、sol、bf 后运行 bash pai.sh。
# g++ -std=c++17 -O2 gen.cpp -o gen   # sol、bf 同理

set -e
for ((i=1; ; i++)); do
    ./gen "$i" > in
    ./sol < in > out
    ./bf < in > ans
    diff -Z ans out
done
