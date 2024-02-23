# Implementation of SHA-1 as specified in FIPS 180-4 

#########################################################################
# Rotate left (circular left shift) from section 3.2
def rotate_left(x, n):
    return (((x << n) | (x >> (32 - n))) & 0xFFFFFFFF)

#########################################################################
# section 4.1.1 when 0 <= t <= 19
def Ch(x, y, z):
    return ((x & y) ^ ((~x) & z))

#########################################################################
# section 4.1.1 when 20 <= t <= 19 and 60 <= t <= 79
def Parity(x, y, z):
    return (x ^ y ^ z)

#########################################################################
# section 4.1.1 when 40 <= t <= 59
def Maj(x, y, z):
    return ((x & y) ^ (x & z) ^ (y & z))

#########################################################################
# calculate hash value
def sha1(message):
    h0 = 0x67452301
    h1 = 0xEFCDAB89
    h2 = 0x98BADCFE
    h3 = 0x10325476
    h4 = 0xC3D2E1F0

    # Pre-processing and padding of original message
    msg_length = len(message)

    # Convert message string to 8-bit ascii equivalent
    padded_message = ''.join(format(ord(ch), '08b') for ch in message)
    # Add 1
    padded_message += '1'
    l = msg_length * 8
    # Pad message with 0s such that 
    while len(padded_message) % 512 != 448:
        padded_message += '0'

    padded_message += format(l, '064b')

    
    chunks = [padded_message[i:i+512] for i in range(0, len(padded_message), 512)]

    # break message into 512-bit chunks
    for chunk in chunks:
        w = [0] * 80
        for i in range(0, 16):
            w[i] = int(chunk[i*32:(i*32)+32], 2)

        for i in range(16, 80):
            w[i] = rotate_left((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1)

        # initialize hash value for this chunk
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

        # add this chunk's hash value
        h0 = (h0 + a) & 0xFFFFFFFF
        h1 = (h1 + b) & 0xFFFFFFFF
        h2 = (h2 + c) & 0xFFFFFFFF
        h3 = (h3 + d) & 0xFFFFFFFF
        h4 = (h4 + e) & 0xFFFFFFFF

    # return final hash value
    hash = (h0 << 128) | (h1 << 96) | (h2 << 64) | (h3 << 32) | h4
    return format(hash, '040x')

if __name__ == "__main__":

    messages = [
        "This is a test of SHA-1.",
        "Kerckhoff's principle is the foundation on which modern cryptography is built.",
        "SHA-1 is no longer considered a secure hashing algorithm.",
        "SHA-2 or SHA-3 should be used in place of SHA-1.",
        "Never roll your own crypto!" 
        ]

    for message in messages:
        digest = sha1(message)
        print(digest)
