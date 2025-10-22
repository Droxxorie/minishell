# 🐚 Minishell Tester - Guide Complet pour l'Équipe

Un testeur complet et organisé pour le projet minishell de 42, avec plus de 1,373 tests automatisés.

## 📁 Structure du Projet

```
droxMinishell/                    # Votre projet minishell
├── minishell                     # Votre exécutable compilé
├── Makefile                      # Votre Makefile
├── sources/                      # Vos fichiers sources
└── minishell_tester/             # Le testeur (copié ici)
    ├── run_tests.sh              # Script principal amélioré
    ├── tester                    # Script original (compatible)
    ├── simple_tester.sh          # Testeur simplifié pour calibration
    ├── test_my_minishell.sh      # Testeur adapté pour votre minishell
    ├── quick_test.sh             # Test rapide d'installation
    ├── tests_original/           # Tests originaux (193 tests)
    │   ├── builtins              # 52 tests
    │   ├── pipes                 # 4 tests
    │   ├── redirects             # 75 tests
    │   ├── extras                # 15 tests
    │   ├── syntax                # 17 tests
    │   ├── os_specific           # 11 tests
    │   └── wildcards             # 17 tests
    ├── tests_csv/               # Tests CSV (1,180+ tests)
    │   ├── csv_builtins          # 730 tests
    │   ├── csv_pipes             # 42 tests
    │   ├── csv_redirects         # 32 tests
    │   ├── csv_syntax            # 74 tests
    │   ├── csv_variables         # 29 tests
    │   ├── csv_quotes            # 11 tests
    │   ├── csv_paths             # 11 tests
    │   ├── csv_misc              # 32 tests
    │   └── csv_*_errors          # Tests d'erreur spécialisés
    ├── scripts/                  # Scripts utilitaires
    ├── docs/                     # Documentation
    └── temp_files/               # Fichiers temporaires
```

## 🚀 Installation

### **Étape 1 : Copier le Testeur**

```bash
# Dans votre projet minishell
cp -r /path/to/minishell_tester .
```

### **Étape 2 : Compiler Votre Minishell**

```bash
# Dans le dossier principal de votre projet
make
```

### **Étape 3 : Vérifier l'Installation**

```bash
# Aller dans le testeur
cd minishell_tester

# Test rapide d'installation
./quick_test.sh
```

**Résultat attendu :**
```
🎉 TOUS LES TESTS RAPIDES RÉUSSIS!
Le testeur est prêt à être utilisé.
```

## 🎯 Utilisation

### **Commandes de Base**

```bash
# Afficher l'aide complète
./run_tests.sh --help

# Lister toutes les catégories disponibles
./run_tests.sh --list

# Tests par défaut (recommandé pour commencer)
./run_tests.sh

# Nettoyer les fichiers temporaires
./run_tests.sh --clean
```

### **Tests Spécifiques**

```bash
# Test d'une catégorie spécifique
./run_tests.sh builtins      # Tests des builtins
./run_tests.sh pipes         # Tests des pipes
./run_tests.sh redirects     # Tests des redirections
./run_tests.sh syntax        # Tests de la syntaxe

# Tests CSV (plus complets)
./run_tests.sh csv_builtins  # 730 tests de builtins
./run_tests.sh csv_pipes     # 42 tests de pipes
./run_tests.sh csv_redirects # 32 tests de redirections
./run_tests.sh csv_syntax    # 74 tests de syntaxe
```

### **Mode Débogage**

```bash
# Mode verbeux pour voir les détails des échecs
./run_tests.sh --verbose builtins
./run_tests.sh --verbose csv_builtins

# Tests d'erreur seulement
./run_tests.sh --errors
```

### **Tests Complets**

```bash
# Tous les tests originaux
./run_tests.sh --original

# Tous les tests CSV
./run_tests.sh --csv

# Tous les tests (originaux + CSV)
./run_tests.sh --all
```

## 🔧 Dépannage

### **Problème : "minishell: not interactive"**

Si vous obtenez ce message, votre minishell a besoin d'être modifié pour gérer le mode non-interactif.

**Solution :**
```bash
# Test manuel pour diagnostiquer
./test_my_minishell.sh

# Ou test simple
echo "echo hello" | ../minishell
```

**Modifications nécessaires dans votre code :**
1. Gérer le mode non-interactif
2. Permettre l'exécution sans terminal
3. Gérer les signaux correctement

### **Problème : "minishell non trouvé"**

