# Use an official image with CMake and a C++ compiler
FROM ubuntu:20.04

# Avoids interactive hang
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libsqlite3-dev \
    libspdlog-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/app

# Copy the CMakeLists and source files to the working directory
COPY src src
COPY cmake cmake

# Create build directory
RUN mkdir -p build

# Set working directory to the build folder
WORKDIR /usr/src/app/build

# Configure the project using CMake
RUN cmake ../src

# Build the project
RUN cmake --build .

# Set the command to run the built executable
CMD ["./personal_expense_logger"]
