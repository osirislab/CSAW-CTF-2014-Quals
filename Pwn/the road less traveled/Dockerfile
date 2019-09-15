FROM ubuntu:16.04
MAINTAINER Steven
LABEL Description="CSAW 2014-Q: The Road Less Traveled" VERSION=1.0

RUN apt-get -y update && \
	apt-get -y upgrade

RUN mkdir -p /root/road
WORKDIR /root/road .
COPY bo/bo .

CMD ["./bo"]
EXPOSE 1515
