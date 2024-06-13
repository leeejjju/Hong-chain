# 베이스 이미지 설정
FROM ubuntu:20.04

# 환경 변수 설정
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Seoul

# 필수 패키지 설치
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    clang++ \
    cmake \
    git \
    libssl-dev \
    libcurl4-openssl-dev \
    libjsoncpp-dev \
    tzdata \
    && rm -rf /var/lib/apt/lists/*

# 시간대 설정
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 작업 디렉토리 설정
WORKDIR /app

# 소스 코드 및 필요 파일 복사
COPY . .

RUN cd AFLplusplus && make

# 서버 빌드
RUN make daemon.out

# 컨테이너 시작 시 실행할 명령어
CMD ["./daemon.out", "9090"]
