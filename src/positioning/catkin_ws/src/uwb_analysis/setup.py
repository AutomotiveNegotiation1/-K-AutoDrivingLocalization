from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

d = generate_distutils_setup(
    packages=['uwb_analysis'],
    package_dir={'': 'src'},
    install_requires=['matplotlib', 'math']
)

setup(**d)