import requests
import hashlib

hash = hashlib.sha256()

fileName = "./build/cp32wr.bin"

with open(fileName, mode='rb') as file: # b is important -> binary

  fileContent = file.read()
  hash.update(fileContent)
  print(hash.hexdigest());
  
  x = requests.put('http://192.168.0.24/update_firmware', data=fileContent,
                   headers={'Hash':hash.digest()},
                   timeout=20)

  # print(x.text)