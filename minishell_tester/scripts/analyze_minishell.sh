#!/bin/bash

# Script d'analyse des résultats de tests sans modifier minishell
MINISHELL_PATH="../minishell"

RED="\e[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
PURPLE="\033[0;35m"
BOLD="\e[1m"
END="\033[0m"

echo -e "${BLUE}${BOLD}=== ANALYSE DES TESTS MINISHELL ===${END}"
echo "Ce script analyse votre minishell sans le modifier"
echo ""

# Vérifier si minishell existe
if [[ ! -f "$MINISHELL_PATH" ]]; then
    echo -e "${RED}Erreur: minishell non trouvé dans $MINISHELL_PATH${END}"
    echo "Compilez d'abord votre minishell avec 'make' dans le dossier parent"
    exit 1
fi

# Fonction pour tester une catégorie et compter les résultats
analyze_category() {
    local category=$1
    local file="csv_$category"

    if [[ ! -f "$file" ]]; then
        return
    fi

    local total=0
    local passed=0
    local failed=0

    echo -e "${YELLOW}Analyse: $category${END}"

    # Helper commands
    REMOVE_COLORS="sed s/\x1B\[[0-9;]\{1,\}[A-Za-z]//g"
    REMOVE_EXIT="grep -v ^exit$"
    PROMPT=$(echo -e "\nexit\n" | $MINISHELL_PATH | head -n 1 | $REMOVE_COLORS)

    mkdir -p ./outfiles ./mini_outfiles ./bash_outfiles

    while read teste; do
        ((total++))

        # Test minishell
        rm -rf ./outfiles/* ./mini_outfiles/*
        MINI_OUTPUT=$(echo -e "$teste" | $MINISHELL_PATH 2> /dev/null | $REMOVE_COLORS | grep -vF "$PROMPT" | $REMOVE_EXIT)
        cp ./outfiles/* ./mini_outfiles &>/dev/null
        MINI_EXIT_CODE=$(echo -e "$MINISHELL_PATH\n$teste\necho \$?\nexit\n" | bash 2> /dev/null | $REMOVE_COLORS | grep -vF "$PROMPT" | $REMOVE_EXIT | tail -n 1)

        # Test bash
        rm -rf ./outfiles/* ./bash_outfiles/*
        BASH_OUTPUT=$(echo -e "$teste" | bash 2> /dev/null)
        BASH_EXIT_CODE=$(echo $?)
        cp ./outfiles/* ./bash_outfiles &>/dev/null

        # Comparaison
        OUTFILES_DIFF=$(diff --brief ./mini_outfiles ./bash_outfiles 2>/dev/null)

        if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" && "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" && -z "$OUTFILES_DIFF" ]]; then
            ((passed++))
        else
            ((failed++))
        fi

        # Affichage du progrès
        if (( total % 50 == 0 )); then
            printf "  Testé: %d/%d\r" $total $(wc -l < "$file")
        fi

    done < "$file"

    # Nettoyage
    rm -rf ./outfiles ./mini_outfiles ./bash_outfiles

    # Calcul du pourcentage
    local percentage=0
    if [[ $total -gt 0 ]]; then
        percentage=$(( (passed * 100) / total ))
    fi

    # Affichage des résultats avec couleurs
    if [[ $percentage -ge 90 ]]; then
        color=$GREEN
    elif [[ $percentage -ge 70 ]]; then
        color=$YELLOW
    else
        color=$RED
    fi

    printf "  ${color}%-15s: %3d/%3d (%3d%%)${END}\n" "$category" $passed $total $percentage

    # Retourner les statistiques
    echo "$passed $total $percentage"
}

# Analyse de toutes les catégories
echo "Analyse en cours..."
echo ""

categories=(
    "syntax"
    "builtins"
    "pipes"
    "redirects"
    "variables"
    "quotes"
    "paths"
    "misc"
)

total_passed=0
total_tests=0
declare -A results

for category in "${categories[@]}"; do
    if [[ -f "csv_$category" ]]; then
        result=$(analyze_category "$category")
        if [[ -n "$result" ]]; then
            read passed total percentage <<< "$result"
            results["$category"]="$passed $total $percentage"
            ((total_passed += passed))
            ((total_tests += total))
        fi
    fi
done

echo ""
echo -e "${PURPLE}${BOLD}=== RÉSUMÉ GLOBAL ===${END}"

overall_percentage=0
if [[ $total_tests -gt 0 ]]; then
    overall_percentage=$(( (total_passed * 100) / total_tests ))
fi

if [[ $overall_percentage -ge 90 ]]; then
    color=$GREEN
    status="🎉 EXCELLENT"
elif [[ $overall_percentage -ge 70 ]]; then
    color=$YELLOW
    status="👍 BON"
elif [[ $overall_percentage -ge 50 ]]; then
    color=$YELLOW
    status="⚠️  MOYEN"
else
    color=$RED
    status="❌ À AMÉLIORER"
fi

echo -e "${color}${BOLD}Total: $total_passed/$total_tests ($overall_percentage%) - $status${END}"
echo ""

# Recommandations
echo -e "${BLUE}${BOLD}=== RECOMMANDATIONS ===${END}"

for category in "${categories[@]}"; do
    if [[ -n "${results[$category]}" ]]; then
        read passed total percentage <<< "${results[$category]}"

        if [[ $percentage -lt 70 ]]; then
            case $category in
                "syntax")
                    echo -e "${YELLOW}• Améliorer la gestion des erreurs de syntaxe${END}"
                    ;;
                "builtins")
                    echo -e "${YELLOW}• Vérifier l'implémentation des builtins (echo, cd, pwd, export, unset, env, exit)${END}"
                    ;;
                "pipes")
                    echo -e "${YELLOW}• Améliorer la gestion des pipes${END}"
                    ;;
                "redirects")
                    echo -e "${YELLOW}• Vérifier les redirections (>, <, >>, <<)${END}"
                    ;;
                "variables")
                    echo -e "${YELLOW}• Améliorer l'expansion des variables d'environnement${END}"
                    ;;
                "quotes")
                    echo -e "${YELLOW}• Vérifier la gestion des guillemets et apostrophes${END}"
                    ;;
            esac
        fi
    fi
done

if [[ $overall_percentage -ge 90 ]]; then
    echo -e "${GREEN}• Votre minishell fonctionne très bien ! 🎊${END}"
elif [[ $overall_percentage -ge 70 ]]; then
    echo -e "${GREEN}• Bon travail ! Quelques ajustements à faire${END}"
else
    echo -e "${YELLOW}• Continuez le développement, vous êtes sur la bonne voie${END}"
fi

echo ""
echo -e "${BLUE}Pour voir les détails des échecs, utilisez :${END}"
echo "  ./test_csv [catégorie]"
echo ""
echo -e "${BLUE}Pour tester une catégorie spécifique :${END}"
echo "  ./run_all_csv_tests.sh [catégorie]"
