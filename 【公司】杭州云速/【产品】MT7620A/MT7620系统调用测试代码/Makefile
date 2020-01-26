CC=mipsel-linux-gcc

LDFLAGS = -I. -I/opt/buildroot-gcc342/include
LIBS = -L/opt/buildroot-gcc342/lib 
LIBEX =  -lpthread -lm -lrt

# Keep alphabetic order
SRCS =  main.c

OBJS = $(SRCS:.c=.o)

all: build-all
	@echo $(CFLAGS)
	@echo $(LDFLAGS)
	@echo $(LIBEX)

build-all: serialport timer sem_timedwait pthread_rwlock

serialport: $(OBJS) 
	@$(CC) -Wall $(LDFLAGS) -o $@ $(OBJS) $(LIBS) $(LIBEX)

timer:
	@$(CC) -Wall $(LDFLAGS) -o $@ timer.c $(LIBS) -lrt -lpthread

sem_timedwait:
	@$(CC) -Wall $(LDFLAGS) -o $@ sem_timedwait.c $(LIBS) -lpthread

pthread_rwlock:
	@$(CC) -Wall -D_GNU_SOURCE $(LDFLAGS) -o $@ pthread_rwlock.c $(LIBS) -lpthread

clean:
	rm -f *.o .depend serialport timer sem_timedwait pthread_rwlock
