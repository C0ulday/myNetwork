# Variables
CC = gcc
CFLAGS = -Wall -ansi -std=c99
OBJDIR = build

# Fichiers source
SRCS = admin.c serveur.c client.c fonctions.c

# Fichiers objets
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

# Cible par défaut
all: admin serveur client

# Cible pour compiler les fichiers objets
$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour l'exécutable admin
admin: $(OBJDIR)/admin.o $(OBJDIR)/fonctions.o
	$(CC) $^ -o $@

# Cible pour l'exécutable serveur
serveur: $(OBJDIR)/serveur.o $(OBJDIR)/fonctions.o
	$(CC) $^ -o $@ -g

# Cible pour l'exécutable client
client: $(OBJDIR)/client.o $(OBJDIR)/fonctions.o
	$(CC) $^ -o $@

# Cible pour nettoyer les fichiers objets et exécutables
clean:
	rm -rf $(OBJDIR) admin serveur client
