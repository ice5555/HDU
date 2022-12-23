all:receiver sender1 sender2

sender1:
	gcc sender1.c -pthread -o sender1

sender2:
	gcc sender2.c -pthread -o sender2

receiver:
	gcc receiver.c -pthread -o receiver

clean:
	rm -rf receiver sender1 sender2