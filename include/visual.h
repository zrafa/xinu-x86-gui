#include <microui.h>

struct SpawnedWindow {
  int width;
  int height;
  mu_Color color;
  int graphic;
};

struct GraphicsWindow {
  int width;
  int height;
  int pictureNumber;
};

void spawned_window(struct SpawnedWindow, mu_Context *);
void graphics_window(struct GraphicsWindow, mu_Context *);
struct SpawnedWindow mu_visual(mu_Context *);
void mu_graphics(mu_Context *);




