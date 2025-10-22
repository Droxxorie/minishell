#!/bin/bash

# Script pour calibrer le testeur avec bash
# Ce script simule un minishell en utilisant bash pour vérifier que le testeur fonctionne

echo "🔧 Calibrage du testeur avec bash..."

# Créer un minishell factice qui utilise bash
cat > fake_minishell << 'EOF'
#!/bin/bash
# Minishell factice qui utilise bash
exec bash "$@"
EOF

chmod +x fake_minishell

# Tester manuellement quelques commandes
echo "📋 Test manuel avec bash vs fake_minishell..."

echo "Test 1: echo hello"
echo "Bash:"
echo "echo hello" | bash
echo "Fake minishell:"
echo "echo hello" | ./fake_minishell

echo ""
echo "Test 2: pwd"
echo "Bash:"
echo "pwd" | bash
echo "Fake minishell:"
echo "pwd" | ./fake_minishell

echo ""
echo "Test 3: exit"
echo "Bash exit code:"
echo "exit" | bash; echo "Exit code: $?"
echo "Fake minishell exit code:"
echo "exit" | ./fake_minishell; echo "Exit code: $?"

# Maintenant tester avec le testeur
echo ""
echo "📋 Test avec le testeur..."

# Créer un script temporaire qui utilise notre fake_minishell
cat > run_tests_fake.sh << 'EOF'
#!/bin/bash

# Script temporaire pour tester avec fake_minishell
MINISHELL_PATH="./fake_minishell"

# Copier le script principal et modifier le chemin
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_ORIGINAL_DIR="$SCRIPT_DIR/tests_original"
TESTS_CSV_DIR="$SCRIPT_DIR/tests_csv"
SCRIPTS_DIR="$SCRIPT_DIR/scripts"
TEMP_DIR="$SCRIPT_DIR/temp_files"

# Couleurs
RED="\e[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
PURPLE="\033[0;35m"
BOLD="\e[1m"
END="\033[0m"

# Fonction pour exécuter les tests
run_tests() {
    local test_file="$1"
    local category_name="$2"
    
    if [[ ! -f "$test_file" ]]; then
        echo -e "${RED}Erreur: fichier de test non trouvé: $test_file${END}"
        return 1
    fi
    
    echo -e "${BLUE}${BOLD}=== Tests: $category_name ===${END}"
    
    local total=0
    local passed=0
    local failed=0
    
    # Helper commands
    REMOVE_COLORS="sed s/\x1B\[[0-9;]\{1,\}[A-Za-z]//g"
    REMOVE_EXIT="grep -v ^exit$"
    PROMPT=$(echo -e "\nexit\n" | $MINISHELL_PATH | head -n 1 | $REMOVE_COLORS)
    
    while read -r test_cmd; do
        [[ -z "$test_cmd" || "$test_cmd" =~ ^[[:space:]]*$ ]] && continue
        
        ((total++))
        
        # Test minishell
        rm -rf ./outfiles/* ./mini_outfiles/* 2>/dev/null
        MINI_OUTPUT=$(echo -e "$test_cmd" | $MINISHELL_PATH 2> /dev/null | $REMOVE_COLORS | grep -vF "$PROMPT" | $REMOVE_EXIT)
        cp ./outfiles/* ./mini_outfiles &>/dev/null
        MINI_EXIT_CODE=$(echo -e "$MINISHELL_PATH\n$test_cmd\necho \$?\nexit\n" | bash 2> /dev/null | $REMOVE_COLORS | grep -vF "$PROMPT" | $REMOVE_EXIT | tail -n 1)
        
        # Test bash
        rm -rf ./outfiles/* ./bash_outfiles/* 2>/dev/null
        BASH_OUTPUT=$(echo -e "$test_cmd" | bash 2> /dev/null)
        BASH_EXIT_CODE=$?
        cp ./outfiles/* ./bash_outfiles &>/dev/null
        
        # Comparaison
        OUTFILES_DIFF=$(diff --brief ./mini_outfiles ./bash_outfiles 2>/dev/null)
        
        if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" && "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" && -z "$OUTFILES_DIFF" ]]; then
            ((passed++))
            printf "${GREEN}✅${END}"
        else
            ((failed++))
            printf "${RED}❌${END}"
        fi
        
        printf " ${YELLOW}%3d${END}" $total
        if (( total % 10 == 0 )); then
            printf "\n"
        fi
        
    done < "$test_file"
    
    # Nettoyage
    rm -rf ./outfiles ./mini_outfiles ./bash_outfiles 2>/dev/null
    
    # Calcul du pourcentage
    local percentage=0
    if [[ $total -gt 0 ]]; then
        percentage=$(( (passed * 100) / total ))
    fi
    
    # Affichage des résultats
    if [[ $percentage -ge 90 ]]; then
        color=$GREEN
    elif [[ $percentage -ge 70 ]]; then
        color=$YELLOW
    else
        color=$RED
    fi
    
    printf "\n${color}${BOLD}%s: %d/%d (%d%%)${END}\n" "$category_name" $passed $total $percentage
    
    return $failed
}

# Créer les répertoires temporaires
mkdir -p "$TEMP_DIR" ./outfiles ./mini_outfiles ./bash_outfiles

# Test des builtins
run_tests "$TESTS_ORIGINAL_DIR/builtins" "builtins"

# Nettoyage final
rm -rf ./outfiles ./mini_outfiles ./bash_outfiles 2>/dev/null

EOF

chmod +x run_tests_fake.sh
./run_tests_fake.sh
rm -f run_tests_fake.sh

# Nettoyer
rm -f fake_minishell

echo "✅ Calibrage terminé!"
