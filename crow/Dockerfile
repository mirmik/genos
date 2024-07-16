ARG ARCH
FROM igris:$ARCH
FROM nos:$ARCH
FROM netricks-20.04-env:$ARCH

COPY --from=0 /usr/local/include/igris /usr/local/include/igris
COPY --from=0 /usr/lib/libigris.so /usr/lib/libigris.so
COPY --from=0 /usr/lib/libigris.a /usr/lib/libigris.a
COPY --from=1 /usr/local/include/nos /usr/local/include/nos
COPY --from=1 /usr/lib/libnos.so /usr/lib/libnos.so
COPY --from=1 /usr/lib/libnos.a /usr/lib/libnos.a

ADD . /root/crow

WORKDIR /root/crow
RUN /root/sanitize-check.sh
RUN ./make.py
RUN ./runtests
RUN sudo ./make.py install
