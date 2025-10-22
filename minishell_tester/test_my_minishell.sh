#!/bin/bash

# Testeur adapté pour votre minishell
# Gère le problème "not interactive"

MINISHELL_PATH="../minishell"
TEST_FILE="tests_original/builtins"

# Couleurs
RED="\e[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
BOLD="\e[1m"
END="\033[0m"

echo -e "${BLUE}${BOLD}=== TESTEUR ADAPTÉ POUR VOTRE MINISHELL ===${END}"
echo ""

# Vérifier si le fichier de test existe
if [[ ! -f "$TEST_FILE" ]]; then
    echo -e "${RED}Erreur: fichier de test non trouvé: $TEST_FILE${END}"
    exit 1
fi

# Vérifier si minishell existe
if [[ ! -f "$MINISHELL_PATH" ]]; then
    echo -e "${RED}Erreur: minishell non trouvé dans $MINISHELL_PATH${END}"
    exit 1
fi

echo -e "${YELLOW}Test de votre minishell...${END}"
echo ""

# Test manuel simple
echo -e "${BLUE}Test manuel:${END}"
echo "Commande: echo hello"
echo "Résultat:"
echo "echo hello" | $MINISHELL_PATH 2>&1
echo ""

echo "Commande: pwd"
echo "Résultat:"
echo "pwd" | $MINISHELL_PATH 2>&1
echo ""

echo "Commande: exit"
echo "Résultat:"
echo "exit" | $MINISHELL_PATH 2>&1
echo ""

# Test avec différentes approches
echo -e "${BLUE}Test avec différentes approches:${END}"

# Approche 1: Avec heredoc
echo "Approche 1 - Heredoc:"
$MINISHELL_PATH << 'EOF'
echo hello
exit
EOF
echo ""

# Approche 2: Avec printf
echo "Approche 2 - printf:"
printf "echo hello\nexit\n" | $MINISHELL_PATH 2>&1
echo ""

# Approche 3: Avec echo et pipe
echo "Approche 3 - echo pipe:"
echo -e "echo hello\nexit" | $MINISHELL_PATH 2>&1
echo ""

# Approche 4: Avec timeout
echo "Approche 4 - timeout:"
timeout 5 bash -c "echo 'echo hello' | $MINISHELL_PATH" 2>&1
echo ""

echo -e "${GREEN}✅ Tests manuels terminés${END}"
echo ""
echo -e "${YELLOW}Si vous voyez 'minishell: not interactive', votre minishell a besoin d'être modifié pour gérer le mode non-interactif.${END}"
echo -e "${YELLOW}Si vous voyez les sorties attendues, le problème vient du testeur.${END}"
