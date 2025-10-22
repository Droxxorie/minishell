#!/usr/bin/env python3
"""
Script pour extraire et organiser les tests du fichier CSV
en les triant par catégories pour le testeur minishell
"""

import csv
import re
import os

def clean_test_command(test_str):
    """Nettoie la commande de test en supprimant les guillemets externes si nécessaire"""
    if not test_str or test_str == "TEST":
        return None

    # Remplace les descriptions par les vraies commandes
    replacements = {
        "\\n (touche entrée)": "",
        "[que des espaces]": "   ",
        "[que des tabulations]": "\t\t",
        "Ctlr-C": "",  # Les signaux ne peuvent pas être testés facilement
        "Ctlr-D": "",
        "Ctlr-\\": "",
        "[touche du haut]": "",  # Histoire non testable automatiquement
    }

    for desc, cmd in replacements.items():
        if test_str == desc:
            return cmd if cmd else None

    # Supprime les commentaires entre parenthèses à la fin
    test_str = re.sub(r'\s*\([^)]*\)\s*$', '', test_str)

    return test_str.strip() if test_str.strip() else None

def categorize_test(test_cmd):
    """Catégorise un test selon son contenu"""
    if not test_cmd:
        return "misc"

    # Tests de syntaxe (erreurs)
    syntax_patterns = [
        r'^[|><&;]+$',  # Seulement des opérateurs
        r'\|$', r'^\|',  # Pipes mal placés
        r'>$', r'^>', r'>>$', r'^>>', r'<$', r'^<', r'<<$', r'^<<',  # Redirections mal placées
        r'&&', r'\|\|',  # Opérateurs logiques
        r';;', r'\(\)', r'[()]+',  # Parenthèses et point-virgules
    ]

    for pattern in syntax_patterns:
        if re.search(pattern, test_cmd):
            return "syntax"

    # Tests de builtins
    builtins = ['echo', 'cd', 'pwd', 'export', 'unset', 'env', 'exit']
    for builtin in builtins:
        if test_cmd.strip().startswith(builtin + ' ') or test_cmd.strip() == builtin:
            return "builtins"

    # Tests avec pipes
    if '|' in test_cmd and not re.search(r'[|><&;]$', test_cmd):
        return "pipes"

    # Tests avec redirections
    if any(op in test_cmd for op in ['>', '<', '>>', '<<']) and not re.search(r'[|><&;]$', test_cmd):
        return "redirects"

    # Tests avec variables d'environnement
    if '$' in test_cmd:
        return "variables"

    # Tests avec quotes
    if '"' in test_cmd or "'" in test_cmd:
        return "quotes"

    # Tests de chemins et commandes
    if test_cmd.startswith('/') or test_cmd.startswith('./'):
        return "paths"

    return "misc"

def extract_tests_from_csv(csv_file):
    """Extrait tous les tests du fichier CSV"""
    tests_by_category = {
        "syntax": [],
        "builtins": [],
        "pipes": [],
        "redirects": [],
        "variables": [],
        "quotes": [],
        "paths": [],
        "misc": [],
        "not_implemented": []  # Tests marqués comme "NON GERE"
    }

    with open(csv_file, 'r', encoding='utf-8') as f:
        reader = csv.reader(f)
        next(reader)  # Skip header

        for row in reader:
            if len(row) < 8:
                continue

            test_cmd = row[0]
            is_not_implemented = len(row) > 1 and "NON GERE" in row[1]

            clean_cmd = clean_test_command(test_cmd)
            if clean_cmd is None:
                continue

            if is_not_implemented:
                tests_by_category["not_implemented"].append(clean_cmd)
            else:
                category = categorize_test(clean_cmd)
                tests_by_category[category].append(clean_cmd)

    return tests_by_category

def write_test_files(tests_by_category, output_dir):
    """Écrit les fichiers de tests pour chaque catégorie"""
    os.makedirs(output_dir, exist_ok=True)

    for category, tests in tests_by_category.items():
        if not tests:
            continue

        # Supprime les doublons tout en préservant l'ordre
        unique_tests = []
        seen = set()
        for test in tests:
            if test not in seen:
                unique_tests.append(test)
                seen.add(test)

        output_file = os.path.join(output_dir, f"csv_{category}")
        with open(output_file, 'w', encoding='utf-8') as f:
            for test in unique_tests:
                f.write(test + '\n')

        print(f"Créé {output_file} avec {len(unique_tests)} tests")

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

    print("Extraction des tests du fichier CSV...")
    tests_by_category = extract_tests_from_csv(csv_file)

    print("\nRésumé des tests extraits:")
    total = 0
    for category, tests in tests_by_category.items():
        count = len(set(tests))  # Compte unique
        print(f"  {category}: {count} tests")
        total += count
    print(f"  Total: {total} tests")

    print("\nÉcriture des fichiers de tests...")
    write_test_files(tests_by_category, output_dir)

    print("\nTerminé! Les nouveaux fichiers de tests ont été créés.")
    print("Vous pouvez maintenant les exécuter avec:")
    for category in tests_by_category.keys():
        if tests_by_category[category]:
            print(f"  ./tester csv_{category}")

if __name__ == "__main__":
    main()
