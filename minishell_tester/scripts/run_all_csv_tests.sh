#!/bin/bash

# Script principal pour tous les tests CSV extraits
# Usage: ./run_all_csv_tests.sh [category]

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

RED="\e[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
PURPLE="\033[0;35m"
BOLD="\e[1m"
END="\033[0m"

# Fonction pour afficher l'aide
show_help() {
    echo "Usage: $0 [OPTION] [CATEGORY]"
    echo ""
    echo "Options:"
    echo "  -h, --help     Afficher cette aide"
    echo "  -l, --list     Lister toutes les catégories disponibles"
    echo "  -a, --all      Exécuter tous les tests (défaut)"
    echo "  -e, --errors   Exécuter seulement les tests d'erreur"
    echo ""
    echo "Catégories disponibles:"
    echo "  syntax         Tests de syntaxe"
    echo "  builtins       Tests des builtins"
    echo "  pipes          Tests des pipes"
    echo "  redirects      Tests des redirections"
    echo "  variables      Tests des variables"
    echo "  quotes         Tests des quotes"
    echo "  paths          Tests des chemins"
    echo "  misc           Tests divers"
    echo "  errors         Tous les tests d'erreur"
    echo ""
    echo "Exemples:"
    echo "  $0                    # Tous les tests"
    echo "  $0 builtins          # Seulement les builtins"
    echo "  $0 --errors          # Seulement les tests d'erreur"
}

# Fonction pour lister les catégories
list_categories() {
    echo "Catégories de tests CSV disponibles:"
    echo ""
    for file in csv_*; do
        if [[ -f "$file" ]]; then
            count=$(wc -l < "$file")
            category=$(echo "$file" | sed 's/csv_//')
            printf "  %-20s %d tests\n" "$category" "$count"
        fi
    done
}

# Fonction pour exécuter une catégorie de tests
run_category() {
    local category=$1
    local file="csv_$category"

    if [[ ! -f "$file" ]]; then
        echo -e "${RED}Erreur: Catégorie '$category' non trouvée${END}"
        return 1
    fi

    echo -e "${BLUE}Exécution des tests: $category${END}"
    ./test_csv "$category"
    return $?
}

# Fonction pour exécuter tous les tests d'erreur
run_error_tests() {
    echo -e "${YELLOW}=== TESTS D'ERREUR ===${END}"

    error_categories=(
        "syntax_errors"
        "command_errors"
        "permission_errors"
        "directory_errors"
        "invalid_identifier"
    )

    total_ok=0
    total_tests=0

    for category in "${error_categories[@]}"; do
        if [[ -f "csv_$category" ]]; then
            echo -e "${BLUE}Tests d'erreur: $category${END}"
            ./test_csv "$category"
            # Note: le script test_csv ne retourne pas le nombre de tests réussis
            # donc on fait une estimation simple
        fi
    done

    echo -e "${YELLOW}Test des codes de sortie:${END}"
    ./test_exit_codes
}

# Fonction pour exécuter tous les tests
run_all_tests() {
    echo -e "${PURPLE}${BOLD}=== TOUS LES TESTS CSV ===${END}"

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

    failed_categories=()

    for category in "${categories[@]}"; do
        echo ""
        if run_category "$category"; then
            echo -e "${GREEN}✅ $category: RÉUSSI${END}"
        else
            echo -e "${RED}❌ $category: ÉCHEC${END}"
            failed_categories+=("$category")
        fi
    done

    echo ""
    echo -e "${PURPLE}${BOLD}=== RÉSUMÉ ===${END}"

    if [[ ${#failed_categories[@]} -eq 0 ]]; then
        echo -e "${GREEN}🎉 TOUS LES TESTS RÉUSSIS! 🎉${END}"
        return 0
    else
        echo -e "${RED}❌ Catégories en échec: ${failed_categories[*]}${END}"
        return 1
    fi
}

# Parsing des arguments
case "$1" in
    -h|--help)
        show_help
        exit 0
        ;;
    -l|--list)
        list_categories
        exit 0
        ;;
    -e|--errors)
        run_error_tests
        exit $?
        ;;
    -a|--all|"")
        run_all_tests
        exit $?
        ;;
    *)
        # Assume c'est une catégorie
        category="$1"
        run_category "$category"
        exit $?
        ;;
esac
