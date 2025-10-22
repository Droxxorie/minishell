# Tests CSV pour Minishell

Ce dossier contient plus de 720 tests extraits automatiquement d'un fichier CSV de tests pour minishell.

## Structure des tests

### Tests principaux (par catégorie)
- `csv_syntax` (54 tests) - Tests d'erreurs de syntaxe
- `csv_builtins` (429 tests) - Tests des commandes intégrées (echo, cd, pwd, export, unset, env, exit)
- `csv_pipes` (34 tests) - Tests des pipes
- `csv_redirects` (21 tests) - Tests des redirections
- `csv_variables` (14 tests) - Tests des variables d'environnement
- `csv_quotes` (9 tests) - Tests des guillemets et apostrophes
- `csv_paths` (11 tests) - Tests des chemins et exécution de fichiers
- `csv_misc` (22 tests) - Tests divers

### Tests d'erreur spécialisés
- `csv_syntax_errors` (23 tests) - Erreurs de syntaxe spécifiques
- `csv_command_errors` (49 tests) - Commandes non trouvées
- `csv_permission_errors` (4 tests) - Erreurs de permission
- `csv_directory_errors` (7 tests) - Erreurs de répertoire
- `csv_invalid_identifier` (56 tests) - Identifiants invalides (export/unset)

### Tests non implémentés
- `csv_not_implemented` (126 tests) - Tests marqués comme "NON GERE" dans le CSV

## Scripts disponibles

### 1. Script principal : `run_all_csv_tests.sh`
```bash
# Exécuter tous les tests
./run_all_csv_tests.sh

# Exécuter une catégorie spécifique
./run_all_csv_tests.sh builtins

# Exécuter seulement les tests d'erreur
./run_all_csv_tests.sh --errors

# Lister toutes les catégories
./run_all_csv_tests.sh --list

# Afficher l'aide
./run_all_csv_tests.sh --help
```

### 2. Script CSV spécialisé : `test_csv`
```bash
# Exécuter tous les tests CSV
./test_csv

# Exécuter une catégorie spécifique
./test_csv syntax
./test_csv builtins
./test_csv pipes
```

### 3. Test des codes de sortie : `test_exit_codes`
```bash
# Tester spécifiquement les codes de sortie
./test_exit_codes
```

### 4. Script original modifié : `tester`
Le script `tester` original a été modifié pour inclure automatiquement tous les tests CSV lors de l'exécution complète :
```bash
# Exécution complète (inclut maintenant les tests CSV)
./tester

# Exécution d'une catégorie spécifique
./tester csv_builtins
```

## Scripts d'extraction

### `extract_csv_tests.py`
Script Python qui extrait et catégorise automatiquement tous les tests du fichier CSV.

### `extract_error_tests.py`
Script Python qui analyse les codes d'erreur et comportements attendus pour créer des tests d'erreur spécialisés.

## Comment utiliser

1. **Test rapide d'une catégorie :**
   ```bash
   ./run_all_csv_tests.sh builtins
   ```

2. **Test complet :**
   ```bash
   ./run_all_csv_tests.sh
   ```

3. **Focus sur les erreurs :**
   ```bash
   ./run_all_csv_tests.sh --errors
   ```

4. **Integration avec le testeur existant :**
   ```bash
   ./tester  # Inclut maintenant automatiquement les tests CSV
   ```

## Résultats attendus

Les tests comparent :
- **Sortie standard** : doit être identique à bash
- **Code de sortie** : doit être identique à bash
- **Fichiers de sortie** : pour les tests avec redirections
- **Messages d'erreur** : comparaison des messages (avec avertissement si différent)

## Notes importantes

- Les tests utilisent `/dev/null` pour supprimer les sorties d'erreur lors de la comparaison
- Les couleurs ANSI sont automatiquement supprimées des sorties
- Les fichiers temporaires sont nettoyés automatiquement
- Les permissions sont gérées automatiquement pour les tests appropriés

## Statistiques

**Total des tests extraits :** 720 tests
- Tests implémentés : 594 tests
- Tests non implémentés : 126 tests
- Tests d'erreur spécialisés : 139 tests supplémentaires

Ces tests couvrent exhaustivement la plupart des cas d'usage et cas limites de minishell selon les spécifications du projet 42.
