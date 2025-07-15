FROM ubuntu:24.04

RUN apt-get -y update && apt-get install -y
RUN apt-get -y install                  \      
                    build-essential     \
                    clang clangd        \                
                    cmake               \                 
                    libboost-all-dev    \    
                    && apt-get clean

RUN mkdir Project 
WORKDIR /Project 

COPY . .