import sys
import os
import pathlib
import platform

import curses
from curses import wrapper


class Application:
    name:str            = ""
    path:str            = ""
    target_type:str     = ""
    generator:str       = ""
    clean_cache:str     = ""

def main(stdscr):

    stdscr = curses.initscr()
    
    curses.noecho()
    curses.cbreak()

    stdscr.keypad(True)


    begin_x = 20; begin_y = 7
    height = 5; width = 40
    win = curses.newwin(height, width, begin_y, begin_x)
    
    pad = curses.newpad(100, 100)
    # These loops fill the pad with letters; addch() is
    # explained in the next section
    for y in range(0, 99):
        for x in range(0, 99):
            pad.addch(y,x, ord('a') + (x*x+y*y) % 26)
    
    # Displays a section of the pad in the middle of the screen.
    # (0,0) : coordinate of upper-left corner of pad area to display.
    # (5,5) : coordinate of upper-left corner of window area to be filled
    #         with pad content.
    # (20, 75) : coordinate of lower-right corner of window area to be
    #          : filled with pad content.
    pad.refresh( 0,0, 5,5, 20,75)

    curses.nocbreak()
    stdscr.keypad(False)
    curses.echo()
    curses.endwin()



if __name__ == '__main__':
    wrapper(main)