OPTIMIZE = -O0 -g3
## OPTIMIZE = -O3

OBJS += \
./obj/client.o \
./obj/http_client.o \
./obj/main.o \
./obj/main_event.o \
./obj/socket_util.o \
./obj/worker.o 

OBJS += ./obj/timers.o
OBJS += ./obj/des.o 
OBJS += ./obj/deskey.o 
OBJS += ./obj/XORcode.o 
OBJS += ./obj/sdata.o 
OBJS += ./obj/debug.o
OBJS += ./obj/elfhash.o
OBJS += ./obj/hashlist.o


LIBS += -lpthread

# All Target
all: Client


# Tool invocations
Client: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -o "Client" $(OBJS) $(USER_OBJS)  $(LIBS) -ldl
	@echo 'Finished building target: $@'
	@echo ' '

./obj/%.o: ./src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(OPTIMIZE) -Wall -c -fmessage-length=0 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
	
./obj/%.o: ./src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(OPTIMIZE) -Wall -c -fmessage-length=0 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Client
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

