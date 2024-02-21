# Implementation of SHA-1 from FIPS 180-4

def rotate_left(x, n):
    return (((x << n) | (x >> (32 - (n % 32)))) & 0xFFFFFFFF)

def Ch(x, y, z):
    return ((x & y) ^ (~x & z))

def Parity(x, y, z):
    return (x ^ y ^ z)

def Maj(x, y, z):
    return ((x & y) ^ (x & z) ^ (y & z))

def main():
    h0 = 0x67452301
    h1 = 0xEFCDAB89
    h2 = 0x98BADCFE
    h3 = 0x10325476
    h4 = 0xC3D2E1F0

    # Pre-processing and padding of original message
    message = "hello world."
    original_len = len(message) * 8
    message += chr(128)

    while (len(message) * 8) % 512 != 448:
        message += chr(0)

    message += original_len.to_bytes(8, byteorder='big').decode()

    chunks = [message[i:i+64] for i in range(0, len(message), 64)]

    # break message into 512-bit chunks
    for chunk in chunks:
        w = [0] * 80
        for i in range(0, 64, 4):
            w[i//4] = int.from_bytes(chunk[i:i+4].encode(), byteorder='big')

        for i in range(16, 80):
            w[i] = rotate_left(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1)

        a = h0
        b = h1
        c = h2
        d = h3
        e = h4

        for i in range(0, 80):
            if 0 <= i <= 19:
                f = Ch(b, c, d)
                k = 0x5A827999
            elif 20 <= i <= 39:
                f = Parity(b, c, d)
                k = 0x6ED9EBA1
            elif 40 <= i <= 59:
                f = Maj(b, c, d)
                k = 0x8F1BBCDC
            elif 60 <= i <= 79:
                f = Parity(b, c, d)
                k = 0xCA62C1D6

            temp = (rotate_left(a, 5) + f + e + k + w[i]) & 0xFFFFFFFF

            e = d
            d = c
            c = rotate_left(b, 30)
            b = a
            a = temp

        h0 = (h0 + a) & 0xFFFFFFFF
        h1 = (h1 + b) & 0xFFFFFFFF
        h2 = (h2 + c) & 0xFFFFFFFF
        h3 = (h3 + d) & 0xFFFFFFFF
        h4 = (h4 + e) & 0xFFFFFFFF

    hh = (rotate_left(h0, 128) | rotate_left(h1, 96) | rotate_left(h2, 64) | rotate_left(h3, 32) | h4)
    print(hex(hh))

if __name__ == "__main__":
    main()
