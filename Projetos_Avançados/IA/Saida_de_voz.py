import asyncio
import subprocess
import io
import edge_tts

# ──────────────────────────────────────────────────────────────
#  JARVIS — Voz masculina pt-BR via edge-tts
#  Voz: AntonioNeural — única voz masculina nativa pt-BR
#  disponível no edge-tts (Microsoft Neural TTS)
# ──────────────────────────────────────────────────────────────

VOZ = "pt-BR-AntonioNeural"

# Velocidade: -10% deixa a fala mais pausada e deliberada,
# característica marcante do JARVIS (ele nunca fala apressado)
VELOCIDADE = "-10%"

# Tom: -15Hz grava a voz, aproximando do timbre do JARVIS
# (AntonioNeural já é grave; isso intensifica o efeito)
TOM = "-15Hz"


async def _sintetizar_e_falar(texto: str):
    comunicar = edge_tts.Communicate(texto, VOZ, rate=VELOCIDADE, pitch=TOM)
    buffer = io.BytesIO()

    async for chunk in comunicar.stream():
        if chunk["type"] == "audio":
            buffer.write(chunk["data"])

    buffer.seek(0)
    audio = buffer.read()

    if not audio:
        return

    subprocess.Popen(
        ["ffplay", "-nodisp", "-autoexit", "-loglevel", "quiet", "-i", "pipe:0"],
        stdin=subprocess.PIPE,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    ).communicate(input=audio)


def falar(texto: str):
    """Faz o JARVIS falar o texto em pt-BR com tom grave e ritmo preciso."""
    if not texto or not texto.strip():
        return
    asyncio.run(_sintetizar_e_falar(texto))


# ──────────────────────────────────────────────────────────────
#  Teste rápido — rode: python voz_saida.py
# ──────────────────────────────────────────────────────────────
if __name__ == "__main__":
    falar("Sistemas online. Todos os módulos carregados. Pronto para receber ordens.")
