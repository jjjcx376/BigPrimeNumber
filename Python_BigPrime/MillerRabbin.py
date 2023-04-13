import ctypes
import math
import random
import os


# 用户输入位宽、Miller_Rabin测试的次数、输出文件名
def Miller_Rabin(width, times, file,result):
    width = int(width)
    # index = math.log2(width)

    # Mac下指令：
    # gcc -shared -Wl,-install_name,Miller_Rabbin.so -o Miller_Rabbin.so -fPIC Miller_Rabin.cpp
    # adder=ctypes.CDLL('./Miller_Rabbin.so')
    #num = random.randint(pow(2,  width-1), pow(2, width)-1)
    cmd = "5.exe {} {}".format(width,times)
    print(cmd)
    prime=int(os.popen(cmd).read(),2)
    print(prime)
    if prime not in result:
        with open(file, 'a+') as f:  # 追加写入，直到函数Miller_Rabin被停止调用
            f.write(str(prime) + '\n')
    result.append(prime)
    print(result)


if __name__ == '__main__':
    i=0
    while(i<5):
        Miller_Rabin(64, 10, "lh.txt",[])
        i+=1