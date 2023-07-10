/*
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>
*/
#include "xinu.h"
#include "renderer.h"
#include "atlas.inl"

#define BUFFER_SIZE 16384

/*
static GLfloat   tex_buf[BUFFER_SIZE *  8];
static GLfloat  vert_buf[BUFFER_SIZE *  8];
static GLubyte color_buf[BUFFER_SIZE * 16];
static GLuint  index_buf[BUFFER_SIZE *  6];
*/

static int width  = 800;
static int height = 600;
static int buf_idx;

/*
static SDL_Window *window;
*/


void r_init(void) {
}


static void flush(void) {
}


static void push_quad(mu_Rect dst, mu_Rect src, mu_Color color) {
}


void r_draw_rect(mu_Rect rect, mu_Color color) {
}


void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color) {
}


void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
}


int r_get_text_width(const char *text, int len) {
}


int r_get_text_height(void) {
  return 18;
}


void r_set_clip_rect(mu_Rect rect) {
}


void r_clear(mu_Color clr) {
}


void r_present(void) {
}


void r_handle_input(mu_Context *ctx)
{
}
