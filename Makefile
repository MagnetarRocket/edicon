# $Id: Makefile,v 1.1 2022/02/07 19:41:40 alx Exp $

CFLAGS = -O2

OBJS = dtIconShell.o event.o fileIO.o fileIODialog.o globals.o graphics.o \
	image.o main.o newIconDialog.o process.o queryDialog.o stdErrDialog.o \
	utils.o DialogBox.o NewCvt.o

dticon: $(OBJS)
	cc -o $@ $(OBJS) -lXm -lXt -lXpm -lX11 -lm

clean:
	rm -f $(OBJS) dticon

install:
	install -m775 dticon /usr/bin/dticon
	install -m664 dticon.1 /usr/share/man/man1/dticon.1

DialogBox.o: DialogBox.c DialogBoxP.h DialogBox.h
dtIconShell.o: dtIconShell.c externals.h constants.h main.h utils.h process.h
event.o: event.c externals.h constants.h main.h utils.h process.h \
 fileIO.h graphics.h image.h
fileIO.o: fileIO.c externals.h constants.h main.h utils.h process.h
fileIODialog.o: fileIODialog.c main.h externals.h constants.h
globals.o: globals.c constants.h
graphics.o: graphics.c externals.h constants.h utils.h process.h
help.o: help.c  externals.h constants.h main.h utils.h
image.o: image.c externals.h constants.h utils.h process.h image.h
main.o: main.c externals.h constants.h main.h  event.h utils.h process.h
NewCvt.o: NewCvt.c DialogBox.h
newIconDialog.o: newIconDialog.c DialogBox.h main.h externals.h \
 constants.h process.h
process.o: process.c externals.h constants.h main.h utils.h process.h \
 graphics.h image.h fileIO.h
queryDialog.o: queryDialog.c main.h externals.h constants.h process.h
stdErrDialog.o: stdErrDialog.c main.h externals.h constants.h process.h
utils.o: utils.c externals.h constants.h main.h process.h fileIO.h \
 graphics.h image.h pixmaps/Circle.pm pixmaps/CircleSolid.pm \
 pixmaps/Connected.pm pixmaps/ConnectedClosed.pm \
 pixmaps/ConnecClosedSolid.pm pixmaps/Ellipse.pm pixmaps/EllipseSolid.pm \
 pixmaps/Eraser.pm pixmaps/Rectangle.pm pixmaps/RectangleSolid.pm \
 pixmaps/PaintBucket.pm pixmaps/Pencil.pm pixmaps/Line.pm \
 pixmaps/SelectArea.pm bitmaps/q_mark.xbm
