include basic_rules.mk
include ca_lib.mk

SRC_ROOT := ./src
DIR_NATIVE := $(SRC_ROOT)/native
DIR_3RD_PARTY := $(SRC_ROOT)/3rdparty
DIR_LIB_INSTALL = ./lib

CA_CHANGE_LOG_ITEM := $(shell find . -name changelog.h | xargs grep "[0-9][0-9]/[0-9][0-9], [0-9]." | head -1)
CA_MOD_DATE := $(shell echo "$(CA_CHANGE_LOG_ITEM)" | awk -F ", " '{ print $$2 }' | sed 's/\///g')
CA_MAIN_VER := $(shell echo "$(CA_CHANGE_LOG_ITEM)" | awk -F ", " '{ print $$3 }' | awk -F ":" '{ print $$1 }')
CA_SVN_VER := $(shell svn info | grep -e 'Last Changed Rev' -e '最后修改的版本' | sed 's/.* \([0-9]\)/\1/')
VERSION_DEFINES := CA_MOD_DATE=$(CA_MOD_DATE) CA_MAIN_VER=$(CA_MAIN_VER) CA_SVN_VER=$(CA_SVN_VER)

# If TinyXML is already installed in your system,
# then you can specify this variable to remove it from libraries of CppAssistant.
# NO_TINYXML=1
ifneq ($(NO_TINYXML),)
	TINYXML_FLAG = "NO_TINYXML=1"
endif

NATIVE_SRCS += $(shell find $(DIR_NATIVE) -name "*.cpp" | grep -v "\.svn")
NATIVE_OBJS += $(addsuffix .o, $(basename $(NATIVE_SRCS)))

3RD_PARTY_SRCS = $(shell find $(DIR_3RD_PARTY) -name "*.c*" | grep -v "\.svn")
3RD_PARTY_OBJS += $(addsuffix .o, $(basename $(3RD_PARTY_SRCS)))

LIB_CPP_ASSISTANT.A := libcpp_assistant.a
LIB_CPP_ASSISTANT.SO := libcpp_assistant.so

LIBS.A = $(LIB_CPP_ASSISTANT.A)
LIBS.SO = $(LIB_CPP_ASSISTANT.SO)

TARGETS = $(LIBS.A) $(LIBS.SO)

all: validate_and_prepare $(TARGETS)

validate_and_prepare:
	chmod 777 ./validate_and_prepare.sh
	./validate_and_prepare.sh

.PHONY: obj_only

# DO NOT create an archive with another one, it looks ok
# during $(AR) operation, but will cause ld errors when other
# programs use the archive!!!
#$(LIB_CPP_ASSISTANT.A): ./native/libca_native.a ./3rdparty/libca_3rdparty.a
$(LIB_CPP_ASSISTANT.A):
	make static $(DEBUG_FLAG) $(STL_FLAG) $(TINYXML_FLAG) -C $(DIR_3RD_PARTY)
	make static $(DEBUG_FLAG) $(STL_FLAG) $(VERSION_DEFINES) -C $(DIR_NATIVE)
	$(AR) $(AR_FLAGS) $@ $(NATIVE_OBJS) $(3RD_PARTY_OBJS)
	$(RANLIB) $@

static: $(LIB_CPP_ASSISTANT.A)

$(LIB_CPP_ASSISTANT.SO):
	make shared $(DEBUG_FLAG) $(STL_FLAG) $(TINYXML_FLAG) -C $(DIR_3RD_PARTY)
	make shared $(DEBUG_FLAG) $(STL_FLAG) $(VERSION_DEFINES) -C $(DIR_NATIVE)
	$(CXX) -shared -o $@.$(CA_MAIN_VER) $(NATIVE_OBJS) $(3RD_PARTY_OBJS)
	ln -s $@.$(CA_MAIN_VER) $@

shared: $(LIB_CPP_ASSISTANT.SO)

debug:
	echo "Newest change log item: $(CA_CHANGE_LOG_ITEM)"
	make all DEBUG=1

pure:
	make all CA_USE_STL=0

pure_debug:
	make all DEBUG=1 CA_USE_STL=0

obj_only: validate_and_prepare
	make obj_only $(DEBUG_FLAG) $(STL_FLAG) $(TINYXML_FLAG) -C $(DIR_3RD_PARTY)
	make obj_only $(DEBUG_FLAG) $(STL_FLAG) -C $(DIR_NATIVE)

check: validate_and_prepare
	make check $(DEBUG_FLAG) $(STL_FLAG) $(TINYXML_FLAG) -C $(DIR_3RD_PARTY)
	make check $(DEBUG_FLAG) $(STL_FLAG) -C $(DIR_NATIVE)

install:
	mkdir -p $(DIR_LIB_INSTALL)
	mkdir -p $(HOME)/lib
	make install -C $(DIR_3RD_PARTY)
	make install -C $(DIR_NATIVE)
	mkdir -p $(HOME)/include/cpp_assistant
	rm -rf $(HOME)/include/cpp_assistant/*
	cp -a -r include/* $(HOME)/include/cpp_assistant/
	#for i in `find $(DIR_3RD_PARTY)/ -type d`; \
	#do \
	#	mkdir -p $(HOME)/include/cpp_assistant/$$i; \
	#done
	#for i in `find $(DIR_3RD_PARTY)/ -type f -a -name "*.h"`; \
	#do \
	#	cp $$i $(HOME)/include/cpp_assistant/$$i; \
	#done
	for i in $(TARGETS); \
	do \
		cp -a $$i* $(DIR_LIB_INSTALL)/ ;\
		cp -a $$i* $(HOME)/lib/ ; \
	done

uninstall:
	make uninstall -C $(DIR_3RD_PARTY)
	make uninstall -C $(DIR_NATIVE)
	cd $(DIR_LIB_INSTALL)/ && rm -f $(addsuffix *, $(basename $(TARGETS))) && cd -
	@echo "Contents in directory $(HOME)/lib and $(HOME)/include/cpp_assistant have to be cleared manually if necessary."

clean:
	rm -f $(addsuffix *, $(basename $(TARGETS)))
	cd $(DIR_LIB_INSTALL)/ && rm -f $(addsuffix *, $(basename $(TARGETS))) && cd -
	find . -iname "*.plist" | xargs rm -f
	make clean -C $(DIR_3RD_PARTY)
	make clean -C $(DIR_NATIVE)

add_copyright:
	# add_code_copyright.sh is a self-made script that adds copyright info into all code files.
	-for i in `find ./src/ -path ./src/3rdparty -prune -o -type f -iname "*.[ch]*" -print | grep -v "\.svn"`; \
	do \
		add_code_copyright.sh $$i; \
	done
	-for i in `find ./include/ -path ./include/3rdparty -prune -o -type f -iname "*.h" -print | grep -v "\.svn"`; \
	do \
		add_code_copyright.sh $$i; \
	done

pack:
	make clean
	make clean -C src/unittest
	make add_copyright
	# tts.sh is a self-made script that turns all tabs in files into spaces.
	-tts.sh ../libcpp_assistant/
	tar -zcvf ~/libcpp_assistant_v$(CA_MAIN_VER).tar.gz ../libcpp_assistant/
	-nautilus ~/libcpp_assistant_v$(CA_MAIN_VER).tar.gz