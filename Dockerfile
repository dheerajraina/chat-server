# Use an official Ubuntu as base image
FROM ubuntu:latest

# Set environment variables to avoid interactive prompts during package installs
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary dependencies: build tools, libraries, and MongoDB/Redis C++ drivers
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libssl-dev \
    libboost-all-dev \
    g++ \
    libjsoncpp-dev \
    # libmongocxx-dev \
    # libbsoncxx-dev \
    # libhiredis-dev \
    curl \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install wget
# Install Crow framework from crow releases
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.1.2/Crow-1.2.1-Linux.deb
RUN dpkg -i Crow-1.2.1-Linux.deb
# Set up working directory
WORKDIR /app/build

# Copy the entire project directory into the container
COPY . /app

# Build the C++ app
RUN rm -rf CMakeCache.txt CMakeFiles && cmake .. && cmake --build . --config RELEASE

# Expose necessary ports (update the port number if required)
EXPOSE 8080

# Command to run the app (assuming `chat_server` is the executable)
CMD ["./chat_server"]


# sudo docker build -t chat_server .

# sudo docker cp . chat_server:/app/

# sudo docker run -it <IMAGE ID>
# sudo docker images

# sudo docker run -it -p 8080:8080 chat_server

# sudo docker run -it -p 8080:8080 7c1faa14cb10

# sudo docker run -it --entrypoint /bin/sh chat_server

# sudo docker rmi chat-server
# sudo docker container prune
# sudo docker image prune
# sudo docker image prune -a
# sudo docker volume prune
# sudo docker network prune
# sudo docker system prune
# sudo docker system prune -a --volumes


# docker run my_image ./build/chat_server -> image name with executable path
