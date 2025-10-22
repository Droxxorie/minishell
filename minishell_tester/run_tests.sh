#!/bin/bash

# Script principal amélioré pour le testeur minishell
# Version organisée et portable

# Charger la configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ -f "$SCRIPT_DIR/config.sh" ]]; then
    source "$SCRIPT_DIR/config.sh"
else
    # Configuration par défaut si config.sh n'existe pas
    MINISHELL_PATH="../minishell"
    DEFAULT_VERBOSE=false
    ENABLE_COLORS=true
    AUTO_CLEANUP=true
    CHECK_PREREQUISITES=true
fi
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

# Fonction d'aide
show_help() {
    echo -e "${BLUE}${BOLD}=== MINISHELL TESTER - GUIDE D'UTILISATION ===${END}"
    echo ""
    echo "Usage: $0 [OPTIONS] [CATEGORY]"
    echo ""
    echo "Options:"
    echo "  -h, --help          Afficher cette aide"
    echo "  -a, --all           Exécuter tous les tests"
    echo "  -c, --csv           Exécuter seulement les tests CSV"
    echo "  -o, --original      Exécuter seulement les tests originaux"
    echo "  -e, --errors        Exécuter les tests d'erreur"
    echo "  -l, --list          Lister toutes les catégories disponibles"
    echo "  -v, --verbose       Mode verbeux"
    echo "  --clean             Nettoyer les fichiers temporaires"
    echo ""
    echo "Catégories disponibles:"
    echo "  Tests originaux:"
    echo "    builtins, pipes, redirects, extras, syntax, os_specific, wildcards"
    echo ""
    echo "  Tests CSV:"
    echo "    csv_syntax, csv_builtins, csv_pipes, csv_redirects"
    echo "    csv_variables, csv_quotes, csv_paths, csv_misc"
    echo "    csv_syntax_errors, csv_command_errors, csv_permission_errors"
    echo "    csv_directory_errors, csv_invalid_identifier"
    echo ""
    echo "Exemples:"
    echo "  $0                    # Tests par défaut (builtins, pipes, redirects, extras + CSV)"
    echo "  $0 --all              # Tous les tests"
    echo "  $0 builtins           # Tests des builtins seulement"
    echo "  $0 csv_builtins       # Tests CSV des builtins"
    echo "  $0 --errors           # Tests d'erreur seulement"
    echo "  $0 --clean            # Nettoyer les fichiers temporaires"
}

