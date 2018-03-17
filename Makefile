#
# makefile for the avalon compiler
#
# :author   Ntwali Bashige
# :year     2017
# :email    ntwali.bashige@gmail.com
#

cc          := g++
cflags      := -std=c++11 -g -Wall -pedantic
ldflags		:= -lgmp -lboost_filesystem -lboost_system
ldpaths		:= -L/usr/local/lib
rdpaths 	:= -Wl,-rpath=/usr/local/lib
src_dir     := src
inc         := -Isrc -Iinclude
build_dir   := build
bin_dir     := bin
target      := $(bin_dir)/avalon

src_ext     := cpp
sources     := $(shell find $(src_dir) -type f -name *.$(src_ext))
objects     := $(patsubst $(src_dir)/%,$(build_dir)/%,$(sources:.$(src_ext)=.o))


.PHONY: all
all: setup $(target)

$(target): $(objects)
	@echo " Linking..."
	@$(cc) $^ -o $(target) $(ldpaths) $(ldflags) $(rdpaths)

$(build_dir)/%.o: $(src_dir)/%.$(src_ext)
	@mkdir -p $(dir $@)
	@$(cc) $(cflags) $(inc) -c -o $@ $<

.PHONY: setup
setup:
	@echo " Setting up..."
	@mkdir -p $(build_dir)
	@mkdir -p $(bin_dir)

.PHONY: clean
clean:
	@echo " Cleaning..."
	@find . -exec touch {} \;
	@rm -rf $(build_dir) $(bin_dir)
