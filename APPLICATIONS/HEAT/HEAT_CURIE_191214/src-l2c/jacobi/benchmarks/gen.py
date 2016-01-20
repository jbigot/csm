import sys

#generation of the driverJacobi of Julien
#attention a changer les nb_tile pour chaque cas !
#64 procs : 8*8
#128 procs : 8*16
#256 procs : 16*16
#512 procs : 16*32
#1024 procs : 32*32
#2048 procs : 64*32
#4096 procs : 64*64
            
def main():
    filename = sys.argv[1]
    nb_proc = sys.argv[2]
    domaine = sys.argv[3]
    nb_iter = sys.argv[4]
    
    f = open(filename, 'w')
    f.write("<?xml version='1.0'?>\n")
    f.write("<lad xmlns='http://avalon.inria.fr/lad'>\n")
    f.write("<mpi>\n")

    for i in range(int(nb_proc)):
        f.write("<process>\n")
        f.write("<start_property instance=\"xpdriver_"+str(i)+"\" property=\"go\"/>\n")
        f.write("<instance id=\"xpdriver_"+str(i)+"\" type=\"SimpleXpDriver\">\n")
        f.write("<property id=\"height\"><value type=\"uint64\">"+str(domaine)+"</value></property>\n")
        f.write("<property id=\"width\"><value type=\"uint64\">"+str(domaine)+"</value></property>\n")
        f.write("<property id=\"computer\"><cppref instance=\"driver_"+str(i)+"\" property=\"xpcomputer\"/></property>\n")
        f.write("</instance>\n")
        f.write("<instance id=\"driver_"+str(i)+"\" type=\"MPIDriver\">\n")
        f.write("<property id=\"nbiter\"><value type=\"uint32\">"+str(nb_iter)+"</value></property>\n")
        f.write("<property id=\"debug\"><value type=\"uint8\">0</value></property>\n")
        f.write("<property id=\"nb_tiles_x\"><value type=\"uint16\">64</value></property>\n")
        f.write("<property id=\"nb_tiles_y\"><value type=\"uint16\">32</value></property>\n")
        f.write("<property id=\"computer\"><cppref instance=\"core_"+str(i)+"\" property=\"computer\"/></property>\n")
        f.write("<property id=\"initialization\"><cppref instance=\"init_"+str(i)+"\" property=\"initializer\"/></property>\n")
        f.write("</instance>\n")
        f.write("<instance id=\"init_"+str(i)+"\" type=\"DataInitializer\" >\n")
        f.write("<property id=\"top\"><value type=\"double\">1</value></property>\n")
        f.write("<property id=\"bottom\"><value type=\"double\">0</value></property>\n")
        f.write("<property id=\"left\"><value type=\"double\">0</value></property>\n")
        f.write("<property id=\"right\"><value type=\"double\">0</value></property>\n")
        f.write("<property id=\"inside\"><value type=\"double\">0</value></property>\n")
        f.write("</instance>\n")
        f.write("<instance id=\"core_"+str(i)+"\" type=\"JacobiCore\"/>\n")
        f.write("</process>\n")

    f.write("<communicator>\n")
    for i in range(int(nb_proc)):
        f.write("<peer instance=\"driver_"+str(i)+"\" property=\"comm\"/>\n")
    f.write("</communicator>\n")

    f.write("</mpi>\n")
    f.write("</lad>\n")

    f.close()


if __name__ == '__main__':
    main()

