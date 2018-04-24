AES Encryption and Decryption
## Environment : Dev C++
## Uint : Hex

## intput
<AES Encryption><br>
Plaintext: a3 c5 08 08 78 a4 ff d3 00 ff 36 36 28 5f 01 02<br>
Key: 36 8a c0 f4 ed cf 76 a6 08 a3 b6 78 31 31 27 6e<br>
  
## output
--------Encryption--------<br>
S0: 95 4f c8 fc 95 6b 89 75 08 5c 80 4e 19 6e 26 6c<br>
S1: b8 8e 7b 5f d0 85 77 2d ba 81 47 eb 88 01 cc 99<br>
S2: 69 66 3a 41 30 d9 3a c2 db 3e bc 8c d5 89 b1 8c<br>
S3: a1 08 a2 2e d8 48 31 5d 3d 28 fd f6 c5 72 ad cb<br>
S4: 5c c0 34 88 e9 da 78 b0 28 99 1f 10 20 02 37 ac<br>
S5: e1 12 c2 ac 45 d2 2b c9 d0 53 3a 71 aa 5f a2 f2<br>
S6: 94 00 6c e3 e7 91 46 9c 30 e0 eb 8c 4f bb 89 64<br>
S7: 12 2a 3d 36 65 3d 98 bd 19 d1 de 38 2c c9 fd a1<br>
S8: 57 97 d6 e9 c9 5e 0b 3e 68 c4 ff c9 c9 ce 25 59<br>
S9: 0a 07 b8 81 6d 31 93 45 7b d2 cc 4b b9 fc 73 9f<br>
Ciphertext: a6 24 62 48 34 dd a8 b9 1a f1 73 5d 00 0e cf 61<br>
--------Decryption--------<br>
S'0: 67 c7 4b db 3c b5 8f 0c 21 b0 6c 6e 56 c5 dc b3<br>
S'1: 5b 58 16 cb dd 1c 3f 1e 45 8b f6 b2 dd 88 2b dd<br>
S'2: c9 27 1d 32 4d 3e 54 05 d4 dd 27 7a 71 e5 46 07<br>
S'3: 22 81 e9 43 94 e1 a7 11 04 ea 50 de 84 63 5a 64<br>
S'4: f8 b5 80 89 6e ed 3a 91 70 cf 25 dd ac c9 f1 a3<br>
S'5: 4a 57 c0 91 1e ee 9a c4 34 77 18 e7 b7 ba bc ca<br>
S'6: 32 52 54 1f 61 34 95 31 27 40 3a 4c a6 30 c7 42<br>
S'7: f9 35 65 64 04 b2 c8 83 b9 a7 80 25 03 33 80 64<br>
S'8: 6c 97 a0 ee 70 0c 4b cf f4 7c 21 d8 c4 19 f5 e9<br>
S'9: 2a 7f cd 50 2a 4a f7 b0 30 9f e8 9d d4 84 a7 2f<br>
Plaintext: a3 c5 08 08 78 a4 ff d3 00 ff 36 36 28 5f 01 02<br>
