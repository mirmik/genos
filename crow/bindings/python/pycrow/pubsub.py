import pycrow.libcrow
from pycrow.libcrow import *


PUBLISHERS = {}
SUBSCRIBERS = {}


def get_publisher(theme):
    if theme in PUBLISHERS:
        return PUBLISHERS[theme]
    crowker = pycrow.libcrow.crowker_address()
    PUBLISHERS[theme] = pycrow.libcrow.publisher(crowker, theme)
    return PUBLISHERS[theme]


def get_subscriber(theme, incoming):
    crowker = pycrow.libcrow.crowker_address()
    SUBSCRIBERS[theme] = pycrow.libcrow.subscriber(incoming)
    SUBSCRIBERS[theme].subscribe(crowker, theme)
    return SUBSCRIBERS[theme]


def publish(theme, bindata):
    pub = get_publisher(theme)
    pub.publish(bindata)


def subscribe(theme, incoming, keepalive=2000):
    sub = get_subscriber(theme, incoming)
    sub.install_keepalive(keepalive, False)
