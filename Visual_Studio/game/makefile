NAME=journey
LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CXX := g++

all: game

%.o: %.cpp
	$(CXX) -c $< -o $@

%.o: %.hpp
	$(CXX) -c $< -o $@

game: character.o game.o intro.o main.o map.o md5.o menu.o monster.o pause.o player.o savegame.o schrift.o sounds.o
	@echo "** Building $(NAME)"
	$(CXX) -o $(NAME) character.o game.o intro.o main.o map.o md5.o menu.o monster.o pause.o player.o savegame.o schrift.o sounds.o $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -f $(NAME) *.o

install:
	@echo "** Installing..."
	mkdir -p ~/$(NAME)
	cp $(NAME) ~/$(NAME)/
	cp -r include ~/$(NAME)/

uninstall:
	@echo "** Uninstalling..."
	rm -r ~/$(NAME)/