
INCDIRS  += -I .

PCIETOOLS_OBJS := \
	pci-commands.o \
	pci-print.o \
	sys-utils.o \
	pci-tool-main.o


LDFLAGS := -g
CFLAGS  := -Werror ${INCDIRS} -g
ASFLAGS := ${INCDIRS} -g
TARGET 	:= pcie-analyzer.exe

$(info compiling...)

all: $(TARGET)


$(TARGET): $(PCIETOOLS_OBJS)
	$(info  )
	$(info linking...)
	$(CC) ${LDFLAGS} -o $@ $^

clean:
	$(RM) $(PCIETOOLS_OBJS) $(TARGET)