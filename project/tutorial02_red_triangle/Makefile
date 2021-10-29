# Qt module 
QT_INC_DIR      := "/home/juzi/Qt5.9.4/5.9.4/gcc_64/include"
QT_LIB_DIR      := "/home/juzi/Qt5.9.4/5.9.4/gcc_64/lib"
QT_INC          := "-isystem" "$(QT_INC_DIR)/QtCore" "-isystem" "$(QT_INC_DIR)/QtGui"
QT_INC          += "-isystem" "$(QT_INC_DIR)/QtSvg" "-isystem" "$(QT_INC_DIR)/QtUiTools"
QT_INC          += "-isystem" "$(QT_INC_DIR)/QtXml" "-isystem" "$(QT_INC_DIR)"
QT_LIB          := "-lQtCore" "-lQtGui" "-lQtSvg" "-lQtUiTools" "-lQtXml" "-L$(QT_LIB_DIR)"
MOC_HEADERS     := $(shell find . -type f -name "*.h" \
			| xargs grep -lx "[[:space:]]*Q_OBJECT[[:space:]]*")
MOC_SOURCES     := $(MOC_HEADERS:.h=.moc.cpp)
GEN_MOC_SOURCES := $(foreach vh, $(MOC_HEADERS), $(shell \
			vd=`echo $(vh) | sed 's/.h/.moc.cpp/'`; \
			if test $(vh) -nt $$vd; then \
				moc $(vh) -o $$vd; \
			fi))

# GLFW module
GLFW_LIB := "-lglfw" "-lGLEW" "-lGLU"
GLFW_LIB += "-lGL" "-lXrandr" "-lX11"

# config CXX
CXX := g++ "-I." "-fPIC" "-lrt" "-lm" "-lpthread" 
#CXX += -L"/usr/local/lib"
#CXX += -L"/usr/lib64"
#CXX += $(QT_INC)$(QT_LIB)  # include Qt library
CXX += $(GLFW_LIB) # include GLFW library

SOURCES  := $(shell find . -type f | grep \.cpp$)
OBJECTS  := $(SOURCES:.cpp=.o) 
TAGET    := a.out

CLEANOBJ := $(TAGET) $(OBJECTS)
CLEANOBJ += $(MOC_SOURCES)

$(TAGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TAGET) 

$(OBJECTS): %.o:%.cpp
	$(CXX) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(CLEANOBJ)

