<div align="right">

**[English](README.md)** · [简体中文](README.zh-CN.md)

</div>

# qt-learning

Qt 6 exercises, from an empty window to TCP client/server and threading.

<sub>C++ · Qt 6 · 7 projects</sub>

---

## What it holds

Seven standalone Qt projects, each one step along a learning path:

```
01 → Qtimer / Timer → file → calculate → TCPclient ⇄ TPCservice
```

| Project | Topic |
|---|---|
| `01` | window and custom widget basics |
| `Qtimer` / `Timer` | timers — two different Qt APIs for the same result |
| `file` | file read/write and dialogs |
| `calculate` | hand-written expression parsing |
| `TCPclient` | `QTcpSocket` client with a chat window |
| `TPCservice` | `QTcpServer` plus `QThread` |

## Two things worth reading

**`calculate`** — parsing without a regex library. The loop that looks for an operator starts at index 1 rather than 0, because `-3+5` has a minus sign at position 0 that is not an operator. Starting at 0 misreads it, leaves an empty left operand, and the conversion fails.

**`TPCservice`** — a threading issue worth studying. The socket is created on the main thread and the `QThread` object lives there too, so the connection established in `run()` actually delivers on the main thread. The notes explain why, and what the correct `moveToThread` shape looks like.

## Building

```bash
cd calculate
qmake calculate.pro
make          # or mingw32-make / nmake
```

Requires Qt 6 with Core, Gui, Widgets and Network.

## License

MIT
