## 2.1: BlackPixel, WhitePixel macros (for testing)
    other macros might be useful
    (Ignore pages 42-47 for now)
## Ch. 3: XCreateWindow, XDestroyWindow, XSetWindowBackground
    best option seems to be using XSetWindowAttributes in initial XCreateWindow call
    struct Visual: contains possible color mappings

Windows are invisible until mapped, using XMapWindow()
XMapWindow() generates an Expose (XEvent) for the mapped window if it is an InputOutput window
## Event types:
    Expose: generated for window by X server with intention to display the window
    KeyPress: key pressed (keyboard must be "grabbed"?)

Event types are (potentially) required to be masked with XSelectInput()

## Ch. 4.3: Properties and Atoms

## Ch. 6: Colors

## Ch. 7: Graphics Contexts
GCs are private structs which are used for most graphical operations
Windows and Pixmaps are collectively termed "drawables"
Of importance, the following variables can be set in XGCValues:
    - Function
    - Foreground and background pixel
    - Line style
    - Font
    - Exposure generation (referring to the Expose event)

### 7.2: Convienience Routines
XSetForeground(), Background, etc.

## Ch. 8: Graphics Functions
XFillRectangle() and etc.
Text drawing functions.

Note: Using same drawable and GC for consecutive calls will reduce server load
