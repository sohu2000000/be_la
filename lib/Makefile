
LIBEVENT_TAR_NAME="libevent-2.0.22-stable.tar.gz"
LIBEVENT_URL="https://github.com/libevent/libevent/releases/download/release-2.0.22-stable/$(LIBEVENT_TAR_NAME)"

LIBEVENT_DIR_NAME="`echo $(LIBEVENT_TAR_NAME) | sed 's/^\(.*\).tar.gz$$/\1/g'`"

all:download-libevent2 compile-libevent2
	@echo $@ build finish

download-libevent2:
	if [ ! -e $(LIBEVENT_TAR_NAME) ]; then wget -c $(LIBEVENT_URL) ;fi;
	if [ ! -d $(LIBEVENT_DIR_NAME) ]; then tar -zxvf $(LIBEVENT_TAR_NAME) ; fi;

compile-libevent2:
	if [ ! -e ./libevent.so ]; then (cd $(LIBEVENT_DIR_NAME) ; ./configure ; make ; cp .libs/libevent.so .. ; ) ; fi;

clean:
	#rm -rf $(LIBEVENT_DIR_NAME)
	#rm -rf libevent.so
