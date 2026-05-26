#include <xinu.h>
#include <microui.h>
#include <gui_buf.h>
#include <gui.h>

#define DOOM_IMPLEMENTATION
#ifdef VERSION
#undef VERSION
#endif
#include "PureDOOM.h"

#include "wad_data.h"   // unsigned char wad_data[]; unsigned int wad_data_len;

uint32 *buf_doom;
int n_doom_window;

// ─── Malloc/Free ─────────────────────────────────────────────────────────────

typedef struct {
    uint32 size;
    unsigned char data[];
} mem_block_t;

static void* my_malloc(int size)
{
    mem_block_t* block = (mem_block_t*)getmem(sizeof(mem_block_t) + (uint32)size);
    block->size = (uint32)size;
    return block->data;
}

static void my_free(void* ptr)
{
    mem_block_t* block = (mem_block_t*)((unsigned char*)ptr - sizeof(mem_block_t));
    freemem((char*)block, sizeof(mem_block_t) + block->size);
}

// ─── File I/O desde array en RAM ─────────────────────────────────────────────

typedef struct {
    const unsigned char* data;
    unsigned int size;
    unsigned int pos;
} mem_file_t;

static mem_file_t wad_file;

static void* my_open(const char* filename, const char* mode)
{
    if (mode[0] == 'w') return NULL;

    const char* p = filename;
    while (*p) {
        if (p[0]=='d' && p[1]=='o' && p[2]=='o' && p[3]=='m' && p[4]=='1')
        {
            wad_file.data = wad_data;
            wad_file.size = wad_data_len;
            wad_file.pos  = 0;
            return &wad_file;
        }
        p++;
    }
    return NULL;
}

static void my_close(void* handle) {}

static int my_read(void* handle, void* buf, int count)
{
    if (handle == (void*)1) return 0;
    mem_file_t* f = (mem_file_t*)handle;
    int remaining = (int)(f->size - f->pos);
    if (count > remaining) count = remaining;
    memcpy(buf, f->data + f->pos, count);
    f->pos += count;
    return count;
}

static int my_write(void* handle, const void* buf, int count)
{
    return count;
}

static int my_seek(void* handle, int offset, doom_seek_t origin)
{
    if (handle == (void*)1) return 0;
    mem_file_t* f = (mem_file_t*)handle;
    int newpos;
    if      (origin == DOOM_SEEK_SET) newpos = offset;
    else if (origin == DOOM_SEEK_CUR) newpos = (int)f->pos + offset;
    else /* DOOM_SEEK_END */          newpos = (int)f->size + offset;
    if (newpos < 0) newpos = 0;
    if (newpos > (int)f->size) newpos = (int)f->size;
    f->pos = (unsigned int)newpos;
    return 0;
}

static int my_tell(void* handle)
{
    if (handle == (void*)1) return 0;
    return (int)((mem_file_t*)handle)->pos;
}

static int my_eof(void* handle)
{
    if (handle == (void*)1) return 1;
    mem_file_t* f = (mem_file_t*)handle;
    return f->pos >= f->size;
}

// ─── Tiempo ──────────────────────────────────────────────────────────────────

static void my_gettime(int* sec, int* usec)
{
    uint32 ticks = get_ticks();
    *sec  = (int)(ticks / 1000);
    *usec = (int)((ticks % 1000) * 1000);
}

// ─── Print ───────────────────────────────────────────────────────────────────

static void my_print(const char* str)
{
    printf("%s", str);
}

// ─── Exit ────────────────────────────────────────────────────────────────────

static void my_exit(int code)
{
    while(1) {}
}

// ─── getenv ──────────────────────────────────────────────────────────────────

static char* my_getenv(const char* var) { return NULL; }

// ─── Video ───────────────────────────────────────────────────────────────────

#define SCREEN_W 320
#define SCREEN_H 200


