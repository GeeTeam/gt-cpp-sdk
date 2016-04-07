
CC = g++
CFLAGS = -g -Wall -O0
#json flags
# CFLAGS += -DJSON_USE_SIMPLE_INTERNAL_ALLOCATOR


#libcgicc libcurl
INCLUDES = -I./ -I/usr/include -I/usr/local/include 

LIBS =  -L/usr/lib -L/usr/local/lib -lcgicc -lcurl


TARGET = login_page.cgi StartCaptchaCgi.cgi VerifyLoginCgi.cgi
# TARGET = login_page.cgi

GEE_SDK_OBJS =  sdk/md5.o \
				sdk/httpclient.o \
				sdk/geetestlib.o 

COMMON_OBJS = 	json/json_value.o \
				json/json_reader.o \
				json/json_writer.o 

all: $(TARGET)
	cp $(TARGET) ../dst/

login_page.cgi:login_page.o $(GEE_SDK_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS)  -o $@ $^ $(LIBS)

StartCaptchaCgi.cgi:StartCaptchaCgi.o $(GEE_SDK_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS)  -o $@ $^ $(LIBS)

VerifyLoginCgi.cgi:VerifyLoginCgi.o $(GEE_SDK_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS)  -o $@ $^ $(LIBS)



%.o:%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $< 


install:
	sudo cp -r ../dst /var/www/cgi-bin/

clean:
	rm -f *.o
	rm -f *.cgi 
	rm -f $(GEE_SDK_OBJS) 
	rm -f $(COMMON_OBJS) 
	rm -f ../dst/*.cgi
.PHONY:clean all