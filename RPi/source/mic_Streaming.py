import socket
import requests
import pyaudio

url = "http://192.168.1.101:8080"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("192.168.1.117", 0))

CHUNK = 2048
FORMAT = pyaudio.paUInt8
CHANNELS = 1
RATE = 16000

p = pyaudio.PyAudio()
inputStream = p.open(format = FORMAT, channels = CHANNELS, rate = RATE, input = True, output = False, frames_per_buffer = CHUNK)
outputStream = p.open(format = FORMAT, channels = CHANNELS, rate = RATE, input = False, output = True, frames_per_buffer = CHUNK)
def httpRequest(msg):
    response = requests.get(url=url, params=msg)
    print("http send complete: ", response.status_code)
    return response.status_code

def udpSend():
    while True:
        outputData = inputStream.read(1600,exception_on_overflow = False)
        sock.sendto(outputData, ("192.168.1.101", 3000))
        inputData = sock.recv(1600)
        outputStream.write(inputData, 1600)

def send():
    if(httpRequest("abc") == 200):
        udpSend()

def main():
    send()

if __name__ == "__main__":
    main()
