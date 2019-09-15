FROM ubuntu:16.04
MAINTAINER Steven
LABEL Description="CSAW 2014-Q: Hashes" VERSION=1.0
# Update
RUN apt-get -y update
RUN apt-get -y install python-pip xvfb xserver-xephyr
RUN pip install flask selenium pyvirtualdisplay
# Files
RUN mkdir -p /root/hashes
WORKDIR /root/hashes
COPY app.py .
COPY config.py .
COPY templates/ .
# Execute
EXPOSE 7878
CMD ["python", "app.py"]