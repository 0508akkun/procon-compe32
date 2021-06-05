import threading
import time
import colorama
from colorama import Fore, Back, Style

def spinner(func):
    def checker(*args, **kwargs):
        def rotate_bar(bar="|/-\\", interval=0.12):
            index = 0
            while running:
                box = '[' + Fore.BLUE + bar[index] + Fore.RESET + '] '
                print((len(box)+len(func.__name__)+7)*'\b' + box + 'RUN: ' + func.__name__ + '()', end="", flush=True)
                time.sleep(interval)
                index = (index + 1) % len(bar)
            done = '[' + Fore.BLUE + 'o' + Fore.RESET + '] '
            print((len(done)+len(func.__name__)+7)*'\b' + done + 'RUN: ' + func.__name__ + '()', flush=True)
        running = True
        g = threading.Thread(target=rotate_bar)
        g.start()
        t = threading.Thread(target=func, args=args, kwargs=kwargs)
        t.start()
        t.join()
        running = False
        g.join()
        print(kwargs)
    return checker