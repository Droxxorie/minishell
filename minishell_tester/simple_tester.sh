#!/bin/bash

# Testeur simplifié pour calibration avec bash
# Compare directement bash avec bash pour vérifier que la logique fonctionne

MINISHELL_PATH="../minishell"  # Chemin vers votre minishell
TEST_FILE="tests_original/builtins"

# Couleurs
RED="\e[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
BOLD="\e[1m"
END="\033[0m"

echo -e "${BLUE}${BOLD}=== TESTEUR SIMPLIFIÉ POUR CALIBRATION ===${END}"
echo ""

# Vérifier si le fichier de test existe
if [[ ! -f "$TEST_FILE" ]]; then
    echo -e "${RED}Erreur: fichier de test non trouvé: $TEST_FILE${END}"
    exit 1
fi

# Créer un minishell factice qui utilise bash
cat > fake_minishell << 'EOF'
#!/bin/bash
exec bash "$@"
EOF

chmod +x fake_minishell

echo -e "${YELLOW}Test de calibration: bash vs bash (via fake_minishell)${END}"
echo ""

total=0
passed=0
failed=0

while read -r test_cmd; do
    # Ignorer les lignes vides
    [[ -z "$test_cmd" || "$test_cmd" =~ ^[[:space:]]*$ ]] && continue
    
    ((total++))
    
    # Test bash direct
    BASH_OUTPUT=$(echo -e "$test_cmd" | bash 2> /dev/null)
    BASH_EXIT_CODE=$?
    
    # Test bash via fake_minishell
    MINI_OUTPUT=$(echo -e "$test_cmd" | ./fake_minishell 2> /dev/null)
    MINI_EXIT_CODE=$?
    
    # Comparaison simple
    if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" && "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" ]]; then
        ((passed++))
        printf "${GREEN}✅${END}"
    else
        ((failed++))
        printf "${RED}❌${END}"
        echo ""
        echo -e "${YELLOW}Échec pour: $test_cmd${END}"
        echo "  Bash output: '$BASH_OUTPUT'"
        echo "  Fake output: '$MINI_OUTPUT'"
        echo "  Bash exit: $BASH_EXIT_CODE"
        echo "  Fake exit: $MINI_EXIT_CODE"
    fi
    
    printf " ${YELLOW}%3d${END}" $total
    if (( total % 10 == 0 )); then
        printf "\n"
    fi
    
done < "$TEST_FILE"

# Calcul du pourcentage
percentage=0
if [[ $total -gt 0 ]]; then
    percentage=$(( (passed * 100) / total ))
fi

# Affichage des résultats
if [[ $percentage -ge 90 ]]; then
    color=$GREEN
    status="🎉 EXCELLENT"
elif [[ $percentage -ge 70 ]]; then
    color=$YELLOW
    status="👍 BON"
else
    color=$RED
    status="❌ À AMÉLIORER"
fi

printf "\n${color}${BOLD}%s: %d/%d (%d%%) - $status${END}\n" "builtins" $passed $total $percentage

# Nettoyer
rm -f fake_minishell

echo ""
if [[ $percentage -ge 90 ]]; then
    echo -e "${GREEN}${BOLD}🎊 CALIBRATION RÉUSSIE! Le testeur fonctionne correctement.${END}"
    echo -e "${BLUE}Vous pouvez maintenant tester votre vrai minishell.${END}"
else
    echo -e "${RED}${BOLD}❌ PROBLÈME DE CALIBRATION!${END}"
    echo -e "${YELLOW}Il y a un problème dans la logique du testeur.${END}"
fi
