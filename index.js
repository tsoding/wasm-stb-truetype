'use strict';

let app = document.getElementById("app");
let ctx = app.getContext("2d");
let wasm = null;

function cstrlen(mem, cstr_ptr) {
    let len = 0;
    while (mem[cstr_ptr] != 0) {
        cstr_ptr += 1;
        len += 1;
    }
    return len;
}

function cstr_by_ptr(ptr) {
    const buffer = wasm.instance.exports.memory.buffer;
    const mem = new Uint8Array(buffer);
    const len = cstrlen(mem, ptr);
    const bytes = new Uint8Array(buffer, ptr, len);
    return new TextDecoder().decode(bytes);
}

function platform_log(message_ptr) {
    ctx.fillStyle = "black";
    ctx.font = "32px Mono";
    ctx.textAlign = "left";
    ctx.fillText(cstr_by_ptr(message_ptr), 0, 100);
}

function platform_panic() {
    console.assert(false)
}

async function start() {
    wasm = await WebAssembly.instantiateStreaming(fetch('index.wasm'), {
        env: {
            platform_log,
            platform_panic,
            sqrt: Math.sqrt,
            floor: Math.floor,
            ceil: Math.ceil,
            fabs: Math.abs,
        }
    });
    wasm.instance.exports.memory.grow(100);
    const ttf_buffer = new Uint8Array(await (await fetch("./fonts/AnekLatin-Light.ttf")).arrayBuffer());
    const ttf_buffer_ptr = wasm.instance.exports.malloc(ttf_buffer.length);
    console.log("ttf_buffer_ptr", ttf_buffer_ptr);

    new Uint8Array(wasm.instance.exports.memory.buffer).set(ttf_buffer, ttf_buffer_ptr);

    wasm.instance.exports.app_init(ttf_buffer_ptr);
    const w_ptr = wasm.instance.exports.malloc(4);
    const h_ptr = wasm.instance.exports.malloc(4);
    const bitmap_ptr = wasm.instance.exports.app_render_char(w_ptr, h_ptr, 'a'.codePointAt());
    const w = new Int32Array(wasm.instance.exports.memory.buffer, w_ptr, 1)[0];
    const h = new Int32Array(wasm.instance.exports.memory.buffer, h_ptr, 1)[0];
    const imageData = new ImageData(new Uint8ClampedArray(wasm.instance.exports.memory.buffer, bitmap_ptr, w*h*4), w, h);
    ctx.putImageData(imageData, 0, 0);
}

start()
