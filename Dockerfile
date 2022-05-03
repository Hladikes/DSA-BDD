FROM ubuntu:focal

WORKDIR /app

COPY bin/ bin/
COPY src/ src/
COPY loop.sh loop.sh
COPY compile.sh compile.sh

ENV MIN 13
ENV MAX 26
ENV FNCOUNT 5

RUN chmod 777 ./loop.sh
RUN chmod 777 ./compile.sh

RUN apt-get -qq update
RUN apt-get -qq -y install g++-10
RUN apt-get -qq -y install zip

RUN ./compile.sh

CMD ./loop.sh $MIN $MAX $FNCOUNT