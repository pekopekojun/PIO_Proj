import sys

# refer to https://www.tutimogura.com/python-bitmap-read/

f = open(sys.argv[1], "rb")

bfType = f.read(2)
bfSize = f.read(4)
bfReserved1 = f.read(2)
bfReserved2 = f.read(2)
bfOffBitsbfOffBits = f.read(4)


bcSize = f.read(4)
bcWidth = f.read(4)
bcHeight = f.read(4)
bcPlanes = f.read(2)
bcBitCount = f.read(2)
biCompression = f.read(4)
biSizeImage = f.read(4)
biXPixPerMeter = f.read(4)
biYPixPerMeter = f.read(4)
biClrUsed = f.read(4)
biCirImportant = f.read(4)


bfType_str = bfType.decode()
bfOffBitsbfOffBits_int = int.from_bytes(bfOffBitsbfOffBits, "little")
bcSize_int = int.from_bytes(bcSize, "little")
bcWidth_int = int.from_bytes(bcWidth, "little")
bcHeight_int = int.from_bytes(bcHeight, "little")
bcBitCount_int = int.from_bytes(bcBitCount, "little")
biCompression_int = int.from_bytes(biCompression, "little")


if (bfType_str != "BM") or \
   (bcSize_int != 40) or \
   (bcBitCount_int != 24) or \
   (biCompression_int != 0):
    print("### Format is not supported! ###")
    sys.exit()

#print("(Width,Height)=(%d,%d)" % (bcWidth_int, bcHeight_int))

offset = bfOffBitsbfOffBits_int-54
f.read(offset)


print(f"static const uint32_t data[{bcHeight_int}][{ bcWidth_int}]", end="")
print("={")
for y in range(bcHeight_int):
    print("{", end="")
    for x in range(bcWidth_int):
        if x != 0:
            print(",", end="")

        #R = int.from_bytes(f.read(1), "little")
        #G = int.from_bytes(f.read(1), "little")
        #B = int.from_bytes(f.read(1), "little")
        B = int.from_bytes(f.read(1), "little")
        G = int.from_bytes(f.read(1), "little")
        R = int.from_bytes(f.read(1), "little")
        rgb = 0
        
        #r = (uint8_t)(c >> 16),
        #g = (uint8_t)(c >>  8),
        #b = (uint8_t)c;
        rgb |= R << 16
        rgb |= G << 8
        rgb |= B
        print('0x%06x' % rgb, end="")
    print("},")
print("};")

sys.exit()

######################
### 画像データ処理 ###
######################
### 画像処理パラメータ ###
gain = 1.5

### 画像データ処理開始 ###
dummy_size = 0
mod = (bcWidth_int*3) % 4
if (mod != 0):
    dummy_size = 4-mod
for y in range(bcHeight_int):
    for x in range(bcWidth_int):
        R = int.from_bytes(f.read(1), "little")
        G = int.from_bytes(f.read(1), "little")
        B = int.from_bytes(f.read(1), "little")
        ### 画像処理(ゲイン) ###
        R = min(int(R*gain), 255)
        G = min(int(G*gain), 255)
        B = min(int(B*gain), 255)
        ### 処理結果を書き込む ###
        fw.write(R.to_bytes(1, "little", signed=False))
        fw.write(G.to_bytes(1, "little", signed=False))
        fw.write(B.to_bytes(1, "little", signed=False))
    ### 画像の横ラインデータサイズを4の倍数にそろえる ###
    for i in range(dummy_size):
        tmp = int.from_bytes(f.read(1), "little")
        fw.write((255).to_bytes(1, "little", signed=False))

### ファイルオブジェクトをclose ###
f.close()
fw.close()
