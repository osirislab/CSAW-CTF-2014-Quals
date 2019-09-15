FROM ubuntu:16.04
MAINTAINER Steven
LABEL Description="CSAW 2014-Q: Saturn/x86" VERSION='1.0'
# Update
RUN apt-get -y update && apt-get -y upgrade
RUN apt-get -y install libc6:i386 \
		libncurses5:i386 libstdc++6:i386 nmap
# Files
COPY run_from_here /root/saturn
WORKDIR /root/saturn
RUN dpkg --add-architecture i386
# Execute
CMD ["./run.sh"]
EXPOSE 8888
