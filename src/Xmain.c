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
#include <ctype.h>

#define FONT_H 12
#define FONT_W 7 /* placeholder until font support is added */

enum WindowMetrics 
{
    INIT_X = 0,
    INIT_Y = 10,
    INIT_W = 100,
    INIT_H = 200,
    /* dimensions can be abritrary values, wm will (should) resize */
    BORDER = 3,
};

enum Limits
{
    BUF_SIZE = 4096,
    /* ... */
};

static char buffer[BUF_SIZE]; /* input will be stored here */

static Display *dpy;
static int scr; 
static Window win;
static GC gc;
static XEvent ev;


static int stringx = 10;
static int stringy = 10;


void
carriage_return(void)
{
    stringx = INIT_X;
    stringy += FONT_H;
}

int
handle_escape(char chr)
{
    char c = chr;
    if (isprint(c)) return 0; /* doesn't support UTF-8 */
    switch (c)
    {
	case '\r':
	    carriage_return();
	    break;
	case '\n':
	    carriage_return();
	    break;
	case '\t':
	    stringx += 25; /* placeholder */
	    break;
    }
    return 1;
}

int
main(int argc, char *argv[])
{

    setlocale(LC_CTYPE, "C");
    char *test = "Exposed";

    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) err(EXIT_FAILURE, "XOpenDisplay: "); /* defaults to $DISPLAY */
    scr = DefaultScreen(dpy); /* only recommended for single screens */
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


    win = XCreateWindow(dpy, root,
	    INIT_X, INIT_Y, INIT_W, INIT_H, BORDER,
	    CopyFromParent, InputOutput, CopyFromParent,
	    CWBackPixmap|CWBackPixel,
	    &attribs);

    gc = XCreateGC(dpy, win, GCFunction|GCForeground|GCBackground, &gcvals);

    XStoreName(dpy, win, "psyterm");
    XMapWindow(dpy, win);

    /* logic section */
    XWindowAttributes return_attribs;
    XSelectInput(dpy, win, ExposureMask|KeyPressMask|KeyReleaseMask|
			    ButtonPressMask|ButtonReleaseMask|
			    StructureNotifyMask); /* listen for these events */

    unsigned int keycode = ev.xkey.keycode;
    KeySym keysym;

    while (1) 
    {
	XNextEvent(dpy, &ev); /* wait */
	/* TODO: should add switch statement here
	 * should handle window resize events (so lines can wrap properly)
	 */
	XGetWindowAttributes(dpy, win, &return_attribs); /* will add this to resize event */
	if (ev.type == Expose) {

	    XDrawString(dpy, win, gc,
		    stringx, stringy, test, strlen(test));

	} else if (ev.type == KeyPress) {

	    XLookupString(&ev.xkey, buffer, 4, &keysym, NULL); /* keycode must be converted to keysym */

	    if (handle_escape(buffer[0])) continue;

	    XDrawString(dpy, win, gc,
		    stringx, stringy, buffer, strlen(buffer));
	    stringx+=FONT_W;

	    if (stringx > return_attribs.width) {
		carriage_return();
	    }

	} else if (ev.type == ResizeRequest) {

	    XGetWindowAttributes(dpy, win, &return_attribs);
	    fprintf(stderr, "resized"); /* doesn't work */

	}
    }

    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);

    return 0;
}
