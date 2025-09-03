#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
/*#include <X11/Xft/Xft.h> use this for installing fonts, need to fix includes */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <err.h>
#include <string.h>

enum WindowMetrics 
{
    INIT_X = 10,
    INIT_Y = 10,
    INIT_W = 100,
    INIT_H = 200,
    /* dimensions can be abritrary values, wm will (should) resize */
    BORDER = 3,
};


int
main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, "sv_SE");
    XEvent ev; /* used in main loop */
    char buffer[4] = "\0\0\0";

    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) err(EXIT_FAILURE, "XOpenDisplay: "); /* defaults to $DISPLAY */
    int scr = DefaultScreen(dpy); /* only recommended for single screens */
    Window root = RootWindow(dpy, scr);


    /* graphics section */
    XSetWindowAttributes attribs = {
	.background_pixmap = ParentRelative, /* must not be 'None'; is overwritten by pixel */
	.background_pixel = BlackPixel(dpy, scr)
    };

    XGCValues gcvals = {
	.function = GXcopy,
	.foreground = WhitePixel(dpy, scr),
	.background = WhitePixel(dpy, scr),
    };


    Window win = XCreateWindow(dpy, root,
	    INIT_X, INIT_Y, INIT_W, INIT_H, BORDER,
	    CopyFromParent, InputOutput, CopyFromParent,
	    CWBackPixmap|CWBackPixel,
	    &attribs);

    GC gc = XCreateGC(dpy, win, GCFunction|GCForeground|GCBackground, &gcvals);

    XStoreName(dpy, win, "psyterm");


    /* logic section */
    XSelectInput(dpy, win, ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask); /* listen for these events */
    XMapWindow(dpy, win);

    int stringx = 10;
    int stringy = 10;

    while (1) 
    {

	XNextEvent(dpy, &ev); /* wait */
	/* TODO: should add switch statement here
	 * should handle window resize events (so lines can wrap properly)
	 */
	if (ev.type == Expose) {
	    XFillRectangle(dpy, win, gc,
		    11, 10, 100, 10);
	    XDrawString(dpy, win, gc,
		    stringx, stringy, buffer, strlen(buffer));
	    XFlush(dpy);
	} else if (ev.type == KeyPress) {
	    unsigned int keycode = (unsigned int)ev.xkey.keycode;
	    KeySym keysym;
	    XLookupString(&ev.xkey, buffer, 4, &keysym, NULL);
	    /* keycode must be converted to keysym */
	    fprintf(stderr, "%lu\n", keysym);
	    XDrawString(dpy, win, gc,
		    stringx, stringy, buffer, strlen(buffer));
	    XFlush(dpy);
	}
	stringx+=5; stringy+=0;
    }

    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    return 0;
}
