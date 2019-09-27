import synchronizer,time,shared_memory_py

master = synchronizer.Master("sync_mem")


# running in sync mode for 4 seconds.
# as fast as possible
print("sync: slave running as fast as possible")
master.start_sync(-1)
time_start = time.time()
while time.time()-time_start < 4 :
    master.pulse()

# stopping synchronization,
# slave can run at whatever frequency
# it is set to (in this case 3Hz)
master.stop_sync()
print("async: slave running at dedicated frequency")
time.sleep(3)

# running in sync mode for 4 seconds.
# as fast as possible
print("sync: slave running as fast as possible")
master.start_sync(-1)
time_start = time.time()
while time.time()-time_start < 4 :
    master.pulse()

print("master exit")
