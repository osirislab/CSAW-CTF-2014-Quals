FROM ubuntu
MAINTAINER Steven
LABEL Description="CSAW 2014-Q: Feal" VERSION=1.0

# Update
RUN apt-get -y update && apt-get -y upgrade
RUN apt-get -y install socat python
# Files
RUN mkdir -p /root/feal
WORKDIR /root/feal
COPY feal.py .
# Execute
CMD ["python", "feal.py"]
EXPOSE 8888
