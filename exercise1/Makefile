
FILENAME=game_of_life.c
EXEC=game_of_life.exe
PLAYGROUND_ORDERED=snapshots_ordered/playground.pbm
PLAYGROUND_STATIC=snapshots_static/playground.pbm

STEPS=20
SNAP=5
DIM=10

.PHONY:compile init_ordered init_static run_ordered run_static

compile: $(EXEC)

$(EXEC) : $(FILENAME)
	gcc $(FILENAME) -o $(EXEC)

init_ordered : $(EXEC)
	./$(EXEC) -i -f $(PLAYGROUND_ORDERED) -k $(DIM)

init_static : $(EXEC)
	./$(EXEC) -i -f $(PLAYGROUND_STATIC) -k $(DIM)

run_ordered : $(EXEC)
	./$(EXEC) -r -f $(PLAYGROUND_ORDERED) -e 0 -k $(DIM) -n $(STEPS) -s $(SNAP)

run_static : $(EXEC)
	./$(EXEC) -r -f $(PLAYGROUND_STATIC) -e 1 -k $(DIM) -n $(STEPS) -s $(SNAP)

clean:
	rm -rf $(EXEC)