```bash
# Vérifier la compilation
cd ..
make
ls -la minishell
cd minishell_tester
```

### **Problème : Fichiers temporaires qui s'accumulent**

```bash
# Nettoyage automatique
./run_tests.sh --clean

# Nettoyage manuel
rm -rf temp_files/* outfiles mini_outfiles bash_outfiles
```

## 📊 Interprétation des Résultats

### **Codes de Couleur**
- 🟢 **90%+** : Excellent (votre minishell fonctionne très bien)
- 🟡 **70-89%** : Bon (quelques ajustements à faire)
- 🔴 **<70%** : À améliorer (travail important à faire)

### **Symboles**
- ✅ : Test réussi
- ❌ : Test échoué
- ⚠️ : Test réussi mais message d'erreur différent

### **Exemples de Sortie**

**Excellent (90%+) :**
```
=== Tests: builtins ===
✅   1✅   2✅   3✅   4✅   5✅   6✅   7✅   8✅   9✅  10
...
builtins: 47/52 (90%)
🎊 🎊 🎊 TOUS LES TESTS RÉUSSIS! 🎉 🎉 🎉
```

**Bon (70-89%) :**
```
=== Tests: builtins ===
✅   1❌   2✅   3❌   4✅   5✅   6❌   7✅   8✅   9✅  10
...
builtins: 40/52 (77%)
👍 Bon travail ! Quelques ajustements à faire
```

**À Améliorer (<70%) :**
```
=== Tests: builtins ===
❌   1❌   2❌   3❌   4❌   5❌   6❌   7❌   8❌   9❌  10
...
builtins: 30/52 (58%)
⚠️ Continuez le développement, vous êtes sur la bonne voie
```

## 🎯 Workflow Recommandé

### **Pour Commencer :**

```bash
# 1. Vérification
./quick_test.sh

# 2. Test de base
./run_tests.sh builtins

# 3. Test complet
./run_tests.sh

# 4. Si échecs, débogage
./run_tests.sh --verbose csv_builtins

# 5. Nettoyage
./run_tests.sh --clean
```

### **Pour le Développement :**

```bash
# Test rapide pendant le développement
./run_tests.sh builtins

# Test complet avant commit
./run_tests.sh --all

# Test des erreurs
./run_tests.sh --errors
```

## 🔍 Tests Manuels

Pour les cas non couverts par les tests automatisés :

```bash
# Test manuel simple
echo "echo hello" | ../minishell

# Test avec pwd
echo "pwd" | ../minishell

# Test avec exit
echo "exit" | ../minishell
```

## 📈 Statistiques

### **Tests Disponibles**
- **Tests originaux** : 193 tests
- **Tests CSV** : 1,180+ tests
- **Total** : 1,373+ tests automatisés

### **Catégories Organisées**
- **Tests originaux** : 7 catégories
- **Tests CSV** : 14 catégories
- **Tests d'erreur** : 5 catégories spécialisées

## 🛠️ Scripts Utilitaires

### **Test Rapide d'Installation**
```bash
./quick_test.sh
```

### **Test de Calibration (bash vs bash)**
```bash
./simple_tester.sh
```

### **Test Adapté pour Votre Minishell**
```bash
./test_my_minishell.sh
```

### **Script Original (Compatible)**
```bash
./tester
```

## 🎉 Avantages

- ✅ **Organisé** : Structure claire et logique
- ✅ **Complet** : 1,373+ tests automatisés
- ✅ **Moderne** : Interface utilisateur avancée
- ✅ **Robuste** : Gestion d'erreurs et nettoyage automatique
- ✅ **Documenté** : Guide complet et exemples
- ✅ **Compatible** : Script original inchangé

## 🤝 Contribution

Pour améliorer le testeur :
1. Ajoutez de nouveaux tests dans les fichiers appropriés
2. Testez vos modifications avec `./run_tests.sh --all`
3. Nettoyez avec `./run_tests.sh --clean`

## 📝 Notes Importantes

- Les tests utilisent `/dev/null` pour supprimer les sorties d'erreur lors de la comparaison
- Les couleurs ANSI sont automatiquement supprimées des sorties
- Les fichiers temporaires sont nettoyés automatiquement
- Les permissions sont gérées automatiquement pour les tests appropriés
- Le testeur est maintenant portable et ne contient plus de chemins hardcodés

---

**Bon développement avec minishell ! 🐚✨**

*Créé par l'équipe pour l'équipe - Testeur minishell v2.0*
