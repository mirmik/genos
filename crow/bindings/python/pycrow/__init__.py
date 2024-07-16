try:
    import pycrow.libcrow
    from pycrow.libcrow import *
except Exception as ex:
    print("PyCrow: Import libs error. ex:", ex)
    exit(0)


def start_client(port=0):
    """
            @port - udp port. 0 for dynamic
    """
    pycrow.libcrow.create_udpgate(12, port)
    pycrow.libcrow.start_spin()


def stop_client():
    pycrow.libcrow.stop_spin()
