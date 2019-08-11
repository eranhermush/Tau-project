import os, errno, sys
from random import choice

def create(path_to_exec, path_to_dir, workers_amount):
	# Make sure the dir exists taken from
	# https://stackoverflow.com/questions/273192/how-can-i-safely-create-a-nested-directory
	try:
		os.makedirs(path_to_dir)
	except OSError as e:
   		if e.errno != errno.EEXIST:
			raise
	# create ids
	id_options = list(range(1,10000))
	ids = set()
	while len(ids) < workers_amount:
		ids.add(choice(id_options))
	# create workers
	for id in ids:
		pid = os.fork()
		if pid == 0:
			import time
			time.sleep(1)
			os.execl(path_to_exec, path_to_exec, path_to_dir, str(id))
			return # on the safe side
		else:
			continue


if __name__ == "__main__":
	if(len(sys.argv)) < 4:
		print("Usage error: expecting 3 parameters:")
		print("\tPath to Worker executable")
		print("\tPath to working directory")
		print("\tWorker amount")
		sys.exit(1)
	create(sys.argv[1], sys.argv[2], int(sys.argv[3]))
	