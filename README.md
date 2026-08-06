📁 Área de Projetos do Pedro

Repositório com testes, exercícios e projetos pessoais desenvolvidos em Python, C e outras linguagens, usados como espaço de prática e evolução.

📂 Estrutura
Projetos/
├── Projetos_Avançados/
│   ├── Gamificação/     # Jogos em desenvolvimento
│   └── IA/               # Assistente de voz local
└── Projetos_em_Python/   # Exercícios e projetos introdutórios em Python
🚀 Projetos Avançados
🤖 IA — Assistente de voz "Sexta-Feira"

Assistente virtual por voz que roda localmente, usando Ollama como motor de linguagem, captura de áudio via SpeechRecognition e resposta falada via edge-tts.

Como executar:

bash
cd Projetos_Avançados/IA
python3 -m venv venv
source venv/bin/activate
pip install -r requisitos.txt
python3 main.py

É necessário ter o Ollama instalado e rodando localmente.

Arquivos principais:

main.py — loop principal (captura voz → gera resposta → fala)
cerebro.py — lógica de conversação com o modelo de IA
voz.py / voz_saida.py — entrada e saída de áudio
🎮 Gamificação

Jogos em fase inicial de desenvolvimento:

Jogo_De_estratégia.c — jogo de estratégia em C
Jogo_da_forca.py — jogo da forca em Python
🐍 Projetos em Python

Exercícios voltados para prática e fixação de lógica de programação:

Projeto	Descrição
Calculadora_Nivel_Simples.py	Calculadora básica com as quatro operações
Calculador_Nivel_Intermediária.py	Calculadora com menu interativo, incluindo potenciação e raiz quadrada
Jogo_da_Forca.py	Jogo da forca com temas variados (frutas, filmes, animais, países, famosos)

Como executar qualquer projeto:

bash
cd Projetos_em_Python
python3 nome_do_arquivo.py
🛠️ Tecnologias
Python 3
C
Bibliotecas: ollama, SpeechRecognition, edge-tts, pyaudio
📌 Status

Repositório em constante atualização — novos projetos e melhorias são adicionados conforme o aprendizado avança.
