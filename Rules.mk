
CC=i386-linux-aros-gcc

% : %.c ;  $(CC) $(CFLAGS) $(CPPFLAGS)  -o $@ $< $(LDFLAGS) 

