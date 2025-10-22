#!/bin/bash

# Script de test rapide pour vérifier que le testeur fonctionne
# Exécute quelques tests basiques pour valider l'installation

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Couleurs
GREEN="\033[0;32m"
RED="\e[0;31m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
BOLD="\e[1m"
END="\033[0m"

echo -e "${BLUE}${BOLD}=== TEST RAPIDE DU TESTEUR MINISHELL ===${END}"
echo ""

# Test 1: Vérifier la structure
echo -e "${YELLOW}1. Vérification de la structure...${END}"
required_dirs=("tests_original" "tests_csv" "scripts" "docs" "temp_files")
for dir in "${required_dirs[@]}"; do
    if [[ -d "$SCRIPT_DIR/$dir" ]]; then
        echo -e "  ${GREEN}✅ $dir${END}"
    else
        echo -e "  ${RED}❌ $dir manquant${END}"
        exit 1
    fi
done

# Test 2: Vérifier les scripts principaux
echo -e "${YELLOW}2. Vérification des scripts...${END}"
required_scripts=("run_tests.sh" "tester")
for script in "${required_scripts[@]}"; do
    if [[ -f "$SCRIPT_DIR/$script" ]]; then
        echo -e "  ${GREEN}✅ $script${END}"
    else
        echo -e "  ${RED}❌ $script manquant${END}"
        exit 1
    fi
done

# Test 3: Vérifier les fichiers de test
echo -e "${YELLOW}3. Vérification des fichiers de test...${END}"
test_files=("tests_original/builtins" "tests_csv/csv_builtins")
for file in "${test_files[@]}"; do
    if [[ -f "$SCRIPT_DIR/$file" ]]; then
        count=$(wc -l < "$SCRIPT_DIR/$file" 2>/dev/null || echo "0")
        echo -e "  ${GREEN}✅ $(basename "$file") ($count tests)${END}"
    else
        echo -e "  ${RED}❌ $file manquant${END}"
        exit 1
    fi
done

# Test 4: Vérifier minishell
echo -e "${YELLOW}4. Vérification de minishell...${END}"
if [[ -f "$SCRIPT_DIR/../minishell" ]]; then
    echo -e "  ${GREEN}✅ minishell trouvé${END}"
else
    echo -e "  ${YELLOW}⚠️  minishell non trouvé (normal si pas encore compilé)${END}"
fi

# Test 5: Test du script d'aide
echo -e "${YELLOW}5. Test du script d'aide...${END}"
if bash "$SCRIPT_DIR/run_tests.sh" --help > /dev/null 2>&1; then
    echo -e "  ${GREEN}✅ Script d'aide fonctionne${END}"
else
    echo -e "  ${RED}❌ Erreur dans le script d'aide${END}"
    exit 1
fi

echo ""
echo -e "${GREEN}${BOLD}🎉 TOUS LES TESTS RAPIDES RÉUSSIS!${END}"
echo ""
echo -e "${BLUE}Le testeur est prêt à être utilisé.${END}"
echo -e "${BLUE}Utilisez '${BOLD}./run_tests.sh --help${END}${BLUE}' pour voir toutes les options.${END}"
