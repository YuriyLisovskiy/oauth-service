FROM xalwart/framework:0.0.0-alpine

WORKDIR /app

COPY CMakeLists.txt ./
COPY LICENSE ./
COPY README.md ./
COPY ./src ./src
COPY scripts/deploy/docker ./

RUN mkdir -p ./build && \
    cd ./build || exit 1 && \
    cmake -D CMAKE_C_COMPILER=clang \
          -D CMAKE_CXX_COMPILER=clang++ \
          -D CMAKE_BUILD_TYPE=Release \
          -D XW_USE_POSTGRESQL=yes \
          -D XW_USE_SQLITE3=yes \
          .. && \
    make application && \
    cd .. && \
    mkdir -p ./ && \
    cp ./build/application ./ && \
    cp ./src/*.yml ./ && \
    tree

RUN rm -rf ./CMakeLists.txt ./src ./build

CMD ./application start-server --bind=0.0.0.0:$PORT \
                               --timeout-seconds=5 \
                               --retries=1 \
                               --workers=5 \
                               --colors=false
