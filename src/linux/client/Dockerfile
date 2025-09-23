#FROM gcc:4.9
FROM ubuntu:questing-20250830
COPY . /usr/src/client
WORKDIR /usr/src/client
#RUN apt-get install sudo apt install build-essential
#RUN apt-get update
RUN gcc -o client main.c
EXPOSE 6667
CMD ["./client"]

