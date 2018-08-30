import socket
import requests
import pyaudio

url = "http://192.168.137.1:8080"
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("192.168.137.83", 0))
inputData = []

CHUNK = 512
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
SILENCE = chr(9) * CHUNK * CHANNELS

p = pyaudio.PyAudio()
stream = p.open(format = FORMAT, channels = CHANNELS, rate = RATE, input = True, output = True, frames_per_buffer = CHUNK)

def httpRequest(msg):
    response = requests.get(url=url, params=msg)
    print("http send complete: ", response.status_code)
    return response.status_code

def udpSend():
    while True:
        outputData = stream.read(CHUNK, exception_on_overflow=False)
        sock.sendto(outputData, ("192.168.137.1", 3000))
        inputData, adrr = sock.recvfrom(CHUNK)
        if inputData == '':
            inputData = SILENCE

        stream.write(inputData, CHUNK, exception_on_underflow=False)


def send():
    if(httpRequest("abc") == 200):
        udpSend()

def main():
    send()

if __name__ == "__main__":
    main()
