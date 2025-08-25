# 千方百块 / Craeft 技术细节

服务器程序于[main.cpp](Server/main.cpp)开始运行。

编译服务器请在Server目录下带着`all`参数运行[`build.py`](Server/build.py)（比如我这里是`python3 build.py all`）。CMake太高深不想用，不如直接用G++。

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

## Craeft Script（CRS）

~~有个人建议我叫美西螈语言，于是我就起了一个别名“螈代码”，也可以叫AxolotL。~~
脚本用于书写模组的逻辑，如实体的tick()等。
需要注意的是，区块等的加载为硬编码，若要优化请直接修改源代码~~或者帮我改~~。
内置类型有：

### `PositionD` 浮点数位置

为避免距离现象，游戏会将全局坐标转换为区块坐标。添加该类型是为了自动处理转换，

该类型使用一个指针记录对应区块，三个[`FixedShort192`](Server/tools/fixed.hpp)类型记录相对位置，仅显示时转换为全局的样子。

~~而且你改的时候直接改实现（比如删去转换）就能在16777216.0走不动路了。~~

### `PositionI` 整数位置

添加理由同上。

该类型使用一个指针记录对应区块，三个`short`类型记录相对位置。

### `Object` 对象

### 其它

- **`Bool` 布尔值**：除0以外的值均变为1。
- **`Byte` 有符号8位整数**：对应`int8_t`。请自行处理溢出。
- **`UByte` 无符号8位整数**：对应`uint8_t`。
- **`Short` 有符号16位整数**：对应`int16_t`。同上，请自行处理溢出。
- **`UShort` 无符号16位整数**：对应`uint16_t`。
- **`Int` 有符号32位整数**：对应`int32_t`。同上。
- **`UInt` 无符号32位整数**：对应`uint32_t`。
- **`Long` 有符号64位整数**：对应`int64_t`。同上。
- **`ULong` 无符号64位整数**：对应`uint64_t`。
- **`Fixed6` 定点数，比例为10的6次方。**：对应[`Fixed6`](Server/tools/fixed.hpp)。同上。
- **`Float` 32位浮点数**：对应`float`。请自行处理精度丢失。
- **`Double` 64位浮点数**：对应`double`。同上
- **`String` 字符串**：对应`std::string`。
- **`Identifier` 标识符**：对应[`Identifier`](Server/thing.hpp)。
