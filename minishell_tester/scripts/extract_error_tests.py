#!/usr/bin/env python3
"""
Script pour analyser les codes d'erreur et les comportements attendus
du fichier CSV et créer des tests spécialisés
"""

import csv
import re
import os

def extract_error_info_from_csv(csv_file):
    """Extrait les informations sur les codes d'erreur et comportements attendus"""
    error_tests = []
    exit_code_tests = []

    with open(csv_file, 'r', encoding='utf-8') as f:
        reader = csv.reader(f)
        next(reader)  # Skip header

        for row_num, row in enumerate(reader, 2):
            if len(row) < 8:
                continue

            test_cmd = row[0]
            exit_status = row[5] if len(row) > 5 else ""
            expected_behavior = row[7] if len(row) > 7 else ""

            # Nettoie la commande
            if not test_cmd or test_cmd == "TEST":
                continue

            # Ignore les descriptions non-commandes
            if any(desc in test_cmd for desc in ["[que des", "\\n (", "Ctlr-", "[touche"]):
                continue

            # Extrait les informations sur les codes d'erreur
            if expected_behavior:
                # Recherche les patterns d'erreur bash
                error_patterns = [
                    r"bash: ([^:]+): (.+)",
                    r"syntax error near unexpected token",
                    r"command not found",
                    r"No such file or directory",
                    r"Permission denied",
                    r"Is a directory",
                    r"not a valid identifier",
                ]

                for pattern in error_patterns:
                    if re.search(pattern, expected_behavior):
                        error_tests.append({
                            'command': test_cmd.strip(),
                            'expected_output': expected_behavior,
                            'row': row_num
                        })
                        break

            # Tests spécifiques pour les codes de sortie
            if exit_status and exit_status.strip():
                exit_code_tests.append({
                    'command': test_cmd.strip(),
                    'exit_status': exit_status.strip(),
                    'row': row_num
                })

    return error_tests, exit_code_tests

def create_error_test_files(error_tests, exit_code_tests, output_dir):
    """Crée des fichiers de tests spécialisés pour les erreurs"""

    # Tests d'erreurs de syntaxe
    syntax_errors = [t for t in error_tests if 'syntax error' in t['expected_output']]
    if syntax_errors:
        with open(os.path.join(output_dir, 'csv_syntax_errors'), 'w') as f:
            for test in syntax_errors:
                f.write(test['command'] + '\n')
        print(f"Créé csv_syntax_errors avec {len(syntax_errors)} tests")

    # Tests de commandes introuvables
    command_not_found = [t for t in error_tests if 'command not found' in t['expected_output']]
    if command_not_found:
        with open(os.path.join(output_dir, 'csv_command_errors'), 'w') as f:
            for test in command_not_found:
                f.write(test['command'] + '\n')
        print(f"Créé csv_command_errors avec {len(command_not_found)} tests")

    # Tests de permissions
    permission_errors = [t for t in error_tests if 'Permission denied' in t['expected_output']]
    if permission_errors:
        with open(os.path.join(output_dir, 'csv_permission_errors'), 'w') as f:
            for test in permission_errors:
                f.write(test['command'] + '\n')
        print(f"Créé csv_permission_errors avec {len(permission_errors)} tests")

    # Tests de répertoires
    directory_errors = [t for t in error_tests if 'Is a directory' in t['expected_output']]
    if directory_errors:
        with open(os.path.join(output_dir, 'csv_directory_errors'), 'w') as f:
            for test in directory_errors:
                f.write(test['command'] + '\n')
        print(f"Créé csv_directory_errors avec {len(directory_errors)} tests")

    # Tests d'identifiants invalides (export/unset)
    invalid_id_errors = [t for t in error_tests if 'not a valid identifier' in t['expected_output']]
    if invalid_id_errors:
        with open(os.path.join(output_dir, 'csv_invalid_identifier'), 'w') as f:
            for test in invalid_id_errors:
                f.write(test['command'] + '\n')
        print(f"Créé csv_invalid_identifier avec {len(invalid_id_errors)} tests")

def create_exit_code_tester(exit_code_tests, output_dir):
    """Crée un testeur spécialisé pour les codes de sortie"""
    script_content = '''#!/bin/bash

# Script pour tester les codes de sortie spécifiques
MINISHELL_PATH="../minishell"

BOLD="\\e[1m"
YELLOW="\\033[0;33m"
GREY="\\033[38;5;244m"
PURPLE="\\033[0;35m"
RED="\\e[0;31m"
GREEN="\\033[0;32m"
END="\\033[0m"

echo "Testing exit codes..."

total=0
ok=0

# Tests des codes de sortie pour exit
exit_tests=(
    "exit 0:0"
    "exit 1:1"
    "exit 42:42"
    "exit 255:255"
    "exit 256:0"
    "exit -1:255"
    "exit -42:214"
)

for test_case in "${exit_tests[@]}"; do
    cmd=$(echo "$test_case" | cut -d: -f1)
    expected=$(echo "$test_case" | cut -d: -f2)

    ((total++))

    # Test avec minishell
    echo -e "$cmd" | $MINISHELL_PATH &>/dev/null
    mini_exit=$?

    # Test avec bash
    echo -e "$cmd" | bash &>/dev/null
    bash_exit=$?

    printf "$YELLOW"
    printf "Test %3s: " $total

    if [[ "$mini_exit" == "$bash_exit" ]]; then
        printf "✅"
        ((ok++))
    else
        printf "❌"
    fi

    printf "$GREY $cmd (expected: $expected, mini: $mini_exit, bash: $bash_exit)\\n$END"
done

printf "$PURPLE"
printf "$BOLD"
echo "Exit code tests: $ok/$total"
printf "$END"
'''

    with open(os.path.join(output_dir, 'test_exit_codes'), 'w') as f:
        f.write(script_content)

    os.chmod(os.path.join(output_dir, 'test_exit_codes'), 0o755)
    print("Créé test_exit_codes script")

def main():
    # Chemins portables
    script_dir = os.path.dirname(os.path.abspath(__file__))
    parent_dir = os.path.dirname(script_dir)
    
    # Chercher le fichier CSV dans le répertoire courant ou parent
    csv_file = None
    possible_paths = [
        "TEST MINshellRT - Feuille 1.csv",
        "../TEST MINshellRT - Feuille 1.csv",
        os.path.join(parent_dir, "TEST MINshellRT - Feuille 1.csv"),
        os.path.join(script_dir, "TEST MINshellRT - Feuille 1.csv")
    ]
    
    for path in possible_paths:
        if os.path.exists(path):
            csv_file = path
            break
    
    output_dir = parent_dir

    if not os.path.exists(csv_file):
        print(f"Erreur: fichier CSV non trouvé: {csv_file}")
        return

    print("Analyse des codes d'erreur et comportements attendus...")
    error_tests, exit_code_tests = extract_error_info_from_csv(csv_file)

    print(f"Trouvé {len(error_tests)} tests d'erreur")
    print(f"Trouvé {len(exit_code_tests)} tests de codes de sortie")

    print("\\nCréation des fichiers de tests d'erreur...")
    create_error_test_files(error_tests, exit_code_tests, output_dir)

    print("\\nCréation du testeur de codes de sortie...")
    create_exit_code_tester(exit_code_tests, output_dir)

    print("\\nTerminé! Tests d'erreur créés.")

if __name__ == "__main__":
    main()