static void render_frame(void)
{
    const unsigned char* fb = doom_get_framebuffer(4);

    gui_buf_draw_image(buf_doom, SCREEN_W, 0, 0, SCREEN_W, SCREEN_H, (uint32*)fb);
    gui_signal_redraw(n_doom_window);

}

// ─── Input ───────────────────────────────────────────────────────────────────

static doom_key_t xinu_to_doom_key(char c)
{
    switch (c) {
        case 'w':  return DOOM_KEY_UP_ARROW;
        case 's':  return DOOM_KEY_DOWN_ARROW;
        case 'a':  return DOOM_KEY_LEFT_ARROW;
        case 'd':  return DOOM_KEY_RIGHT_ARROW;
        case ' ':  return DOOM_KEY_SPACE;
        case '\n': return DOOM_KEY_ENTER;
        case 27:   return DOOM_KEY_ESCAPE;
        default:
            if (c >= 'a' && c <= 'z') return (doom_key_t)c;
            if (c >= '0' && c <= '9') return (doom_key_t)c;
            return DOOM_KEY_UNKNOWN;
    }
}

static void handle_input(void)
{
    mu_event_t e;
    mu_get_event(n_doom_window, &e);
    if (e.c[0] != '\0') {
        doom_key_t key = xinu_to_doom_key(e.c[0]);
        if (key != DOOM_KEY_UNKNOWN) {
            doom_key_down(key);
            doom_key_up(key);
        }
    }
}

// ─── Main ────────────────────────────────────────────────────────────────────

/*
void doom_main(void)
{
    doom_set_print(my_print);
    doom_set_malloc(my_malloc, my_free);
    doom_set_file_io(my_open, my_close, my_read, my_write,
                     my_seek, my_tell, my_eof);
    doom_set_gettime(my_gettime);
    doom_set_exit(my_exit);
    doom_set_getenv(my_getenv);

    doom_set_resolution(SCREEN_W, SCREEN_H);

    int flags = DOOM_FLAG_HIDE_MOUSE_OPTIONS |
                DOOM_FLAG_HIDE_SOUND_OPTIONS  |
                DOOM_FLAG_HIDE_MUSIC_OPTIONS;

    char* doom_argv[] = { "doom" };
    doom_init(1, doom_argv, flags);

    while (1)
    {
        handle_input();
        doom_update();
        render_frame();
        sleepms(28);
    }
}
*/



#define FRAME_SPACE_W 10
#define FRAME_SPACE_H 35

process xinu_doom(void)
{

    doom_set_print(my_print);
    doom_set_malloc(my_malloc, my_free);
    doom_set_file_io(my_open, my_close, my_read, my_write,
                     my_seek, my_tell, my_eof);
    doom_set_gettime(my_gettime);
    doom_set_exit(my_exit);
    doom_set_getenv(my_getenv);

    doom_set_resolution(SCREEN_W, SCREEN_H);

    int flags = DOOM_FLAG_HIDE_MOUSE_OPTIONS |
                DOOM_FLAG_HIDE_SOUND_OPTIONS  |
                DOOM_FLAG_HIDE_MUSIC_OPTIONS;

    char* doom_argv[] = { "doom" };
    doom_init(1, doom_argv, flags);

        buf_doom = gui_buf_getmem(SCREEN_W*SCREEN_H*4);
        //gui_buf_draw_image(buf, SCREEN_W, 0, 0, SCREEN_W, SCREEN_H, nina);
        n_doom_window = mu_add_win("Demo doom", 700, 40, SCREEN_W, SCREEN_H, buf_doom);

        /* program source code (for example, modify surface
         * drawn into window
         */

        for (;;) {

        	handle_input();
        	doom_update();
        	render_frame();
		   for (int j=0; j<320; j++) {

                        gui_buf_pixel(buf_doom, SCREEN_W, j, 100, 40000);
                }


		//printf("render\n\r");
        	// sleepms(28);
        };

        gui_buf_freemem(buf_doom, SCREEN_W*SCREEN_H*4);
        mu_free_win(n_doom_window) ;
}


