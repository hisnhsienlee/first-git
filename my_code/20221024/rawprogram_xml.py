import xml.etree.ElementTree as ET
import sys,struct
import argparse
import math

IMAGE_PATH = '../../images/SA515M/' 
RAWPROGRAM_XML = 'rawprogram_nand_p4K_b256K.xml'
RESULT_SAVE = 'rawprogram_nand_p4K_b256K_secelf_only.xml'         
if __name__ == '__main__':
    part_num = 0

    parser = argparse.ArgumentParser()
    parser.add_argument('-i', action="store", dest="IMAGE_PATH",
            help="path for image files and partition xml")
    args = parser.parse_args()

    IMAGE_PATH = args.IMAGE_PATH
    print("================================================")
    print("Start rawprogram_xml.py")
    print("List Environments")
    print("IMAGE_PATH:" + str(IMAGE_PATH))
    print("================================================")
    # case 1
    #tree = ET.parse('./rawprogram_nand_p4K_b256K.xml')
    #root = tree.getroot()
         
    #pros = root.findall('program')
    #for pro in pros:
    #    name = pro.get('filename')
    #    if(name == 'sec.elf'):
    #        print ("OK")
    #    else:
    #        root.remove(pro)
    #tree.write(RESULT_SAVE)
    # case 2
    
    xml_f = open(IMAGE_PATH + RAWPROGRAM_XML, "r")
    res_f = open(RESULT_SAVE, "w")
    lines = xml_f.readlines()
    for line in lines:
        if line.find( 'xml') != -1:
            res_f.write(line)
  
        if line.find('data') != -1:
            res_f.write(line)
   
        if line.find('sec.elf') != -1:
            res_f.write(line)

    xml_f.close()
    res_f.close()