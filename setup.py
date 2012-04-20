from setuptools import setup, Extension

atomic = Extension('atomic',
                   sources=['atomic_py.c'])

setup(name='AtomicPy',
      version='dev',
      ext_modules=[atomic]
      )
