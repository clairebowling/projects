def rotate_left(x, n):
    return (((x << n) | (x >> (32 - (n % 32)))) & 0xFFFFFFFF)

def Ch(x, y, z):
    return ((x & y) ^ ((~x) & z))

def Parity(x, y, z):
    return (x ^ y ^ z)

def Maj(x, y, z):
    return ((x & y) ^ (x & z) ^ (y & z))


def sha1(message):
    h0 = 0x67452301
    h1 = 0xEFCDAB89
    h2 = 0x98BADCFE
    h3 = 0x10325476
    h4 = 0xC3D2E1F0

    # Pre-processing and padding of original message
    msg_length = len(message)

    # print(msg_length)

    # Convert message string to 8-bit ascii equivalent
    padded_message = ''.join(format(ord(ch), '08b') for ch in message)
    # Add 1
    padded_message += '1'
    #Pad with zeros
    l = msg_length * 8
    # k = (448 - (l + 1) ) % 512
    while len(padded_message) % 512 != 448:
        padded_message += '0'

    # print(bits_length)

    padded_message += format(l, '064b')

    # print("Padded Message in binary:", padded_message)

    chunks = [padded_message[i:i+512] for i in range(0, len(padded_message), 512)]

    # break message into 512-bit chunks
    for chunk in chunks:
        print(len(chunk))
        words = [chunk[i:i+32] for i in range(0, len(chunk), 32)]
        print(word for word in words)
        w = [0] * 80
        for i in range(0, len(chunk), 32):
            w[i] = int.from_bytes(chunk[i:i*32].encode(), byteorder='big')
            # int(chunk[i:i*32], base=2)
        #     block = chunk[i:i+32]
        #     w[i] = int(block, 2)


        for i in range(16, 80):
            w[i] = rotate_left((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1)

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

    hash = (h0 << 128) | (h1 << 96) | (h2 << 64) | (h3 << 32) | h4
    return format(hash, '040x')

if __name__ == "__main__":

    messages = [
        "hello world."]

    for message in messages:
        digest = sha1(message)
        print(digest)
