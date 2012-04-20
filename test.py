"""
Run test.py with 1 for atomic, 0 for non-AtomicWorker
"""
import sys
import threading

import atomic


atom = None


class Worker(threading.Thread):
    def run(self):
        global atom
        for _ in range(500):
            atom += 1


class AtomicWorker(threading.Thread):
    def run(self):
        global atom
        for _ in range(500):
            atom.increment_by(1)


def main(use_atomic):
    workers = {}
    global atom
    if use_atomic:
        atom = atomic.AtomicInt(0)
    else:
        atom = 0
    for x in range(10):
        if use_atomic:
            thread = AtomicWorker()
        else:
            thread = Worker()
        thread.start()
        workers[x] = thread
    for _, worker in workers.items():
        worker.join()
    global foo
    if use_atomic:
        print atom.number
    else:
        print atom

if __name__ == '__main__':
    sys.setcheckinterval(0)
    main(eval(sys.argv[1]))
