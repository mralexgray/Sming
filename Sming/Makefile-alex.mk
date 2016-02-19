

# SHELL 		 	:= $(shell which zsh)
COM_PORT = $(lastword $(wildcard /dev/tty.*))

# BIN_EXEC 	 	?= /opt/xtensa-lx106-elf/bin
# export PATH := $(BIN_EXEC)subst:$(PATH)

WIFI_SSID  = Dickens
WIFI_PWD   = dickens1931
