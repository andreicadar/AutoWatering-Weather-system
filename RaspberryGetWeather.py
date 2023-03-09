#!/usr/bin/env python3

from bs4 import BeautifulSoup
import requests

headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36                                                                                 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3"
}

import serial
import time

if __name__ == "__main__":
    ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=2)
    ser.reset_input_buffer()
    while True:
        city = "Cluj Naopoca"
        location = city
        city = city + " weather"
        city = city.replace(" ", "+")
        res = requests.get(
            f"https://www.google.com/search?q={city}&oq={city}&aqs=chrome.0.35i3                                                                                9l2j0l4j46j69i60.6128j1j7&sourceid=chrome&ie=UTF-8",
            headers=headers,
        )
        soup = BeautifulSoup(res.text, "html.parser")
        #location = soup.select("#wob_loc")[0].getText().strip()
        time2 = soup.select("#wob_dts")[0].getText().strip()
        info = soup.select("#wob_dc")[0].getText().strip()
        info = info.replace("ș", "s")
        info = info.replace("Î", "I")
        info = info.replace("î", "i")
        info = info.replace("ț", "t")
        info = info.replace("Ț", "ț")
        info = info.replace("ă", "a")
        if info == "In mare parte innorat" or info == "Cer senin, temporar noros                                                                                ":
            info = "Mostly innorat"
        weather = soup.select("#wob_tm")[0].getText().strip()
        result = location + ";" + info + " " + weather + "\n"
        result = result.replace("ș", "s")
        result = result.replace("Î", "I")
        result = result.replace("î", "i")
        print(result)
        ser.write(result.encode("ascii"))
        time.sleep(3)
