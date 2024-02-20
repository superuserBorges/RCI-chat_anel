# Nome do compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall

# Nome do executável
TARGET = COR

# Arquivos fonte
SOURCES = main.c camada_topologica.c interface_utilizador.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
