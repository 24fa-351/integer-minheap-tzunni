minheap: asst6.c
	gcc -o minheap asst6.c some_heap.c

clean:
	rm minheap

output: minheap
	./minheap > output.txt