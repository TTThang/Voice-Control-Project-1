cmd_/home/thang/Project_1/driver/driver.mod := printf '%s\n'   driver.o | awk '!x[$$0]++ { print("/home/thang/Project_1/driver/"$$0) }' > /home/thang/Project_1/driver/driver.mod
