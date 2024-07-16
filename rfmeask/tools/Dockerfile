FROM igris-20.04
FROM nos-20.04
FROM crow-20.04
FROM ralgo-20.04
FROM rabbit-20.04
FROM netricks-20.04-env

COPY --from=0 /usr/local/include/igris /usr/local/include/igris
COPY --from=0 /usr/lib/libigris.so /usr/lib/libigris.so

COPY --from=1 /usr/local/include/nos /usr/local/include/nos
COPY --from=1 /usr/lib/libnos.so /usr/lib/libnos.so

COPY --from=2 /usr/local/include/crow /usr/local/include/crow
COPY --from=2 /usr/lib/libcrow.so /usr/lib/libcrow.so

COPY --from=3 /usr/local/include/ralgo /usr/local/include/ralgo
COPY --from=3 /usr/lib/libralgo.so /usr/lib/libralgo.so

COPY --from=4 /usr/local/include/rabbit /usr/local/include/rabbit
COPY --from=4 /usr/lib/librabbit.so /usr/lib/librabbit.so

ADD . /root/rfmeask

WORKDIR /root/rfmeask
RUN ./make.py
RUN sudo ./make.py install

#WORKDIR /root/rfdaemon/tests
#RUN ./make.py
#RUN ./runtests