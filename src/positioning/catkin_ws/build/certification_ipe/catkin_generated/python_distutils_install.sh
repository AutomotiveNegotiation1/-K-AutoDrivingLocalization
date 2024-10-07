#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/keti/catkin_ws/src/certification_ipe"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/keti/catkin_ws/install/lib/python3/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/keti/catkin_ws/install/lib/python3/dist-packages:/home/keti/catkin_ws/build/certification_ipe/lib/python3/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/keti/catkin_ws/build/certification_ipe" \
    "/usr/bin/python3" \
    "/home/keti/catkin_ws/src/certification_ipe/setup.py" \
     \
    build --build-base "/home/keti/catkin_ws/build/certification_ipe" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/keti/catkin_ws/install" --install-scripts="/home/keti/catkin_ws/install/bin"
