import sounddevice as sd
import speech_recognition as sr
import numpy as np
import io
import wave

r = sr.Recognizer()

def capturar_voz_ia():
    TAXA = 16000
    DURACAO = 5  # segundos ouvindo por vez

    print("[Ouvindo...]")
    gravacao = sd.rec(
        int(DURACAO * TAXA),
        samplerate=TAXA,
        channels=1,
        dtype="int16"
    )
    sd.wait()  # aguarda terminar

    # Converte para formato WAV em memória
    buffer = io.BytesIO()
    with wave.open(buffer, 'wb') as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)  # int16 = 2 bytes
        wf.setframerate(TAXA)
        wf.writeframes(gravacao.tobytes())
    buffer.seek(0)

    # Envia pro Google Speech Recognition
    with sr.AudioFile(buffer) as source:
        audio = r.record(source)
    try:
        return r.recognize_google(audio, language="pt-BR")
    except sr.UnknownValueError:
        return None
    except sr.RequestError:
        print("[Erro] Sem conexão com Google Speech.")
        return None
