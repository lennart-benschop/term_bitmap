# LIBRARY TO DRAW SIMPLE BITMAP GRAPHICS ON TERMINALS

If you long back to the days of the BBC Micro, where you could
immediately draw simple shapes in BASIC, using the built-in PLOT commands,
this library may be something for you.

This is the first version.

Only the tbm_out_fb.c file is truly dependent on Linux, the others might
as well work on other Unix-like systems.

## GETTING STARTED

The makefile is set up to run out of the box on Linux. Simply type:
```
make
```
Depending on the type of terminal emulator you have at hand, set the TBM_MODE
variable to one of the values 'kitty', 'sixel', 'fb', 'octant', 'sixel' or
'braille'.

The modes 'fb', 'kitty' and 'sixel' are true bitmap modes and curently support
up to 16 colours.
* kitty is available in the Kitty terminal, but also in several other
  modern terminal emulators, like KDE Konsole abd ghostty.
* sixel is available in the old xterm program, but it has to be invoked with
  the correct terminal option, like `xterm -ti vt340`. It is also available
  in several modern terminal emulators, such as KDE Konsole.
* fb is available on the Linux console (outside the graphic desktop).

The modes 'octant', 'sextant' and 'braille' are character modes, that
use special 2x4 or 2x3 block characters. These are currently
monochrome only.  These default to 160x96 or 160x72 bitmaps, but you
can change that in the program. By stretching the terminal window and
selecting a small font, 320x200 is feasible.
* The GNOME terminal works well with 'octant'. Most other terminals do
  not support octant mode (or incompletely). They might do so if you
  have a Unicode font with these characters included.
* Most terminal emulators will support sextant mode reasonably well. They
  render the relevant block characters internally, independent of the selected
  font.
* Braille uses the 2x4 dot patterns for Braille, that are included in
  most Unicode fonts. It depends heavily on the selected font.

Then run the test with:
```
./test_simple
```

