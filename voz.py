import os
from gtts import gTTS

def play_audio(text):
    try:
        tts = gTTS(text=text, lang="es")
        tts.save("/Users/cesarhernandez/Documents/PlatformIO/Projects/RAG-1/prueba2/temp.mp3")
        os.system("/opt/homebrew/bin/mpg123 /Users/cesarhernandez/Documents/PlatformIO/Projects/RAG-1/prueba2/temp.mp3")
        os.remove("/Users/cesarhernandez/Documents/PlatformIO/Projects/RAG-1/prueba2/temp.mp3")
    except Exception as e:
        print(f"Error reproduciendo el audio: {e}")
# Ejemplo de uso
#play_audio("¡Hola! Esto es una prueba de audio.")
