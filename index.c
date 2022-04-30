#define NULL ((void*)0)

typedef unsigned long size_t;

void platform_panic(const char *file_path, int line, const char *message);
void platform_log(const char *message);

float floor(float x);
float pow(float x, float y);
float ceil(float x);
float sqrt(float x);
float fmod(float x, float y);
float cos(float x);
float acos(float x);
float fabs(float x);

size_t strlen(const char *s)
{
    size_t len = 0;
    while (*s++ != '\0') len++;
    return len;
}

extern unsigned char __heap_base;

static unsigned char *bump_pointer = &__heap_base;

void *memset(void *s, int c, size_t n)
{
    unsigned char *si = s;
    while (n-- > 0) {
        *si++ = c;
    }
    return s;
}

void *malloc(size_t size)
{
    void *result = bump_pointer;
    bump_pointer += size;
    return result;
}

void free(void *ignored)
{
    (void) ignored;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *desti = dest;
    const unsigned char *srci = src;
    while (n-- > 0) {
        *desti++ = *srci++;
    }
    return dest;
}

#define STBTT_ifloor(x)   ((int) floor(x))
#define STBTT_iceil(x)    ((int) ceil(x))
#define STBTT_sqrt(x)      sqrt(x)
#define STBTT_pow(x,y)     pow(x,y)
#define STBTT_fmod(x,y)    fmod(x,y)
#define STBTT_cos(x)       cos(x)
#define STBTT_acos(x)      acos(x)
#define STBTT_fabs(x)      fabs(x)
#define STBTT_malloc(x,u)  ((void)(u),malloc(x))
#define STBTT_free(x,u)    ((void)(u),free(x))
#define STBTT_strlen(x)    strlen(x)
#define STBTT_memcpy       memcpy
#define STBTT_memset       memset
#define STBTT_assert(cond) do { if (!(cond)) { platform_panic(__FILE__, __LINE__, "Assertion Failed"); } } while(0)

#define STB_TRUETYPE_IMPLEMENTATION
#include "./stb_truetype.h"

static stbtt_fontinfo font;

void *app_render_char(int *w, int *h, int c)
{
    unsigned char *bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, 200), c, w, h, 0, 0);

    unsigned int *result = malloc((*w) * (*h));

    for (int y = 0; y < *h; ++y) {
        for (int x = 0; x < *w; ++x) {
            result[y*(*w) + x] = bitmap[y*(*w) + x]<<(8*3) | 0x0000FF;
        }
    }
    return result;
}

void app_init(void *ttf_buffer)
{
    stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer,0));
}
