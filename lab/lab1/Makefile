run-serial: build
	./serial

run-crcw: build
	./crcw

run-crew: build
	./crew

build: serial crcw crew

serial:
	g++ serial.cpp -o serial 

crew: 
	g++ crew.cpp -o crew -pthread

crcw:
	g++ crcw.cpp -o crcw -pthread 

clean:
	rm crcw crew serial