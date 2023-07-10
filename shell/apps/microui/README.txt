
This microui has main.c clean. All to be ported is into renderer.c.
We can replace the code of every function with our gui code.
We will not use atlas[] nor push_quad() probably.

Another different renderer (it does not use libsdl)
https://github.com/ghaerr/microui/blob/master/demo-nano-X/renderer.c
I like on that r_draw_icon(): it uses just r_draw_text() to do the icons!... 
probably we can copy and paste this r_draw_icon from this renderer to our 
renderer and it will work (not sure, but probably).


