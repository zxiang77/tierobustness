makeAndTest:
	gcc ShortestTree.c -I /usr/local/include/igraph -L /usr/local/lib -ligraph
	./a.out /Users/zilixiang/SDL/tie_range/data/simmons81.graphml
	
makeTree:
	gcc ShortestTree.c -I /usr/local/include/igraph -L /usr/local/lib -ligraph

make:
	gcc * -I /usr/local/include/igraph -L /usr/local/lib -ligraph

clean:
	rm global.h.gch
	rm a.out

test:
	./a.out /Users/zilixiang/SDL/tie_range/data/Texas84.graphml /Users/zilixiang/SDL/tierobustness/data/o.graphml

testT:
	./a.out /Users/zilixiang/SDL/tie_range/data/simmons81.graphml
