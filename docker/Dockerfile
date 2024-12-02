FROM nvidia/cuda:11.8.0-devel-ubuntu20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y build-essential unzip git cmake g++ libgtest-dev \
	                   wayland-protocols libwayland-dev \
					   libxkbcommon-dev libx11-dev libx11-xcb-dev libxcb1-dev \
					   libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
					   python3 python3-pip pkg-config && \
    apt-get clean

WORKDIR /app

COPY . /app

RUN cmake -S . -B build
RUN cmake --build build
#RUN nvidia-smi
#RUN build/SandTest
