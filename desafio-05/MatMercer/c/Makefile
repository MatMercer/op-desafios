fasjson: fasjson.c fasticionarios.c fasticionarios.h
	gcc fasjson.c fasticionarios.c -O3 -g0 -o fasjson
fasdebug: fasjson.c fasticionarios.c fasticionarios.h
	gcc fasjson.c fasticionarios.c fasticionarios.h -O3 -g0 -o fasdebug -D DEBUG
fastest: fastest.c fasticionarios.c fasticionarios.h
	gcc fastest.c fasticionarios.c fasticionarios.h -o fastest -O3 -D DEBUG
clean:
	rm -v fastest fasdebug fasjson

