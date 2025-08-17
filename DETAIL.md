# 千方百块 / Craeft 技术细节

服务器程序于[main.cpp](Server/main.cpp)开始运行。

编译请运行[build.sh](build.sh)或[build,bat](build.bat)。CMake太高深不想用，不如直接用G++。

## Utf8Wrapper

本程序使用Utf8Wrapper以此将UTF-8字符串转换成Unicode处理（实际上包装为个迭代器）。只读。见[utf8.hpp](Server/utf8.hpp)和[utf8.cpp](Server/utf8.cpp)了解更多。

## JSON 约定

对于所有 JSON 中涉及到“名称”“提示”之类的，一律允许以下格式：

`"你好世界！"`

`{"translation":"hello.world"}`

`[{"translation":"hello"},{"translation":"world"}]`

```json
{
    "translation": "greeting_someone",
    "args": {
        "someone": {"translation": "world"}
    }
}
```
注：（此处仅举例，非真实）"greeting_someone"对应的本地化字符串是"你好{someone}！"

`{"text":"你好世界！","color":1131796}`

```json
{
    "translation": "greeting_someone", "color": "#114514",
    "args": {
        "someone": [
            {"image": "creaft.gui.icon.earth"},
            {"translation": "world", "color": "#309eff", "bold": true, "underline": true, "italic": true, "strike": false}
        ]
    }
}
```