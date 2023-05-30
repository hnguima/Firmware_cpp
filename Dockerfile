FROM espressif/idf

RUN apt-get update && apt-get install -y \
    software-properties-common \
    npm
RUN npm install npm@latest -g && \
    npm install n -g && \
    npm install -g protobufjs protobufjs-cli\
    n latest \
    pip install protobuf grpcio-tools 