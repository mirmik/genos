FROM igris-20.04
FROM nos-20.04
FROM netricks-20.04-env

COPY --from=0 /usr/local/include/igris /usr/local/include/igris
COPY --from=0 /usr/lib/libigris.so /usr/lib/libigris.so
COPY --from=1 /usr/local/include/nos /usr/local/include/nos
COPY --from=1 /usr/lib/libnos.so /usr/lib/libnos.so

ADD . /root/rfdaemon

WORKDIR /root/rfdaemon
RUN ./make.py
RUN sudo ./make.py install

#WORKDIR /root/rfdaemon/tests
#RUN ./make.py
#RUN ./runtests