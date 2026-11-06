"""识别代码中的函数与语句范围，供 PDF 分页使用；不修改原文。"""
from functools import lru_cache

from pygments.lexers import CppLexer
from pygments.token import Keyword
from tree_sitter import Language, Parser
import tree_sitter_cpp


@lru_cache(maxsize=256)
def code_boundaries(source, path):
    if not path.endswith(".cpp"):
        return ()
    # 当前 C++ grammar 会误判 unsigned(expr)；只在解析副本中替换等长类型名。
    parsed = list(source)
    for offset, token, value in CppLexer().get_tokens_unprocessed(source):
        if token in Keyword.Type and value == "unsigned" and source[offset + 8:].lstrip().startswith("("):
            parsed[offset:offset + 8] = "uint32_t"
    tree = Parser(Language(tree_sitter_cpp.language())).parse("".join(parsed).encode())
    if tree.root_node.has_error:
        raise ValueError(f"{path}: 无法可靠识别代码边界，停止生成 PDF")
    spans = set()
    stack = [tree.root_node]
    while stack:
        node = stack.pop()
        stack.extend(node.named_children)
        function = node.type in ("function_definition", "lambda_expression")
        if node.type == "compound_statement":
            continue
        if not (function or node.type.endswith("_statement") or node.type in
                ("declaration", "field_declaration", "comment", "string_literal", "raw_string_literal")):
            continue
        start, end = node.start_point.row, node.end_point.row + 1
        if end - start > 1:
            spans.add((start, end, function))
        if function:
            first = node
            if first.parent and first.parent.type == "template_declaration":
                first = first.parent
            start = first.start_point.row
            previous = first.prev_named_sibling
            while previous and previous.type == "comment" and previous.end_point.row + 1 == start:
                start = previous.start_point.row
                previous = previous.prev_named_sibling
            if start < node.start_point.row:
                spans.add((start, end, False))
    return tuple(sorted(spans))
