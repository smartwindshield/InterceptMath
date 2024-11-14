# Compiler and flags
CC = gcc
CFLAGS = -c

# Targets
TARGET = a.exe
OBJECTS = SolTrack.o TestSolTrackSingle.o SolTrack_RiseSet.o

# Rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Dependencies and compilation rules
TestSolTrackSingle.o: TestSolTrackSingle.c SolTrack.h
	$(CC) $(CFLAGS) TestSolTrackSingle.c -o TestSolTrackSingle.o

SolTrack.o: SolTrack.c SolTrack.h
	$(CC) $(CFLAGS) SolTrack.c -o SolTrack.o

SolTrack_RiseSet.o: SolTrack_RiseSet.c SolTrack.h
	$(CC) $(CFLAGS) SolTrack_RiseSet.c -o SolTrack_RiseSet.o

# Clean up build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the executable
run: all
	./$(TARGET)
