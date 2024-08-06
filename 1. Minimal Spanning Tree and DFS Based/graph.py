import numpy
import matplotlib.pyplot as plt
from sys import argv
from os import listdir

def plots(filename, graphtype):
    file = open("./data/"+filename,"r")
    for line in file:
        if line.strip() == "NODE_COORD_SECTION":
            break
    
    points = []

    for line in file:
        if line.strip() == "EOF":
            break
        data = line.strip().split()
        
        points.append([int(data[1]), int(data[2])])

    
    file = open("./"+graphtype+"/"+filename,"r")
    edges = []
    for line in file:
        data = line.strip().split()
        edges.append([int(data[0]), int(data[1])])
        
    # print(points)
    # print(edges)
    points = numpy.array(points)
    edges = numpy.array(edges)

    x = points[:,0].flatten()
    y = points[:,1].flatten()

    return [x[edges.T], y[edges.T]]
    plt.plot(x[edges.T], y[edges.T], linestyle='-', color='y',
            markerfacecolor='red', marker='o') 

    plt.show()

def plot(s):
    f, axs = plt.subplots(1, 3, sharey=True)
    names = ["tree","path","random"]

    for i in range(len(axs)):
        a, b = plots(s, names[i])
        axs[i].plot(a, b, linestyle='-', color='y',
            markerfacecolor='red', marker='o')
        axs[i].title.set_text(names[i])
    f.suptitle(s)
    f.set_figwidth(42)
    f.set_figheight(7)
    f.savefig("graph/"+s+".png")

def plotall(ss):
    f, axss = plt.subplots(len(ss), 3)
    names = ["tree","path","random"]

    for i in range(len(ss)):
        for j in range(len(names)):
            a, b = plots(ss[i], names[j])
            axss[i][j].plot(a, b, linestyle='-', color='y',
                markerfacecolor='red', marker='o',scaley=True)
            axss[i][j].title.set_text(ss[i]+" "+names[j])
            # axss[i][j].use_sticky_edges = True
        # axss[i].suptitle(ss[i])
    # f.suptitle(s)
    f.set_figwidth(7*3)
    f.set_figheight(5*9)
    f.tight_layout()
    
    f.savefig("graph/all.png")
    
    
from os import system
# plotall(listdir("data"))
# plot("dca1389.tsp")
plot("xvb13584.tsp")
