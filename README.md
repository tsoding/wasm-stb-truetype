# stb_truetype.h compiled to WASM

A small experiment on doing all of the font rendering inside of the WASM module without depending on font rendering capabilities of the browser itself.

Big thanks to [Sean Barrett](https://github.com/nothings) for making [stb_truetype.h](https://github.com/nothings/stb/blob/master/stb_truetype.h) which enabled us to easily do that.

## Quick Start

```console
$ ./build.sh
$ python -m http.server 6969
$ iexplore.exe http://localhost:6969/
```
