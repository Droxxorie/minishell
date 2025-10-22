#!/bin/bash

# Script de débogage pour comprendre pourquoi les tests échouent

echo "🔍 Débogage du testeur..."

# Créer un minishell factice
cat > fake_minishell << 'EOF'
#!/bin/bash
exec bash "$@"
EOF

chmod +x fake_minishell

# Test simple
echo "Test 1: echo hello"
echo "Commande: echo hello"
echo "Bash direct:"
echo "echo hello" | bash
echo "Bash via fake_minishell:"
echo "echo hello" | ./fake_minishell

echo ""
echo "Test 2: pwd"
echo "Commande: pwd"
echo "Bash direct:"
echo "pwd" | bash
echo "Bash via fake_minishell:"
echo "pwd" | ./fake_minishell

echo ""
echo "Test 3: exit"
echo "Commande: exit"
echo "Bash direct exit code:"
echo "exit" | bash; echo "Exit code: $?"
echo "Bash via fake_minishell exit code:"
echo "exit" | ./fake_minishell; echo "Exit code: $?"

echo ""
echo "🔍 Test de la logique du testeur..."

# Simuler la logique du testeur
test_cmd="echo hello"

echo "Test de la commande: $test_cmd"

# Test bash direct
BASH_OUTPUT=$(echo -e "$test_cmd" | bash 2> /dev/null)
BASH_EXIT_CODE=$?
echo "Bash output: '$BASH_OUTPUT'"
echo "Bash exit code: $BASH_EXIT_CODE"

# Test via fake_minishell
MINI_OUTPUT=$(echo -e "$test_cmd" | ./fake_minishell 2> /dev/null)
MINI_EXIT_CODE=$?
echo "Fake minishell output: '$MINI_OUTPUT'"
echo "Fake minishell exit code: $MINI_EXIT_CODE"

# Comparaison
if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" && "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" ]]; then
    echo "✅ Comparaison réussie"
else
    echo "❌ Comparaison échouée"
    echo "Outputs égaux: $([[ "$MINI_OUTPUT" == "$BASH_OUTPUT" ]] && echo "Oui" || echo "Non")"
    echo "Exit codes égaux: $([[ "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" ]] && echo "Oui" || echo "Non")"
fi

# Nettoyer
rm -f fake_minishell

echo "✅ Débogage terminé!"
