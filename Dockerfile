FROM ubuntu:focal

WORKDIR /app

COPY test.cpp test.cpp
COPY bdd.hpp bdd.hpp
COPY jhvolka_dnfgen.hpp jhvolka_dnfgen.hpp
COPY loop.sh loop.sh

RUN chmod 777 ./loop.sh
RUN apt-get update
RUN apt-get -y install g++-10
RUN g++-10 -std=gnu++20 -m64 -O2 -o test test.cpp

CMD [ "./loop.sh" ]