all: receiver sender

receiver:
	gcc receiver.c -pthread -o receiver

sender:
	gcc sender.c -pthread -o sender

clean:
	rm -rf sender receiver