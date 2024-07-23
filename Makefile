COMPILER = gcc
CFLAGS = -Wall -Wextra -Werror -g
# CFLAGS = -Wall -std=c11
INSTALL_DIR=3dViewer
RES_DIR=resource/
DVI_DIR=documentation

# C_LIBS
ifeq ($(OS),Windows_NT) # MinGW
	OPEN = start
	C_LIBS = -lcheck -lm -lpthread -ls21_3d -L.
    QMAKE = C:/Qt/6.3.1/mingw_64/bin/qmake.exe
    OPT = -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"
    BUILD_DIR = build-3DViewer
    DIR = /debug
    RES_NAME = 3dviewer_win.tex
    TEXI2DVI = latex
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux) # Linux
		OPEN = xdg-open
		C_LIBS = -lcheck -lsubunit -lpthread -lrt -lm -ls21_3d -L. -D_GNU_SOURCE
        QMAKE=qmake 
        OPT=-spec linux-g++ CONFIG+=qtquickbuilder
        BUILD_DIR = build-3DViewer
        DIR = 
				RES_NAME = 3dviewer_linux.texi
        TEXI2DVI = texi2dvi -p --dvipdf
	endif
	ifeq ($(UNAME_S),Darwin) # Apple
 		OPEN = open
 		C_LIBS = -lcheck -ls21_3d -L.
        QMAKE=qmake
        OPT= CONFIG+=qtquickbuilder
        BUILD_DIR=3dViewer-Desktop_x86_64bit-Release/
        DIR = 3dViewer.app/Contents/MacOS
				RES_NAME = 3dviewer_linux.tex
	endif
endif

all rebuild: clean build_3d

build_3d:
	$(QMAKE) -o $(BUILD_DIR)/Makefile 3DViewer.pro $(OPT)
	make -C $(BUILD_DIR)/ first
      
install: build_3d
	@mkdir -p $(INSTALL_DIR)
	@cp -r $(BUILD_DIR)$(DIR) $(INSTALL_DIR)
	@cp -r libs/* $(INSTALL_DIR)$(DIR)
	@cp $(RES_DIR)/$(RES_NAME) $(INSTALL_DIR)/$(RES_NAME)
	@rm -rf $(INSTALL_DIR)$(DIR)/*.o  $(INSTALL_DIR)$(DIR)/*.h $(INSTALL_DIR)$(DIR)/*.cpp $(INSTALL_DIR)$(DIR)/*.Debug

uninstall:
	@rm -rf $(INSTALL_DIR) $(DVI_DIR)

dist: clean
	tar -czf "3dViewer.tgz" Makefile *.cpp *.c *.h *.pro *.ui libs/* resource/*
    
dvi:
	$(TEXI2DVI) $(RES_DIR)$(RES_NAME)
	mkdir -p $(DVI_DIR)
	mv 3dviewer*.* $(DVI_DIR)

clean:   
	rm -rf s21_*.o *.a *.exe s21_tests *.gcno *.gcda *.gcov s21_3d_coverage.info coverage_report *.txt *.gch  
	rm -rf $(INSTALL_DIR) $(BUILD_DIR)
	rm -rf *.tgz $(DVI_DIR)

s21_3d.o: clean parcer.c
	$(COMPILER) $(CFLAGS) -c parcer.c -o s21_3d.o

s21_3d.a: s21_3d.o
	ar rc libs21_3d.a s21_3d.o
	ranlib libs21_3d.a
	cp libs21_3d.a s21_3d.a

s21_3d_test.o: s21_3dV_test.c
	$(COMPILER) $(CFLAGS) -c s21_3dV_test.c -o s21_3d_test.o

mall: s21_3d.a s21_3d_test.o

# tests
tests: clean mall
	$(COMPILER) $(CFLAGS) -c s21_3dV_test.c
	$(COMPILER) $(CFLAGS) s21_3d.a s21_3d_test.o $(C_LIBS) -o s21_tests
	./s21_tests

cppcheck:
	cppcheck --suppress=missingIncludeSystem *.c parcer.h
    
clang:
	clang-format -n *.c *.h

leakscheck_linux: clean
	$(COMPILER) $(CFLAGS) -g -O0 -c parcer.c -o s21_3d.o
	$(COMPILER) $(CFLAGS) -g -O0 -c s21_3dV_test.c
	ar rc libs21_3d.a s21_3d.o
	ranlib libs21_3d.a
	cp libs21_3d.a s21_3d.a
	$(COMPILER) $(CFLAGS) -g -O0 s21_3d.a s21_3dV_test.o $(C_LIBS) -o s21_tests
	valgrind --trace-children=yes --track-fds=yes --leak-check=yes --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_log.txt ./s21_tests 9>&1 
	tail -n 8 valgrind_log.txt
    
leakscheck_mac: clean
	$(COMPILER) $(CFLAGS) -g -O0 -c parcer.c -o s21_3d.o
	$(COMPILER) $(CFLAGS) -g -O0 -c s21_3dV_test.c
	ar rc libs21_3d.a s21_3d.o
	ranlib libs21_3d.a
	cp libs21_3d.a s21_3d.a
	$(COMPILER) $(CFLAGS) -g -O0 s21_3d.a s21_3dV_test.o $(C_LIBS) -o s21_tests
	CK_FORK=no leaks --atExit -- ./s21_tests

gcov_report: clean # does not use gcov, but generates an html report indeed
	$(COMPILER) $(CFLAGS) --coverage -c parcer.c -o s21_3d.o
	$(COMPILER) $(CFLAGS) -c s21_3dV_test.c
	ar rc libs21_3d.a s21_3d.o
	ranlib libs21_3d.a
	cp libs21_3d.a s21_3d.a
	$(COMPILER) $(CFLAGS) --coverage s21_3d.a s21_3dV_test.o $(C_LIBS) -o s21_tests
	./s21_tests
	lcov -c --directory . --output-file s21_3d_coverage.info
	genhtml s21_3d_coverage.info --output-directory coverage_report

