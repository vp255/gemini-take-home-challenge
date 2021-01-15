FROM debian:latest AS env
RUN echo 'deb http://deb.debian.org/debian testing main' >> /etc/apt/sources.list
RUN apt-get -y update \
  && apt-get -y install build-essential \
  && apt-get clean
COPY src/ /app
RUN cd /app && make all
