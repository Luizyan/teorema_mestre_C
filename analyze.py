"""Le resultados.csv, gera tabelas (dados) e graficos para o relatorio."""
import numpy as np, pandas as pd, matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

df = pd.read_csv("resultados.csv")
cfg = {
 "P1": dict(g=lambda n: n**3,            gl="n³",        exato=lambda n: 2*n**3-n**2,
            exl="2n³ − n²",       titulo="Problema 1 – T(n)=8T(n/2)+Θ(n²) → Θ(n³)"),
 "P2": dict(g=lambda n: n*np.log2(n),    gl="n log n",   exato=lambda n: n*np.log(n)/np.log(3),
            exl="n·log₃ n",       titulo="Problema 2 – T(n)=3T(n/3)+Θ(n) → Θ(n log n)"),
 "P3": dict(g=lambda n: n**2.0,          gl="n²",        exato=lambda n: n*(n-1)/2,
            exl="n(n−1)/2",       titulo="Problema 3 – T(n)=2T(n/2)+Θ(n²) → Θ(n²)"),
}
tabs = {}
for p, c in cfg.items():
    d = df[df.problema == p].sort_values("n").reset_index(drop=True)
    n = d.n.values.astype(float)
    g = c["g"](n)
    d["g"] = g
    d["ops_teor"] = c["exato"](n)
    k_ops = np.median((d.operacoes / g).values[-5:])
    k_t = np.median((d.tempo_ms / g).values[-5:])
    d["ops_ass"] = k_ops * g
    d["t_teor"] = k_t * g
    d["razao_ops"] = d.operacoes / g
    d["razao_t"] = d.tempo_ms / g
    d["k_ops"] = k_ops; d["k_t"] = k_t
    tabs[p] = d
    d.to_csv(f"tabela_{p}.csv", index=False)

    fig, ax = plt.subplots(1, 2, figsize=(9.5, 3.8))
    ax[0].loglog(n, d.operacoes, "o", label="experimental", ms=5)
    ax[0].loglog(n, d.ops_teor, "-", label=f"exato: {c['exl']}")
    ax[0].loglog(n, d.ops_ass, "--", label=f"{k_ops:.3g}·{c['gl']}")
    ax[0].set_title("Operações contadas"); ax[0].set_xlabel("n"); ax[0].set_ylabel("operações")
    ax[0].legend(fontsize=8); ax[0].grid(True, which="both", alpha=.3)
    ax[1].loglog(n, d.tempo_ms, "o", label="experimental", ms=5)
    ax[1].loglog(n, d.t_teor, "--", label=f"{k_t:.3g}·{c['gl']} ms")
    ax[1].set_title("Tempo de execução"); ax[1].set_xlabel("n"); ax[1].set_ylabel("tempo (ms)")
    ax[1].legend(fontsize=8); ax[1].grid(True, which="both", alpha=.3)
    fig.suptitle(c["titulo"], fontsize=10); fig.tight_layout()
    fig.savefig(f"fig_{p}_curvas.png", dpi=170); plt.close(fig)

    fig, ax = plt.subplots(figsize=(5.2, 3.3))
    ax.semilogx(n, d.razao_ops / d.razao_ops.iloc[-1], "o-", label="operações / g(n) (normalizado)")
    ax.semilogx(n, d.razao_t / d.razao_t.iloc[-1], "s-", label="tempo / g(n) (normalizado)")
    ax.set_yscale("log"); ax.axhline(1, color="gray", lw=.8, ls=":")
    ax.set_xlabel("n"); ax.set_ylabel(f"razão / razão em n máx.  (g = {c['gl']})")
    ax.legend(fontsize=8); ax.grid(True, which="both", alpha=.3)
    fig.tight_layout(); fig.savefig(f"fig_{p}_razao.png", dpi=170); plt.close(fig)

    print(p, "k_ops=%.4g k_t=%.4g" % (k_ops, k_t))
    print(d[["n","operacoes","ops_teor","tempo_ms","t_teor","razao_t"]].to_string())
