from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

d = generate_distutils_setup(
    packages=['inout_deepl'],
    package_dir={'': 'src'},
    install_requires=['matplotlib', 'math', 'opencv-python']
)

setup(**d)