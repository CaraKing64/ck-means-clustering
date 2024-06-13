import random, sys, math, string

def readFileToArray(fname):
    rls = []
    with open(fname, "r") as doc:
        rls = doc.readlines()
    ls = []
    for rl in rls:
        templ = rl.replace("\n", "").split(',')
        temprow = []
        for i in templ:
            temprow.append(float(i))
        ls.append(temprow)
    return ls
        
def randFloat(min, max, r=2):
    return (round(random.random() * (max-min) + min, r))

def distance(l1, l2, nVars, r=3):
    sum = 0
    for i in range(nVars):
        sum += (l1[i]-l2[i])**2
    return(round(math.sqrt(sum), r))
    
def printArray(array):
    for r in range(len(array)):
        print(array[r])

dataFilename = -1
nValues = -1
nDimensions = -1
startK = 1
maxK = -1
attempts = -1
debug_mode = False

argv = sys.argv
argc = len(argv)
for i in range(argc):
    arg = argv[i]
    if arg == "-f":
        dataFilename = argv[i+1]
    elif arg == "-sk":
        startK = int(argv[i+1])
    elif arg == "-mk":
        maxK = int(argv[i+1])
    elif arg == "-k":
        startK = int(argv[i+1])
        maxK = int(argv[i+1])
    elif arg == "-a":
        attempts = int(argv[i+1])
    elif arg == "-d":
        debug_mode = True
    
if dataFilename == -1:
    dataFilename = "output.csv"
    raise Exception("No file name provided")

data = readFileToArray(dataFilename)

nValues = len(data)
nDimensions = len(data[0])

if startK == -1:
    startK = 1
if maxK == -1:
    maxK = nValues
if attempts == -1:
    attempts = 10

if debug_mode or True:
    print(f"Data: {dataFilename}") 
    print(f"nValues: {nValues}") 
    print(f"nDimensions: {nDimensions}") 
    print(f"startK: {startK}")
    print(f"maxK: {maxK}")
    print(f"attempts: {attempts}")
    print(f"debug_mode: {debug_mode}")
    print()

mins = []
maxs = []

if debug_mode:
    print("Data")
    printArray(data)
    print()

for c in range(nDimensions):
    temp = []
    for r in range(nValues):
        temp.append(data[r][c])
    minV = min(temp)
    maxV = max(temp)
    mins.append(minV)
    maxs.append(maxV)

if debug_mode:
    print("mins")
    print(mins)
    print("maxs")
    print(maxs)
    print()

kCentroidList = []
kErrorList = []
for i in range(startK, maxK+1):
    kCentroidList.append({})
    kErrorList.append(10_000_000_000_000)
if debug_mode:
    print("kCentroid List")
    print(kCentroidList)
    print("kError List")
    print(kErrorList)
    print()
for k in range(startK, maxK+1):
    
    for attempt in range(1, attempts+1):
        finished = False
        centroids = []
        
        # generate centroids
        for c in range(k):
            c_row = []
            for d in range(nDimensions):
                c_row.append(randFloat(mins[d], maxs[d]))
            centroids.append(c_row)

        if debug_mode:
            print("Centroids")
            printArray(centroids)
            print()

        # iterate until all centroids stop moving
        while not finished:       
            assignments = []
            for i in range(k):
                assignments.append([])

            for point_i in range(nValues):
                point = data[point_i]
                distances = []
                for i in range(k):
                    distances.append(distance(point, centroids[i], nDimensions))

                cluster = distances.index(min(distances))
                assignments[cluster].append(point_i)

            if debug_mode:
                print("Assignments")
                printArray(assignments)
                print()

                print("REASSIGNING")
            for c in range(len(centroids)):
                if len(assignments[c]) != 0:
                    pos = []
                    for _ in range(nDimensions):
                        pos.append(0)
                    for a in range(len(assignments[c])):
                        p = data[assignments[c][a]]
                        for v in range(nDimensions):
                            pos[v] += p[v]
                    for v in range(nDimensions):
                        pos[v] = round(pos[v] / len(assignments[c]), 3)
                else:
                    pos = centroids[c] 

                old_pos = centroids[c]
                centroids[c] = pos

                if debug_mode:
                    print(c)
                    print(old_pos)
                    print(pos)
                    print()
                if old_pos == pos:
                    finished = True


        error = 0
        for a in range(len(assignments)):
            for v in assignments[a]:
                dist = distance(data[v], centroids[a], nDimensions)
                error += dist
                #print(data[v], centroids[a], dist)
                #input()

        error = round(error, 3)
        if kErrorList[startK-k] > error:
            kErrorList[startK-k] = error
        kCentroidList[startK-k][str(error)] = centroids
    

if debug_mode:

    print("kCentroid List")
    print(kCentroidList)
    print("kError List")
    print(kErrorList)
    print()

    print("kCentroidList", len(kCentroidList))
    for k in range(startK, maxK+1):
        for key in kCentroidList[startK-k].keys():
            print(key)
            printArray(kCentroidList[startK-k][key])
            print()
        print(f"k={k}  minError={(kErrorList[startK-k])}")
        print(kCentroidList[startK-k][str(kErrorList[startK-k])])


printArray(kErrorList)
if startK != maxK:
    chosen_k = int(input("What value of k would you like to output: "))
else:
    chosen_k = startK
final_centroids = kCentroidList[startK-chosen_k][str(kErrorList[startK-chosen_k])]
print(final_centroids)


with open("centroids.csv", "w+") as doc:
    for c in final_centroids:
        for v in range(len(c)):
            doc.write(f"{c[v]}")
            if v+1 != nDimensions:
                doc.write(",")

        doc.write("\n")