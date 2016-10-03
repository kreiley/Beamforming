CC = gcc

TARGET = Beamforming

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c delay.c delaytest.c -lm

clean: 
	$(RM) $(TARGET)
