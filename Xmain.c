#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xft/Xft.h> /* use this for installing fonts, need to fix includes */
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

enum WindowMetrics {
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
    XEvent ev; /* used in main loop */
    const char* buffer = "Hello and welcome 2 my awsum program!!!";

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

    XStoreName(dpy, win, "pterm");


    /* logic section */
    XSelectInput(dpy, win, ExposureMask|KeyPressMask); /* listen for these events */
    XMapWindow(dpy, win);

    int stringx = 10;
    int stringy = 10;

    while (1) {

	XNextEvent(dpy, &ev);
	if (ev.type == Expose) {
	    XFillRectangle(dpy, win, gc,
		    100, 10, 100, 10);
	    XDrawString(dpy, win, gc,
		    stringx, stringy, buffer, strlen(buffer));
	    XFlush(dpy);
	} else if (ev.type == KeyPress) {
	    XDrawString(dpy, win, gc,
		    stringx, stringy, "Press!", 6);
	    XFlush(dpy);
	}
	stringx+=0; stringy+=10;
    }

    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    return 0;
}
