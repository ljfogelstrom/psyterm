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
    INIT_X = 1,
    INIT_Y = 1,
    INIT_W = 500,
    INIT_H = 300,
    /* dimensions can be abritrary values, wm will (should) resize */
    BORDER = 3,
};

enum Limits
{
    BUF_SIZE = 4096,
    /* ... */
};

void
draw(void) {
    return;
}

static struct Cursor {
    int x;
    int y;
    int w; 
    int h;
    _Bool isblinking;
    _Bool isfat;
    void(*draw);
} cursor = {
    0,
    0,
    2,
    12,
    0,
    0,
    draw,
};

static struct String {
    int x;
    int y;
} string = {
    INIT_X,
    FONT_H,
};


char buffer[BUF_SIZE]; /* input will be stored here */

static Display *dpy;
static int scr; 
static Window win;
static GC gc;
static XEvent ev;



void
draw_cursor(unsigned int x, unsigned int y, int visible)
{
    cursor.x = string.x + 2;
    cursor.y = string.y - 10;
    if (visible) {
	XFillRectangle(dpy, win, gc, cursor.x, cursor.y, cursor.w, cursor.h);
    } else {
	XClearArea(dpy, win, cursor.x, cursor.y, cursor.w, cursor.h, 0);
    }
}

void
carriage_return(void)
{
    string.x = INIT_X;
    string.y += FONT_H;
    // draw_cursor(0, 0, 1);
}

void
reset_screen(void) {
    string.x = INIT_X;
    string.y = INIT_Y;
    // draw_cursor(0, 0, 1);
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
	    string.x += 25; /* placeholder */
	    break;
	case '\f':
	    reset_screen();
	    break;
    }

    return 1;
}

int
main(void)
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

    unsigned char *composed = (unsigned char*)calloc(2048, sizeof(char));
    int i = 0;

    while (1) 
    {
	XNextEvent(dpy, &ev); /* wait */
	/* TODO: should add switch statement here
	 * should handle window resize events (so lines can wrap properly)
	 */
	XGetWindowAttributes(dpy, win, &return_attribs); /* will add this to resize event */
	if (ev.type == Expose) {
	    continue; /* only for testing currently */

	XDrawString(dpy, win, gc,
		string.x, string.y, test, strlen(test));

	} else if (ev.type == KeyPress) {

	    XLookupString(&ev.xkey, buffer, 4, &keysym, NULL); /* keycode must be converted to keysym */
	    fprintf(stderr, "%d\n", buffer[0]);
	    composed[i] = buffer[0];

	    if (composed[i] == '\r') {
		fprintf(stderr, "%s\n", composed);
		/* write_out */
		composed = (unsigned char*)calloc(2048, sizeof(char));
		i = 0;
	    } else {
		i++;
	    }


	    draw_cursor(string.x, string.y, 0);

	    if (handle_escape(buffer[0])) continue;
	    /* undraw cursor */
	    XDrawString(dpy, win, gc,
		    string.x, string.y, buffer, strlen(buffer));
	    string.x+=FONT_W;
	    draw_cursor(string.x, string.y, 1);

	    if (string.x > return_attribs.width) {
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
