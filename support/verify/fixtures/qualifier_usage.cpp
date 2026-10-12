#include "../../../template/ds/吉司机线段树.cpp"
#include "../../../template/string/字符串哈希.cpp"
#include "../../../template/string/二维哈希.cpp"
#include "../../../template/graph/最小斯坦纳树.cpp"
#include "../../../template/graph/重赋权全源最短路.cpp"
#include "../../../template/graph/第几短游走.cpp"
#include "../../../template/math/多项式基本运算.cpp"
#include "../../../template/math/多项式多点求值与快速插值.cpp"

int main() {
    // 取地址并按引用传参，验证 static const 有实体定义；以 -O0 编译链接。
    V < const ll * > constants{&Beats::INF, &Hash::mod1, &Hash::mod2, &Hash::base,
        &Hash2D::Table::bx, &Hash2D::Table::by, &Steiner::inf, &Johnson::inf,
        &KShortestWalk::inf};
    for (auto p : constants) assert(*p > 0);
    assert(min(FPS::mod, Multipoint::mod) == 998244353);
    V < int > values(2, FPS::mod); assert(values[0] == Multipoint::mod);

    // 去掉 constexpr 不得引入 const 数据成员，导致对象无法赋值。
    Hash h("aba"), copy; copy = h;
    assert(copy.query(0, 0) == copy.query(2, 2));
    Hash2D a, b; a.build({"abc", "def"}); b = a;
    assert(b.query(0, 0, 1, 2) == a.query(0, 0, 1, 2));
    Beats st, other; st.init({0, 3, 1, 4}); other = st;
    assert(other.query(1, 3) == 8);
    Multipoint poly, saved; poly.build({0, 1, 2}); saved = poly;
    assert(saved.evaluate(FPS::mul({1, 1}, {1, 1})) == V < int > ({1, 4, 9}));
    string s = "aba"; h.build(s);
    assert(s == "aba");
    cout << "PASS: simple qualifiers, constant linkage, assignment and temporary inputs\n";
}
