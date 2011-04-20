
CC=i386-linux-aros-gcc

%.o : %.c ;  $(CC) -c $(CFLAGS) $(CPPFLAGS)  -o $@ $^

% : %.o ;  $(CC) $(CFLAGS) $(CPPFLAGS)  -o $@ $^ $(LDFLAGS) 