# Fonction de nettoyage
clean_temp_files() {
    echo -e "${YELLOW}Nettoyage des fichiers temporaires...${END}"
    rm -rf "$TEMP_DIR"/* 2>/dev/null
    rm -rf ./outfiles ./mini_outfiles ./bash_outfiles 2>/dev/null
    echo -e "${GREEN}✓ Nettoyage terminé${END}"
}

# Fonction pour lister les catégories
list_categories() {
    echo -e "${BLUE}${BOLD}=== CATÉGORIES DISPONIBLES ===${END}"
    echo ""
    echo -e "${YELLOW}Tests originaux:${END}"
    for file in "$TESTS_ORIGINAL_DIR"/*; do
        if [[ -f "$file" ]]; then
            basename_file=$(basename "$file")
            count=$(wc -l < "$file" 2>/dev/null || echo "0")
            echo "  $basename_file ($count tests)"
        fi
    done
    echo ""
    echo -e "${YELLOW}Tests CSV:${END}"
    for file in "$TESTS_CSV_DIR"/csv_*; do
        if [[ -f "$file" ]]; then
            basename_file=$(basename "$file")
            count=$(wc -l < "$file" 2>/dev/null || echo "0")
            echo "  $basename_file ($count tests)"
        fi
    done
}

# Fonction pour vérifier les prérequis
check_prerequisites() {
    if [[ ! -f "$MINISHELL_PATH" ]]; then
        echo -e "${RED}Erreur: minishell non trouvé dans $MINISHELL_PATH${END}"
        echo "Compilez d'abord votre minishell avec 'make' dans le dossier parent"
        exit 1
    fi
    
    # Créer les répertoires temporaires
    mkdir -p "$TEMP_DIR" ./outfiles ./mini_outfiles ./bash_outfiles
}

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
        
        if [[ "$VERBOSE" == "true" ]]; then
            printf " ${YELLOW}Test %3d:${END} $test_cmd\n" $total
            if [[ "$MINI_OUTPUT" != "$BASH_OUTPUT" ]]; then
                echo "  mini output = ($MINI_OUTPUT)"
                echo "  bash output = ($BASH_OUTPUT)"
            fi
            if [[ "$MINI_EXIT_CODE" != "$BASH_EXIT_CODE" ]]; then
                echo "  mini exit code = $MINI_EXIT_CODE"
                echo "  bash exit code = $BASH_EXIT_CODE"
            fi
        else
            printf " ${YELLOW}%3d${END}" $total
            if (( total % 10 == 0 )); then
                printf "\n"
            fi
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

# Variables par défaut
RUN_ALL=false
RUN_CSV=false
RUN_ORIGINAL=false
RUN_ERRORS=false
VERBOSE=false
CLEAN=false

# Parsing des arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -a|--all)
            RUN_ALL=true
            shift
            ;;
        -c|--csv)
            RUN_CSV=true
            shift
            ;;
        -o|--original)
            RUN_ORIGINAL=true
            shift
            ;;
        -e|--errors)
            RUN_ERRORS=true
            shift
            ;;
        -l|--list)
            list_categories
            exit 0
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        -*)
            echo -e "${RED}Option inconnue: $1${END}"
            show_help
            exit 1
            ;;
        *)
            # Catégorie spécifique
            CATEGORY="$1"
            shift
            ;;
    esac
done

# Nettoyage si demandé
if [[ "$CLEAN" == "true" ]]; then
    clean_temp_files
    exit 0
fi

# Vérification des prérequis
check_prerequisites

# Bannière
echo -e "${RED}${BOLD}"
echo "🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥";
echo "                                                                                             ";
echo " ██    ██ █ ██    █ █ █████ █   █ █████ █     █       ██████ █████ █████ ██████ █████ █████  ";
echo " ███  ███ █ ███   █ █ █     █   █ █     █     █         ██   █     █       ██   █     █   ██ ";
echo " █ ████ █ █ █ ██  █ █ █████ █████ ████  █     █         ██   ████  █████   ██   ████  █████  ";
echo " █  ██  █ █ █  ██ █ █     █ █   █ █     █     █         ██   █         █   ██   █     █   ██ ";
echo " █      █ █ █   ███ █ █████ █   █ █████ █████ █████     ██   █████ █████   ██   █████ █   ██ ";
echo "                                                                                             ";
echo "🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥";
echo -e "${END}"

# Déterminer quels tests exécuter
if [[ -n "$CATEGORY" ]]; then
    # Test d'une catégorie spécifique
    if [[ -f "$TESTS_ORIGINAL_DIR/$CATEGORY" ]]; then
        run_tests "$TESTS_ORIGINAL_DIR/$CATEGORY" "$CATEGORY"
    elif [[ -f "$TESTS_CSV_DIR/$CATEGORY" ]]; then
        run_tests "$TESTS_CSV_DIR/$CATEGORY" "$CATEGORY"
    else
        echo -e "${RED}Erreur: catégorie '$CATEGORY' non trouvée${END}"
        echo "Utilisez '$0 --list' pour voir les catégories disponibles"
        exit 1
    fi
elif [[ "$RUN_ALL" == "true" ]]; then
    # Tous les tests
    total_failed=0
    for file in "$TESTS_ORIGINAL_DIR"/* "$TESTS_CSV_DIR"/csv_*; do
        if [[ -f "$file" ]]; then
            basename_file=$(basename "$file")
            run_tests "$file" "$basename_file"
            total_failed=$((total_failed + $?))
        fi
    done
elif [[ "$RUN_CSV" == "true" ]]; then
    # Tests CSV seulement
    total_failed=0
    for file in "$TESTS_CSV_DIR"/csv_*; do
        if [[ -f "$file" ]]; then
            basename_file=$(basename "$file")
            run_tests "$file" "$basename_file"
            total_failed=$((total_failed + $?))
        fi
    done
elif [[ "$RUN_ORIGINAL" == "true" ]]; then
    # Tests originaux seulement
    total_failed=0
    for file in "$TESTS_ORIGINAL_DIR"/*; do
        if [[ -f "$file" ]]; then
            basename_file=$(basename "$file")
            run_tests "$file" "$basename_file"
            total_failed=$((total_failed + $?))
        fi
    done
elif [[ "$RUN_ERRORS" == "true" ]]; then
    # Tests d'erreur seulement
    total_failed=0
    for file in "$TESTS_CSV_DIR"/csv_*errors* "$TESTS_CSV_DIR"/csv_*invalid*; do
        if [[ -f "$file" ]]; then
            basename_file=$(basename "$file")
            run_tests "$file" "$basename_file"
            total_failed=$((total_failed + $?))
        fi
    done
else
    # Tests par défaut (comportement original)
    default_tests=("builtins" "pipes" "redirects" "extras" "csv_syntax" "csv_builtins" "csv_pipes" "csv_redirects" "csv_variables" "csv_quotes" "csv_paths" "csv_misc")
    total_failed=0
    for test in "${default_tests[@]}"; do
        if [[ -f "$TESTS_ORIGINAL_DIR/$test" ]]; then
            run_tests "$TESTS_ORIGINAL_DIR/$test" "$test"
            total_failed=$((total_failed + $?))
        elif [[ -f "$TESTS_CSV_DIR/$test" ]]; then
            run_tests "$TESTS_CSV_DIR/$test" "$test"
            total_failed=$((total_failed + $?))
        fi
    done
fi

# Nettoyage final
clean_temp_files

# Résultat final
if [[ $total_failed -eq 0 ]]; then
    echo -e "${GREEN}${BOLD}🎊 🎊 🎊 TOUS LES TESTS RÉUSSIS! 🎉 🎉 🎉${END}"
    exit 0
else
    echo -e "${RED}${BOLD}😭 $total_failed catégorie(s) ont échoué 😭${END}"
    exit 1
fi
