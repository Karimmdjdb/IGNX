# Compilateur et Options
CXX = g++
CXXFLAGS = -I$(INC_DIR)
LDFLAGS = -Llibs -lsfml-graphics -lsfml-window -lsfml-system

# Répértoires
SRC_DIR = src
BUILD_DIR = build
INC_DIR = includes

# Exécutable
EXEC = $(BUILD_DIR)/main

# Listes des fichiers sources et objets
SRC = $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJ = $(foreach source, $(SRC), $(BUILD_DIR)/$(patsubst %.cpp,%.o, $(notdir $(source))))

# Verbose
VERBOSE ?= 0
ifeq ($(VERBOSE),1)
	ECHO = @echo
    CMD_PREFIX =
else
	ECHO = @:
    CMD_PREFIX = @
endif

# Régle par defaut
all : $(EXEC)

# Règle pour créer l'exécutable
$(EXEC) : $(OBJ)
	$(ECHO) "Génération de l'éxécutable $(EXEC)"
	$(CMD_PREFIX) mkdir -p $(BUILD_DIR)
	$(CMD_PREFIX) $(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Fonction pour générer les régles de compilation
define compile_rule
$(1) : $(2) $(wildcard $(3))
	$(ECHO) "Compilation du fichier $(2) en $(1)"
	$(CMD_PREFIX) mkdir -p $(BUILD_DIR)
	$(CMD_PREFIX) $(CXX) $(CXXFLAGS) -c $(2) -o $(1)
endef

$(foreach source, $(SRC), $(eval $(call compile_rule, $(BUILD_DIR)/$(patsubst %.cpp,%.o, $(notdir $(source))) , $(source), $(patsubst src%.cpp,includes%.hpp, $(source)))))

# Nettoyage
clean :
	@echo "Nettoyage de /$(BUILD_DIR)"
	$(CMD_PREFIX) rm -rf $(BUILD_DIR)

# Affichage des fichiers objets générés
show :
	@echo "Fichiers sources : $(SRC)"
	@echo "Fichiers objets : $(OBJ)"

# Executer
run : all
	$(ECHO) "Exécution du programme"
	@./$(EXEC)

test :
	$(foreach source, $(SRC), @echo )

