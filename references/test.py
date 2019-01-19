import os
import time
import curses

def main(win):
        win.nodelay(True)
        playerInput = ""
        win.clear()
        win.addstr("Detected key:")
        #clear = lambda: os.system("clear")

        while 1:
                try:
                        key = win.getkey()
                        win.clear()
                        win.addstr("Detected Key")
                        win.addstr(str(key))
                        if key == os.linsep:
                                break
                except Exception as e:
                        pass

curses.wrapper(main)
        

