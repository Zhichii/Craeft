# 千方百块 / Craeft 技术细节

## Utf8Wrapper

本程序使用Utf8Wrapper以此将UTF-8字符串转换成Unicode处理（实际上包装为个迭代器）。见[utf8.hpp](Server/tools/utf8.hpp)了解更多。

## CRON（Craeft Object Notation）

CRON 是 JSON 的扩展方言，兼容 JSON 标准，在 Craeft 中使用自定义解释器实现（位于[component.hpp](Server/component.hpp)与[component.cpp](Server/component.cpp)）。

CRON 中添加了“组件引用”，格式为`|<category>:<type>:<namespace>:<identifier>|`或`|<type>:<namespace>:<identifier>|`（注意不要加双引号）比如（非真实，仅举例）：
```cron
{
    "id": |thing:block:craeft:dirt|,
    "count":5
}
```
或者
```cron
{
    "texture": |texture:block:craeft:dirt|
}
```
其中"test"键对应的值为泥土方块的引用。

至于category和type有哪些，这还是看你的目录。我只是显式地语义化，规定好了。
