
INCDIRS  += -I .


$(info  )
$(info compiling pcie-tools)


PCIETOOLS_OBJS := \
	pci-config-reader.o


LDFLAGS := -g
CFLAGS  := -Werror ${INCDIRS} -g
ASFLAGS := ${INCDIRS} -g
TARGET 	:= pcie-analyzer


all: $(TARGET)


$(TARGET): $(PCIETOOLS_OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

clean:
	$(RM) $(PCIETOOLS_OBJS) $(TARGET)