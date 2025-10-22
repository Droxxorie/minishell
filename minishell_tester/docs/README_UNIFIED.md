# 🐚 Minishell Tester - Guide Complet

Un testeur complet et organisé pour le projet minishell de 42, avec plus de 720 tests automatisés.

## 📁 Structure du Projet

```
minishell_tester/
├── run_tests.sh          # Script principal amélioré
├── tester                # Script original (compatible)
├── scripts/              # Scripts utilitaires
│   ├── cleanup.sh        # Nettoyage automatique
│   ├── extract_csv_tests.py
│   ├── extract_error_tests.py
│   └── run_all_csv_tests.sh
├── tests_original/       # Tests originaux du projet
│   ├── builtins
│   ├── pipes
│   ├── redirects
│   ├── extras
│   ├── syntax
│   ├── os_specific
│   └── wildcards
├── tests_csv/           # Tests extraits du CSV (720+ tests)
│   ├── csv_syntax
│   ├── csv_builtins
│   ├── csv_pipes
│   ├── csv_redirects
│   ├── csv_variables
│   ├── csv_quotes
│   ├── csv_paths
│   ├── csv_misc
│   └── csv_*_errors
├── docs/                # Documentation
│   ├── README_UNIFIED.md
│   ├── README.md
│   └── README_CSV.md
├── temp_files/          # Fichiers temporaires
└── test_files/          # Fichiers de test
```

## 🚀 Installation

1. **Cloner le testeur dans votre dossier minishell :**
   ```bash
   cd minishell
   git clone <url-du-repo> minishell_tester
   cd minishell_tester
   ```

2. **Compiler votre minishell :**
   ```bash
   cd ..
   make
   cd minishell_tester
   ```

3. **Rendre les scripts exécutables :**
   ```bash
   chmod +x run_tests.sh scripts/*.sh
   ```

## 🎯 Utilisation

### Script Principal Amélioré (`run_tests.sh`)

Le nouveau script principal offre une interface moderne et flexible :

```bash
# Afficher l'aide
./run_tests.sh --help

# Tests par défaut (recommandé)
./run_tests.sh

# Tous les tests
./run_tests.sh --all

# Tests CSV seulement
./run_tests.sh --csv

# Tests originaux seulement
./run_tests.sh --original

# Tests d'erreur seulement
./run_tests.sh --errors

# Test d'une catégorie spécifique
./run_tests.sh builtins
./run_tests.sh csv_builtins

# Mode verbeux
./run_tests.sh --verbose builtins

# Lister toutes les catégories
./run_tests.sh --list

# Nettoyer les fichiers temporaires
./run_tests.sh --clean
```

### Script Original (`tester`)

Le script original reste compatible :

```bash
# Tous les tests
./tester

# Test d'une catégorie
./tester builtins
./tester csv_builtins
```

## 📊 Catégories de Tests

### Tests Originaux
- **builtins** (429 tests) - Commandes intégrées (echo, cd, pwd, export, unset, env, exit)
- **pipes** (34 tests) - Gestion des pipes
- **redirects** (21 tests) - Redirections (>, <, >>, <<)
- **extras** - Tests supplémentaires
- **syntax** - Erreurs de syntaxe
- **os_specific** - Tests spécifiques au système
- **wildcards** - Tests des wildcards (bonus)

### Tests CSV (720+ tests)
- **csv_syntax** (54 tests) - Erreurs de syntaxe
- **csv_builtins** (429 tests) - Commandes intégrées
- **csv_pipes** (34 tests) - Pipes
- **csv_redirects** (21 tests) - Redirections
- **csv_variables** (14 tests) - Variables d'environnement
- **csv_quotes** (9 tests) - Guillemets et apostrophes
- **csv_paths** (11 tests) - Chemins et exécution
- **csv_misc** (22 tests) - Tests divers

