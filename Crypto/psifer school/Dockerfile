FROM ubuntu
MAINTAINER Steven
LABEL Description="CSAW 2014-Q: Psifer School" VERSION=1.0
# Update
RUN apt-get -y update && apt-get -y upgrade
RUN apt-get -y install socat python
# Files
RUN mkdir -p /root/psifer
WORKDIR /root/psifer
COPY psiferschool.py psifer.py
# Execute
CMD ["socat", "TCP4-LISTEN:12345,reuseaddr,fork", "EXEC:\"python -u ./psifer.py\""]
EXPOSE 12345
