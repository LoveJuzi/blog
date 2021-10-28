TAGET := a.out

# include paths
#QT_INC   :="-isystem" "$(QT_INC_DIR)/QtCore" "-isystem" "$(QT_INC_DIR)/QtGui" "-isystem" "$(QT_INC_DIR)/QtSvg" "-isystem" "$(QT_INC_DIR)/QtUiTools" "-isystem" "$(QT_INC_DIR)/QtXml" "-isystem" "$(QT_INC_DIR)"

# libs
#QT_LIB   := "-lQtCore" "-lQtGui" "-lQtSvg" "-lQtUiTools" "-lQtXml" "-L$(QT_LIB_DIR)"
LIBFLAG  := "-lrt" "-lm" "-lpthread" $(QT_LIB)

# config CXX
CXX             := g++ "-I." $(QT_INC) $(LIBFLAG) "-fPIC"

MOC_HEADERS     := $(shell find . -type f -name "*.h" | xargs grep -lx "[[:space:]]*Q_OBJECT[[:space:]]*")
MOC_SOURCES     := $(MOC_HEADERS:.h=.moc.cpp)
GEN_MOC_SOURCES := $(foreach vh, $(MOC_HEADERS), $(shell \
						vd=`echo $(vh) | sed 's/.h/.moc.cpp/'`; \
						if test $(vh) -nt $$vd; then \
							moc $(vh) -o $$vd; \
						fi))
SOURCES         := $(shell find . -type f | grep \.cpp$)
OBJECTS         := $(SOURCES:.cpp=.o) 


$(TAGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TAGET) 

$(OBJECTS): %.o:%.cpp
	$(CXX) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(TAGET) $(OBJECTS) $(MOC_SOURCES)
