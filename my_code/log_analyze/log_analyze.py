import xml.etree.ElementTree as ET
import sys,struct
import argparse
import math

PATH = './' 
TEST_LOG = 'test.log'
RESULT_SAVE = 'test_analyze.log'         
if __name__ == '__main__':
    
    log_f = open(PATH + TEST_LOG, "r")
    res_f = open(RESULT_SAVE, "w")
    lines = log_f.readlines()
    for line in lines:
        if line.find( 'atstime - systime diff time =') != -1:
            res_f.write(line)

    log_f.close()
    res_f.close()