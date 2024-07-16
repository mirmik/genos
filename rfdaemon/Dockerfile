ARG ARCH
FROM igris:$ARCH
FROM nos:$ARCH
FROM crow:$ARCH
FROM netricks-20.04-env:$ARCH

COPY --from=0 /usr/local/include/igris /usr/local/include/igris
COPY --from=0 /usr/lib/libigris.so /usr/lib/libigris.so
COPY --from=0 /usr/lib/libigris.a /usr/lib/libigris.a

COPY --from=1 /usr/local/include/nos /usr/local/include/nos
COPY --from=1 /usr/lib/libnos.so /usr/lib/libnos.so
COPY --from=1 /usr/lib/libnos.a /usr/lib/libnos.a

COPY --from=2 /usr/local/include/crow /usr/local/include/crow
COPY --from=2 /usr/lib/libcrow.so /usr/lib/libcrow.so
COPY --from=2 /usr/lib/libcrow.a /usr/lib/libcrow.a

ADD . /root/rfdaemon
WORKDIR /root/rfdaemon
RUN ./make.py
RUN sudo ./make.py install

CMD rfdaemon --noconsole
