#!/bin/bash

# Script de nettoyage pour le testeur minishell
# Supprime tous les fichiers temporaires et remet le testeur dans un état propre

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PARENT_DIR="$(dirname "$SCRIPT_DIR")"
TEMP_DIR="$PARENT_DIR/temp_files"

# Couleurs
RED="\e[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;36m"
BOLD="\e[1m"
END="\033[0m"

echo -e "${BLUE}${BOLD}=== NETTOYAGE DU TESTEUR MINISHELL ===${END}"
echo ""

# Nettoyer les fichiers temporaires
echo -e "${YELLOW}Nettoyage des fichiers temporaires...${END}"
rm -rf "$TEMP_DIR"/* 2>/dev/null
rm -rf "$PARENT_DIR"/outfiles 2>/dev/null
rm -rf "$PARENT_DIR"/mini_outfiles 2>/dev/null
rm -rf "$PARENT_DIR"/bash_outfiles 2>/dev/null
rm -rf "$PARENT_DIR"/minishell_heredoc_* 2>/dev/null

# Nettoyer les fichiers de test temporaires
rm -rf "$PARENT_DIR"/*.out 2>/dev/null
rm -rf "$PARENT_DIR"/*.tmp 2>/dev/null

echo -e "${GREEN}✓ Fichiers temporaires supprimés${END}"

# Recréer les répertoires nécessaires
mkdir -p "$TEMP_DIR"
mkdir -p "$PARENT_DIR"/outfiles
mkdir -p "$PARENT_DIR"/mini_outfiles
mkdir -p "$PARENT_DIR"/bash_outfiles

echo -e "${GREEN}✓ Répertoires temporaires recréés${END}"

# Vérifier les permissions
if [[ -f "$PARENT_DIR/test_files/invalid_permission" ]]; then
    chmod 666 "$PARENT_DIR/test_files/invalid_permission" 2>/dev/null
    echo -e "${GREEN}✓ Permissions restaurées${END}"
fi

echo ""
echo -e "${GREEN}${BOLD}🎉 Nettoyage terminé avec succès!${END}"
echo -e "${BLUE}Le testeur est maintenant dans un état propre.${END}"
