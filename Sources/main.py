from snowboy import snowboydecoder
import os
from pyvi import ViTokenizer, ViPosTagger, ViUtils
import speech_recognition as sr
from gtts import gTTS
from rgb_led.pixels import Pixels

led = Pixels()
led.wakeup()
led.think()
r = sr.Recognizer()
mic = sr.Microphone()
detector = snowboydecoder.HotwordDetector("my_model.pmdl", sensitivity=0.5)

my_control_dictionary = {
    "on":["mở", "bật"],
    "off":["đóng", "tắt"],
    "relay_1":["1", "đèn", "nhất", "một"],
    "relay_2":["2", "hai", "quạt"],
    "relay_3":["3", "ba"],
    "relay_4":["4", "bốn", "tư"],
    "relay_all":["tất_cả", "hết", "toàn_bộ"],
}

def callback():
    print("Hotword detected")
    led.wakeup()
    led.speak()
    os.system("sudo mpg123 welcome.mp3")
    with mic as source:
        r.adjust_for_ambient_noise(source)
        print("Say something!")
        led.think()
        audio = r.listen(source)
        print("Stop listen!")
        try:
            print("Process audio!!")
            text = r.recognize_google(audio, language='vi-VN').lower()
            print("You said:", text)
            data = ViTokenizer.tokenize(text)
            tokens,tags = ViPosTagger.postagging(data)
            key_word = [tokens[i] for i in range(len(tokens)) if tags[i] in ['N', 'V', 'M','P']]
            special_key_word = []
            for word in key_word:
                for key, value in my_control_dictionary.items():
                    if word in value:
                        special_key_word.append(key)
            try:
                command = 'echo "' +  special_key_word[0] + '" >> /proc/' + special_key_word[1]
                print(os.system(command))
                respone = "Vâng tôi đã " + key_word[0]
                print(respone)
                gTTS(text=respone, lang='vi').save("respone.mp3")
                os.system("sudo mpg123 respone.mp3")
            except:
                os.system("sudo mpg123 not_detected.mp3")
        except sr.UnknownValueError:
            os.system("sudo mpg123 not_detected.mp3")
    led.off()

print("Program setup done!!")
led.off()
def interrupt_callback():
    return False

detector.start(detected_callback=callback,
               interrupt_check=interrupt_callback,
               sleep_time=0.03)
detector.stop()

