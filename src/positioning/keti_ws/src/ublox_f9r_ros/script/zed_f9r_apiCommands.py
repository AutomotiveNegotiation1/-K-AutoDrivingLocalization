#!/usr/bin/env python3
""" For more info on the documentation go to https://www.decawave.com/sites/default/files/dwm1001-api-guide.pdf
"""

from zed_f9r_systemDefinitions import SYS_DEFS


__author__     = SYS_DEFS.AUTHOR
__version__    = SYS_DEFS.VERSION
__maintainer__ = SYS_DEFS.MAINTAINER
__email__      = SYS_DEFS.EMAIL
__status__     = SYS_DEFS.STATUS


class ZED_F9R_API_MESSAGES:
        CHECK_HEADER    = b'\xb5\x62' # ZED-F9R-01B header check
        UBX_ESF_RAW     = b'\x10\x03' # ZED-F9R-01B Class ID ESF-RAW
        UBX_NAV_PVT     = b'\x01\x07' # ZED-F9R-01B Class ID NAV-PVT