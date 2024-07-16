ARG ARCH
FROM netricks-20.04-env:$ARCH

ADD . /root/nos

WORKDIR /root/nos
RUN /root/sanitize-check.sh
RUN ./make.py
RUN ./runtests
RUN sudo ./make.py install
