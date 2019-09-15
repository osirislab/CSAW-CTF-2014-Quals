FROM ubuntu:14.04
MAINTAINER Steven
LABEL Description="CSAW 2014-Q: S3" VERSION=1.0

RUN apt-get -y update && \
	apt-get -y upgrade && \
	apt-get -y install libc, libstdc++, xinetd

RUN mkdir -p /root/s3
WORKDIR /root/s3
COPY 

EXPOSE 5333
