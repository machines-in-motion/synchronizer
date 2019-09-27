import synchronizer

slave = synchronizer.Slave("sync_mem",3)

counter = 0

while (True):
    counter+=1
    slave.pulse()
    print("pulse",counter)


    
