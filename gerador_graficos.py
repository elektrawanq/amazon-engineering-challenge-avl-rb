import pandas as pd
import matplotlib.pyplot as plt
import os

# Configuração visual para os slides (estilo clean)
plt.style.use('seaborn-v0_8-darkgrid')
os.makedirs("graficos_slides", exist_ok=True)

try:
    df = pd.read_csv("metrics.csv")
except FileNotFoundError:
    print("Arquivo metrics.csv não encontrado. Rode o programa C++ primeiro.")
    exit(1)

# Função auxiliar para gerar gráficos de barras comparativos
def plot_comparacao(df_cenario, metrica, titulo, nome_arquivo):
    plt.figure(figsize=(8, 5))
    pivot = df_cenario.pivot(index='Tamanho', columns='Arvore', values=metrica)
    pivot.plot(kind='bar', ax=plt.gca(), color=['#1f77b4', '#d62728'], edgecolor='black')
    
    plt.title(titulo, fontsize=14, weight='bold')
    plt.xlabel("Quantidade de Elementos", fontsize=12)
    plt.ylabel(metrica, fontsize=12)
    plt.xticks(rotation=0)
    plt.legend(title="Estrutura")
    plt.tight_layout()
    plt.savefig(f"graficos_slides/{nome_arquivo}.png", dpi=300)
    plt.close()

# Gerando os gráficos principais
cenarios = df['Cenario'].unique()
for cenario in cenarios:
    df_c = df[df['Cenario'] == cenario]
    plot_comparacao(df_c, 'Rotacoes', f'Total de Rotações - {cenario}', f'rotacoes_{cenario}')
    plot_comparacao(df_c, 'Tempo_ms', f'Tempo de Execução (ms) - {cenario}', f'tempo_{cenario}')

print("✅ Gráficos gerados com sucesso na pasta 'graficos_slides/'!")