### Tests d'Erreur Spécialisés
- **csv_syntax_errors** (23 tests) - Erreurs de syntaxe spécifiques
- **csv_command_errors** (49 tests) - Commandes non trouvées
- **csv_permission_errors** (4 tests) - Erreurs de permission
- **csv_directory_errors** (7 tests) - Erreurs de répertoire
- **csv_invalid_identifier** (56 tests) - Identifiants invalides

## 🔧 Scripts Utilitaires

### Nettoyage (`scripts/cleanup.sh`)
```bash
./scripts/cleanup.sh
```
Supprime tous les fichiers temporaires et remet le testeur dans un état propre.

### Extraction CSV (`scripts/extract_csv_tests.py`)
```bash
python3 scripts/extract_csv_tests.py
```
Extrait et organise les tests d'un fichier CSV.

### Tests d'Erreur (`scripts/extract_error_tests.py`)
```bash
python3 scripts/extract_error_tests.py
```
Crée des tests spécialisés pour les codes d'erreur.

## 📈 Interprétation des Résultats

### Codes de Couleur
- 🟢 **Vert** : 90%+ de réussite (Excellent)
- 🟡 **Jaune** : 70-89% de réussite (Bon)
- 🔴 **Rouge** : <70% de réussite (À améliorer)

### Symboles
- ✅ : Test réussi
- ❌ : Test échoué
- ⚠️ : Test réussi mais message d'erreur différent

### Comparaisons Effectuées
- **Sortie standard** : Doit être identique à bash
- **Code de sortie** : Doit être identique à bash
- **Fichiers de sortie** : Pour les tests avec redirections
- **Messages d'erreur** : Comparaison avec avertissement si différent

## 🛠️ Dépannage

### Problèmes Courants

1. **"minishell non trouvé"**
   ```bash
   cd ..
   make
   cd minishell_tester
   ```

2. **Fichiers temporaires qui s'accumulent**
   ```bash
   ./run_tests.sh --clean
   # ou
   ./scripts/cleanup.sh
   ```

3. **Permissions refusées**
   ```bash
   chmod +x run_tests.sh scripts/*.sh
   ```

4. **Tests qui échouent de manière inattendue**
   - Vérifiez que votre minishell est compilé
   - Utilisez `--verbose` pour plus de détails
   - Vérifiez les messages d'erreur spécifiques

### Tests Manuels

Pour les cas non couverts par les tests automatisés, consultez le dossier `manual_tests/` :
- `mandatory/` - Tests manuels obligatoires
- `heredoc/` - Tests heredoc
- `signals/` - Tests de signaux
- `not_mandatory/` - Tests non obligatoires

## 🎯 Conseils d'Utilisation

1. **Commencez par les tests par défaut :**
   ```bash
   ./run_tests.sh
   ```

2. **Focus sur les catégories qui échouent :**
   ```bash
   ./run_tests.sh --verbose csv_builtins
   ```

3. **Testez régulièrement pendant le développement :**
   ```bash
   ./run_tests.sh builtins  # Test rapide des builtins
   ```

4. **Nettoyez régulièrement :**
   ```bash
   ./run_tests.sh --clean
   ```

## 📝 Notes Importantes

- Les tests utilisent `/dev/null` pour supprimer les sorties d'erreur lors de la comparaison
- Les couleurs ANSI sont automatiquement supprimées des sorties
- Les fichiers temporaires sont nettoyés automatiquement
- Les permissions sont gérées automatiquement pour les tests appropriés
- Le testeur est maintenant portable et ne contient plus de chemins hardcodés

## 🤝 Contribution

Pour améliorer le testeur :
1. Ajoutez de nouveaux tests dans les fichiers appropriés
2. Utilisez `extract_csv_tests.py` pour organiser les tests CSV
3. Testez vos modifications avec `./run_tests.sh --all`
4. Nettoyez avec `./run_tests.sh --clean`

---

**Bon développement avec minishell ! 🐚✨**
