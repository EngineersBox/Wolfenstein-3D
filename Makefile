CXX_VERSION := 11
CXX := -clang++ -std=c++$(CXX_VERSION)
CXXFLAGS := -framework OpenGL -framework GLUT -stdlib=libc++
OUT_DIR := out
LOGS_DIR := logs
TARGET := run
SRC := $(wildcard src/raytracer/*.cpp)
OBJECTS := $(SRC:%.cpp)

compile:
	@echo ">> Compiling new build"
	@echo "--------------------------------"
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT_DIR)/$(TARGET)
	@echo "--------------------------------\n"

clean:
	@echo ">> Removing previous builds"
	@echo "--------------------------------"
	-@rm -rvf $(OUT_DIR)/*
	@echo "--------------------------------\n"

clean_logs:
	@echo ">> Removing logs"
	@echo "--------------------------------"
ifeq (,$(wildcard $(LOGS_DIR)/*))
	@echo "No files to remove"
else
	-@rm -rvf $(LOGS_DIR)/*
endif
	@echo "--------------------------------\n"

destroy:
	@echo ">> Removing $(OUT_DIR) directory"
	@echo "--------------------------------"
	-@rm -rvf $(OUT_DIR)
	@echo "--------------------------------\n"

run:
	@echo ">> Running new build..."
	@echo "--------------------------------\n"
	@ $(OUT_DIR)/$(TARGET)
	@echo ">> Stopped running"
	@echo "--------------------------------\n"

build:
	@make clean
	@make compile
	@make run