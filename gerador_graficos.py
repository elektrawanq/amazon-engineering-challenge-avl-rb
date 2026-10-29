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

# Remove espaços extras nos nomes de colunas e valores de texto (evita "falsas" categorias)
df.columns = df.columns.str.strip()
for col in ['Cenario', 'Arvore']:
    df[col] = df[col].astype(str).str.strip()

# Função auxiliar para gerar gráficos de barras comparativos
def plot_comparacao(df_cenario, metrica, titulo, nome_arquivo, aggfunc='mean'):
    plt.figure(figsize=(8, 5))

    # pivot_table agrega linhas repetidas (mesmo Tamanho + Arvore),
    # o que o pivot() simples não consegue fazer
    pivot = df_cenario.pivot_table(
        index='Tamanho',
        columns='Arvore',
        values=metrica,
        aggfunc=aggfunc
    ).sort_index()

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

    # Aviso útil: mostra se há repetições sendo agregadas
    n_dup = df_c.duplicated(subset=['Tamanho', 'Arvore']).sum()
    if n_dup > 0:
        print(f"ℹ️  Cenário '{cenario}': {n_dup} linhas repetidas agregadas pela média.")

    # Rotações: contagem -> média (ou 'max'); Tempo: mediana é mais robusta a outliers
    plot_comparacao(df_c, 'Rotacoes', f'Total de Rotações - {cenario}', f'rotacoes_{cenario}', aggfunc='mean')
    plot_comparacao(df_c, 'Tempo_ms', f'Tempo de Execução (ms) - {cenario}', f'tempo_{cenario}', aggfunc='median')

print("✅ Gráficos gerados com sucesso na pasta 'graficos_slides/'!")