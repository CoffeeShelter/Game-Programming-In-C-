CC = g++

# 컴파일러 옵션
CXXFLAGS = -Wall -O2
# 링커 옵션
LDFLAGS = -lmingw32 -lSDL2main -lSDL2
#라이브러리 경로
LIBRARY = -Llib
# 헤더파일 경로
INCLUDE = -Iinclude
# 소스 파일 디렉터리
SRC_DIR = ./src
# 오브젝트 파일 디렉터리
OBJ_DIR = ./obj
# 생성하고자 하는 실행 파일 이름
TARGET = main

# Make 할 소스 파일들
# wildcard 로 SRC_DIR 에서 *.cc 로 된 파일들 목록을 뽑아낸 뒤에
# notdir 로 파일 이름만 뽑아낸다.
# (e.g SRCS 는 foo.cc bar.cc main.cc 가 된다.)
SRCS = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

# OBJS 안의 object 파일들 이름 앞에 $(OBJ_DIR)/ 을 붙인다.
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: main

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(OBJECTS)
	$(CC) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBRARY) $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)


# all:
# 	g++ -I src/Include -Lsrc/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2