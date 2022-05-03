FROM ubuntu:focal

WORKDIR /app

COPY bin/ bin/
COPY src/ src/
COPY loop.sh loop.sh

ENV MIN 13
ENV MAX 26
ENV FNCOUNT 5

RUN chmod 777 ./loop.sh
RUN apt-get -qq update
RUN apt-get -qq -y install g++-10
RUN apt-get -qq -y install zip
RUN g++-10 -std=gnu++20 -m64 -O2 -o ./bin/test ./src/test.cpp

CMD ./loop.sh $MIN $MAX $FNCOUNT