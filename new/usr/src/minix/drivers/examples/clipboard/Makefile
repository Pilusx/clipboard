# Makefile for the clipboard driver.
PROG=	clipboard
SRCS=	bw386389.c stack.c stack.h bw386389.h clipboard_protocol.h

DPADD+=	${LIBCHARDRIVER} ${LIBSYS}
LDADD+=	-lchardriver -lsys

.include <minix.service.mk>
