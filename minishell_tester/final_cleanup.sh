#!/bin/bash

# Nettoyage final du testeur minishell
# Supprime tous les fichiers temporaires et de test qui traînent

echo "🧹 Nettoyage final du testeur minishell..."

# Fichiers à supprimer ou déplacer
temp_files=(
    "a" "b" "c" "d" "e"
    "hey" "HOLA" "hola" "hola1" "hola2" "ls1" "pwd" "bonjour"
    "bash.supp" "local.supp" "loop.out"
    "bonus" "bonus_bonus"
    "dir" "dir*"
    "*.out" "*.supp"
)

# Déplacer vers temp_files
for file in "${temp_files[@]}"; do
    if [[ -e "$file" ]]; then
        mv "$file" temp_files/ 2>/dev/null && echo "✅ Déplacé: $file"
    fi
done

# Supprimer les fichiers avec des caractères spéciaux
find . -maxdepth 1 -name "*\$*" -exec mv {} temp_files/ \; 2>/dev/null
find . -maxdepth 1 -name "*>*" -exec mv {} temp_files/ \; 2>/dev/null
find . -maxdepth 1 -name "*<*" -exec mv {} temp_files/ \; 2>/dev/null

echo "🎉 Nettoyage final terminé!"
