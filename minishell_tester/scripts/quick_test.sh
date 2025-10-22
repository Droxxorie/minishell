#!/bin/bash

# Test rapide de base pour minishell
MINISHELL_PATH="../minishell"

RED="\e[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
END="\033[0m"

echo -e "${BLUE}=== TEST RAPIDE MINISHELL ===${END}"

if [[ ! -f "$MINISHELL_PATH" ]]; then
    echo -e "${RED}Erreur: minishell non trouvé${END}"
    exit 1
fi

# Tests de base
basic_tests=(
    "echo hello"
    "echo 'hello world'"
    'echo "hello world"'
    "pwd"
    "env | head -3"
    "export TEST=value"
    "echo \$TEST"
    "unset TEST"
    "cd ."
    "exit"
)

passed=0
total=${#basic_tests[@]}

echo "Tests de base ($total tests):"

for test in "${basic_tests[@]}"; do
    echo -n "  Testing: $test ... "

    # Test avec minishell
    mini_result=$(echo -e "$test\nexit" | $MINISHELL_PATH 2>/dev/null | head -n -1)

    # Test avec bash
    bash_result=$(echo "$test" | bash 2>/dev/null)

    if [[ "$mini_result" == "$bash_result" ]]; then
        echo -e "${GREEN}✅${END}"
        ((passed++))
    else
        echo -e "${RED}❌${END}"
        echo "    Expected: '$bash_result'"
        echo "    Got:      '$mini_result'"
    fi
done

echo ""
percentage=$(( (passed * 100) / total ))

if [[ $percentage -ge 80 ]]; then
    color=$GREEN
    status="🎉"
elif [[ $percentage -ge 60 ]]; then
    color=$YELLOW
    status="👍"
else
    color=$RED
    status="⚠️"
fi

echo -e "${color}Résultat: $passed/$total ($percentage%) $status${END}"

if [[ $passed -eq $total ]]; then
    echo -e "${GREEN}Parfait ! Votre minishell passe tous les tests de base${END}"
elif [[ $percentage -ge 80 ]]; then
    echo -e "${YELLOW}Très bien ! Quelques détails à ajuster${END}"
else
    echo -e "${YELLOW}Continuez le développement${END}"
fi

echo ""
echo "Pour une analyse complète, utilisez: ./analyze_minishell.sh"
