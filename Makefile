CC       =  gcc
CFLAGS   = -Wall -O2 -g
LIB      = -lSDL -lSDL_image -lGLU -lGL -lm  
INCLUDES = 

OBJ      = main.o ship.o background.o collision.o enemy.o missile.o
RM       = rm -f
BIN      = jeu

all : $(BIN) 

$(BIN) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) $(INCLUDES)  -o $(BIN)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN) &"
	@echo "--------------------------------------------------------------"

main.o : main.c header.h
	@echo "compile jeu"
	$(CC) $(CFLAGS) -c $<  
	@echo "done..."

ship.o : ship.c header.h
	@echo "compile ship"
	$(CC) $(CFLAGS) -c $<  
	@echo "done..."

background.o : background.c header.h
	@echo "compile background"
	$(CC) $(CFLAGS) -c $<  
	@echo "done..."

collision.o : collision.c header.h
	@echo "compile collision"
	$(CC) $(CFLAGS) -c $<  
	@echo "done..."

enemy.o : enemy.c header.h
	@echo "compile enemy"
	$(CC) $(CFLAGS) -c $<  
	@echo "done..."

missile.o : missile.c header.h
	@echo "compile missile"
	$(CC) $(CFLAGS) -c $<  
	@echo "done..."

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ) $(BIN) 


