import ctypes
from ctypes import cdll, c_double



try:
    flycapture2 = cdll.LoadLibrary('C:/Users/User/Desktop/pythonTest/FlyCapture2_dll.dll')
    print("Find Library!")
except OSError:
    print("Error: could not find Library!")
    exit(1)
