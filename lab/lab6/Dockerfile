FROM ubuntu:20.04

RUN apt-get update && apt-get install -y locales && rm -rf /var/lib/apt/lists/* \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

ENV DEBIAN_FRONTEND noninteractive
ENV LANG en_US.utf8

RUN apt update && apt install make -y && apt install build-essential -y && apt install manpages-dev -y \
    && apt install mpich -yq

WORKDIR "/workspace"
