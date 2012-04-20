from setuptools import setup, Extension

atomic = Extension('atomic',
                   sources=['C/atomic.c'],
                   )

setup(name='AtomicPy',
      version='dev',
      ext_modules=[atomic]
      )
