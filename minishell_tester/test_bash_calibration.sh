#!/bin/bash

# Script pour calibrer le testeur avec bash
# Ce script simule un minishell en utilisant bash pour vérifier que le testeur fonctionne

echo "🔧 Calibrage du testeur avec bash..."

# Créer un minishell factice qui utilise bash
cat > fake_minishell << 'EOF'
#!/bin/bash
# Minishell factice qui utilise bash
exec bash "$@"
EOF

chmod +x fake_minishell

# Tester avec le minishell factice
echo "📋 Test avec minishell factice (bash)..."
# Modifier temporairement le script pour utiliser notre minishell factice
sed 's|MINISHELL_PATH="../minishell"|MINISHELL_PATH="./fake_minishell"|g' run_tests.sh > run_tests_fake.sh
chmod +x run_tests_fake.sh
./run_tests_fake.sh builtins
rm -f run_tests_fake.sh

# Nettoyer
rm -f fake_minishell

echo "✅ Calibrage terminé!"
