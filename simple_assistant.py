UWAGA:Ten e-mail pochodzi spoza Twojej organizacji. Nie klikaj w linki oraz nie otwieraj załączników, chyba że rozpoznajesz nadawcę i wiesz ze zwartość jest bezpieczna.
CAUTION:This e-mail originated outside our organisation. Do not click links or open attachments unless you recognise the sender and know the content is safe.
import speech_recognition as sr
from vosk import Model, KaldiRecognizer
import json
import os
import subprocess
import time
import datetime

# --- Konfiguracja Ścieżek ---
VOSK_MODEL_PATH = os.path.expanduser("~/models/vosk/pl_model") # Pełna ścieżka do rozpakowanego modelu Vosk
PIPER_EXECUTABLE_PATH = os.path.expanduser("~/models/piper/piper") # Pełna ścieżka do wykonywalnego pliku Piper
PIPER_MODEL_PATH = os.path.expanduser("~/models/piper/pl_PL-bazyli-low.onnx") # Pełna ścieżka do modelu głosu Piper
PIPER_CONFIG_PATH = os.path.expanduser("~/models/piper/pl_PL-bazyli-low.onnx.json") # Pełna ścieżka do pliku konfiguracyjnego Pipera

# --- Inicjalizacja Rozpoznawania Mowy (Vosk) ---
if not os.path.exists(VOSK_MODEL_PATH):
print(f"Błąd: Model Vosk nie znaleziony w {VOSK_MODEL_PATH}")
print("Upewnij się, że pobrałeś i rozpakowałeś model Vosk.")
exit()

vosk_model = Model(VOSK_MODEL_PATH)
recognizer = KaldiRecognizer(vosk_model, 16000) # 16000 Hz to standardowa częstotliwość dla Vosk

# --- Funkcja Syntezy Mowy (Piper) ---
def text_to_speech(text, filename="temp_speech.wav"):
if not os.path.exists(PIPER_EXECUTABLE_PATH):
print(f"Błąd: Plik wykonywalny Piper nie znaleziony w {PIPER_EXECUTABLE_PATH}")
print("Upewnij się, że pobrałeś i rozpakowałeś Pipera oraz model głosu.")
return None

command = [
PIPER_EXECUTABLE_PATH,
"--model", PIPER_MODEL_PATH,
"--output_file", filename,
"--json_config", PIPER_CONFIG_PATH
]
try:
process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
stdout, stderr = process.communicate(input=text.encode('utf-8'))
if process.returncode != 0:
print(f"Błąd TTS (Piper): {stderr.decode('utf-8')}")
return None
return filename
except Exception as e:
print(f"Błąd TTS: {e}")
return None

# --- Funkcja Odtwarzania Audio ---
def play_audio(filename):
if filename and os.path.exists(filename):
try:
subprocess.run(["aplay", filename], check=True)
os.remove(filename) # Usuń tymczasowy plik audio
except subprocess.CalledProcessError as e:
print(f"Błąd odtwarzania audio (aplay): {e}")
except Exception as e:
print(f"Ogólny błąd odtwarzania: {e}")
else:
print(f"Nie znaleziono pliku audio do odtworzenia: {filename}")

# --- Funkcja Mówienia (TTS + Odtwarzanie) ---
def speak(text):
print(f"Asystent mówi: {text}")
audio_file = text_to_speech(text)
play_audio(audio_file)

# --- Funkcja Rozpoznawania Mowy (offline z Vosk) ---
def listen_command():
r = sr.Recognizer()
# Użyj ReSpeakera jako źródła, upewnij się, że jest domyślny lub ustawiony
# Jeśli arecord -l pokazuje ReSpeaker jako card 0, device 0, to powinno działać
with sr.Microphone(sample_rate=16000) as source:
print("Słucham...")
# W zależności od tego jak długo chcesz słuchać, możesz dostosować listen()
# lub użyć adjust_for_ambient_noise()
try:
audio = r.listen(source, timeout=5, phrase_time_limit=5) # Słuchaj max 5s
except sr.WaitTimeoutError:
print("Nie usłyszałem niczego.")
return ""

try:
# Użycie Vosk Recognizer bezpośrednio z biblioteki SpeechRecognition
# Możliwe jest też użycie KaldiRecognizer z Vosk bezpośrednio do strumienia
# Ale SR upraszcza nagrywanie z mikrofonu

# Opcja 1: Użycie SpeechRecognition z backendem Vosk
# (Wymaga zainicjowania Recognizera z modelem Vosk, jeśli masz zaimplementowane)
# Na potrzeby prostoty, ten przykład użyje Vosk bezpośrednio z obiektu Recognizer

# Opcja 2: Ręczne przetwarzanie z Vosk (bardziej bezpośrednie)
# Nagraj audio bezpośrednio do pliku i przetwórz go Voskiem

# Na razie użyjemy uproszczonej wersji, która wysyła audio do Vosk
# z uwagami, że Vosk działa najlepiej, gdy dostaje surowy strumień.
# Dla prostoty: SpeechRecognition może konwertować audio do formatu Vosk.

raw_audio_data = audio.raw_data # Surowe dane audio z SpeechRecognition

if recognizer.AcceptWaveform(raw_audio_data):
result_json = recognizer.Result()
else:
result_json = recognizer.PartialResult() # Jeśli nie ma pełnego zdania, weź częściowy wynik

result = json.loads(result_json)
recognized_text = result.get('text', '')
print(f"Rozpoznano: {recognized_text}")
return recognized_text.lower() # Zwracaj małe litery dla łatwiejszego porównania

except sr.UnknownValueError:
print("Nie rozumiem mowy.")
return ""
except sr.RequestError as e:
print(f"Błąd usługi rozpoznawania mowy: {e}")
return ""
except Exception as e:
print(f"Nieoczekiwany błąd podczas rozpoznawania mowy: {e}")
return ""

# --- Główna logika asystenta ---
def main_assistant():
speak("Witaj! Jestem Twoim prostym asystentem. Możesz powiedzieć 'cześć' lub 'jaka jest godzina'.")

while True:
command = listen_command()

if "cześć" in command or "hej" in command:
speak("Cześć! Jak się masz?")
elif "jaka jest godzina" in command or "która godzina" in command:
now = datetime.datetime.now()
current_time = now.strftime("Jest godzina %H i %M.")
speak(current_time)
elif "do widzenia" in command or "pa pa" in command:
speak("Do widzenia! Miło było pomóc.")
break
elif command: # Jeśli coś rozpoznano, ale nie pasuje do komend
speak("Przepraszam, nie rozumiem tej komendy. Mogę powiedzieć 'cześć' lub 'jaka jest godzina'.")
# else: # Jeśli nic nie rozpoznano, pętla się powtórzy i ponownie będzie słuchać


if __name__ == "__main__":
main_assistant()
