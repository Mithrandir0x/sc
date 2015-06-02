
setup:
	sudo apt-get install libopenmpi-dev
	sudo apt-get install openmpi-bin

clean:
	rm -rf helloWorld.exe

helloWorld: clean
	mpicc helloWorld.c -o helloWorld.exe
	mpirun -np 4 ./helloWorld.exe
